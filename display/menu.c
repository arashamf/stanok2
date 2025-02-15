
// Includes -------------------------------------------------------------------------------------------//
#include "menu.h"
#include "typedef.h"
#include "tim.h"
#include "button.h"
#include "drive.h"
#include "ssd1306.h"
#include "fonts.h"
#include "calc_value.h"
#include "encoder.h"
#include "usart.h"
#include "gpio.h"
#include "iwdg.h"

// Declarations and definitions ----------------------------------------------------------------------//

// Variables -----------------------------------------------------------------------------------------//
uint8_t count_delay = 0; //���������� �������� �� 10 �� ��� �������� ������� ��������� �����
__IO uint8_t drive_mode = PRESET1; //����� �������

// Functions -----------------------------------------------------------------------------------------//
void init_loop (void)	
{
	__IO uint16_t key_code = NO_KEY; //��� ������
	while (count_delay < START_DELAY) //�������� 1�
	{
		if ((key_code = start_scan_key_PEDAL()) == NO_KEY)  //���� ������ ������
		{	count_delay++; }
		else
		{
			release_pedal_menu (); //�������������� � ������� ������
			while ((key_code = start_scan_key_PEDAL()) != NO_KEY) {}
		}	
		delay_us (10000);
	}
	init_status_flags_drives();
}

//-----------------------------------------------------------------------------------------------------//
void main_loop (encoder_data_t * HandleEncData, coil_data_t ** HandleCoilData)
{
	__IO uint16_t key_code = NO_KEY; //��� ������
	if ((key_code = scan_keys()) != NO_KEY)
	{
		switch (key_code) //��������� ���� ������� ������
		{	
			case KEY_PEDAL_LONG:
				start_drives_turn (drive_mode, *(HandleCoilData + (drive_mode-1))); //��������� �������
				while (scan_button_PEDAL() != OFF) //�������� ���������� ������
				{}
				break;
					
			case KEY_SETUP_SHORT:	
				drive_mode = menu_select_preset(HandleEncData, HandleCoilData);	//�������������� �������� � ������� ������ ������������ ������� 
				break;
			
			case KEY_SETUP_LONG:
				if (drive_mode != NO_PRESET) //���� ���������� ����� ������������ ������� (1..4)
				{	settings_preset_reset (drive_mode, *(HandleCoilData + (drive_mode-1))); }//����� ������������ �������
				break;
				
			case KEY_NULL_SHORT:
			case KEY_NULL_LONG:
				mode_setup_null_screen();
				setup_null_position ();
				main_menu_select_preset_screen();
				break;
			
			case KEY_MANUAL_SHORT:
			case KEY_MANUAL_LONG:
				mode_setup_manual_dir_screen(); //����� �������� ������ ����������� ������ ������ ��������� ����������� ���������
				while (select_direction() == ERROR) {} //�������� ������� ������ ������ ����������� ����������� ���������
				mode_setup_manual_screen (); //����� �������� ������ ������ ��������� ����������� ���������
				manual_control_drive1 ();	//�-� ������ ��������� ��������� ����������� ���������
				main_menu_select_preset_screen(); //����� �������� ������ ������ �������
				break;
		
			default:
				break;	
		}			
	}
}

//-----------------------------------------------------------------------------------------------------//
void setup_menu (encoder_data_t * HandleEncData, coil_data_t * HandleCoilData)
{
	uint16_t key_code = NO_KEY;	
	
	//���� ��������
	if(HandleCoilData->drive2_turn_in_minute == 0) 
	{	HandleCoilData->drive2_turn_in_minute = BASE_TURN_IN_MINUTE; }
	
	Stop_Count_Timers(); //��������� ������� ��������
	setup_speed_screen (HandleCoilData->drive2_turn_in_minute);
	while (1)
	{
		if (read_encoder (HandleEncData) == ON) //���� ������ �� �������� ����������
		{
		//	HandleCoilData->rotation_speed += 5*HandleEncData->delta; //����������/���������� ��������� �������� � ��������� (��� ��������� - 5)
			HandleCoilData->drive2_turn_in_minute += 3*HandleEncData->delta;
			if (HandleCoilData->drive2_turn_in_minute > MAX_VALUE_TURN) //���� ���������� �������� � ������ ��������� ����������� ���������� ��������
			{	HandleCoilData->drive2_turn_in_minute = MIN_VALUE_TURN;	}
			else
			{
				if (HandleCoilData->drive2_turn_in_minute <	MIN_VALUE_TURN)
				{	HandleCoilData->drive2_turn_in_minute = MAX_VALUE_TURN;	}
			}
			HandleCoilData->pulse_frequency = calc_rotation_speed(HandleCoilData->drive2_turn_in_minute); //������ ���������� �������� � ������
			drive2_turn (HandleCoilData);	//������ ���������� ���������
			setup_speed_screen (HandleCoilData->drive2_turn_in_minute);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //���� ���� ������ ������
		{
			if (key_code ==  KEY_SETUP_SHORT) //�������� ������� ������ �������� - ������� � ����� ���������� ������ ��������� �������
			{	
				SoftStop_Drives() ;
				HardStop_Drives () ;
				status_drives.end_turn_drive2 = DRIVE_FREE;
				break;
			}
		}
	}	
	
	//��������� ������������� �����������
	if(HandleCoilData->gear_ratio == 0) 
	{	HandleCoilData->gear_ratio = DEFAULT_GEAR_RATIO; }	
	setup_ratio_screen (HandleCoilData->gear_ratio);
	
	while (1)
	{
		if (read_encoder (HandleEncData) == ON) //���� ������ �� �������� ����������
		{
			HandleCoilData->gear_ratio += 5*HandleEncData->delta;
			if (HandleCoilData->gear_ratio > MAX_VALUE_RATIO)
			{	HandleCoilData->gear_ratio = MIN_VALUE_RATIO;	}
			else
			{
				if (HandleCoilData->gear_ratio < MIN_VALUE_RATIO)
				{	HandleCoilData->gear_ratio = MAX_VALUE_RATIO;	}
			}
			drive1_turn (HandleCoilData);	//������ ����������� ���������
			drive2_turn (HandleCoilData);	//������ ���������� ���������
			setup_ratio_screen (HandleCoilData->gear_ratio);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //���� ���� ������ ������
		{
			if (key_code == KEY_SETUP_SHORT) //�������� ������� ������ �������� - ������� � ����� ���������� ������ ��������� �������
			{	
				SoftStop_Drives() ;
				HardStop_Drives () ;
				status_drives.end_turn_drive1 = DRIVE_FREE;		
				status_drives.end_turn_drive2 = DRIVE_FREE;
				break;
			}
		}
	}	

	//��������� ���������� ������ �������
	for (uint8_t count = 0; count < MAX_NUMBER_COIL; count++)
	{
		setup_coil_screen (count+1, HandleCoilData->set_coil[count]); //�������� ������ ��������� ���������� ������
		while (1)
		{
			if (read_encoder (HandleEncData) == ON) //���� ������ �� �������� ����������
			{
				HandleCoilData->set_coil[count] += HandleEncData->delta;	//����������� ���������� ��������� ��������
				if (HandleCoilData->set_coil[count] < MIN_VALUE_COIL)
				{	HandleCoilData->set_coil[count] = MAX_VALUE_COIL;	}
				else
				{
					if (HandleCoilData->set_coil[count] > MAX_VALUE_COIL)
					{	HandleCoilData->set_coil[count] = MIN_VALUE_COIL;	}
				}
				setup_coil_screen (count+1, HandleCoilData->set_coil[count]); //�������� ������ ��������� ���������� ������
			}
			if ((key_code = scan_keys()) != NO_KEY) //���� ���� ������ ������
			{
				if (key_code == KEY_SETUP_LONG) //������� ������� ������ �������� - ����� �� ������ ��������� ���������� ������
				{		
					HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count]; //��������� ����������� ���������� ������ �������					
					HandleCoilData->set_numb_winding = count+1; //����������� ���������� �������
					HandleCoilData->complet_winding = 0; //���������� ��������� ���������� �������				
					return;
				}	
				else
				{
					if (key_code == KEY_SETUP_SHORT) //�������� ������� ������ �������� - ������� � ����� ���������� ������ ��������� �������
					{
						HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count]; //��������� ����������� ���������� ������ �������
						break;
					}
				}
			}			
		}
	}
	HandleCoilData->set_numb_winding = MAX_NUMBER_COIL; //����������� ���������� �������
	HandleCoilData->complet_winding = 0;	//���������� ��������� ���������� �������
	return;
}

//-----------------------------------------------------------------------------------------------------//
uint8_t menu_select_preset (encoder_data_t * HandleEncData, coil_data_t ** HandleBufCoilData)
{
	__IO uint16_t key_code = NO_KEY;
	
	menu_select_preset_screen (drive_mode);
	while (1)
	{		
		if (read_encoder (HandleEncData) == ON) //������ ������ ��������
		{
			drive_mode += HandleEncData->delta; //��������� ��������� �� ����� �������
			if (drive_mode > PRESET4)
			{	drive_mode = PRESET1;	}
			else
			{	
				if (drive_mode < PRESET1) 
				{	drive_mode = PRESET4;	}
			}	
			HandleEncData->delta = 0;	
			menu_select_preset_screen (drive_mode);				
		}		
			
		if ((key_code = scan_keys()) != NO_KEY)
		{
			switch (key_code) //��������� ���� ������� ������
			{	
				case KEY_SETUP_SHORT:
					setup_menu (HandleEncData, *(HandleBufCoilData+drive_mode-1)); //������� � ���� ��������� �������
					break;
				
				case KEY_SETUP_LONG:
					turn_drive_menu (drive_mode, HandleBufCoilData[drive_mode-1], 0); //����������� ������ �������, � ���������� ���������� � �������������� ���������� ������ 1 �������
					return drive_mode; //����� � ������� ����
					break;
			}
			menu_select_preset_screen (drive_mode);		
		}
	}
}


//-----------------------------------------------------------------------------------------------------//
void turn_drive_menu (uint8_t numb_preset, coil_data_t * HandleCoilData, uint8_t numb_coil)
{
	default_screen (numb_preset, HandleCoilData->remains_coil[numb_coil], HandleCoilData->set_coil[numb_coil], numb_coil+1);	
}
