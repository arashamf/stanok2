
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "menu.h"
#include "usart.h"
#include "typedef.h"
#include "button.h"
#include "eeprom.h"

// Functions --------------------------------------------------------------------------------------//
static void init_drives_setting (coil_data_t * , uint8_t);
static void drives_one_full_turn (void);
static void drive1_turn (void);

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)
PWM_data_t Drives_PWM = {0};
volatile STATUS_FLAG_DRIVE_t status_drives; 

//-----------------------------------------------------------------------------------------------------//
void init_status_flags_drives (void) 
{
	status_drives.direction 			= 	RIGHT; //��������� �� �������, ����� ������� ������
	status_drives.end_turn_drive1 = 	DRIVE_FREE; //��������� 1 ��������
	status_drives.end_turn_drive2 = 	DRIVE_FREE; //��������� 2 ��������
	status_drives.stop_drives			=		START; //���������� �� ������ ����������
}

//-----------------------------------------------------------------------------------------------------//
static void init_drives_setting (coil_data_t * HandleCoilData, uint8_t number)
{
	uint32_t period_drive2 		=	1000000/HandleCoilData->pulse_frequency; //������ �������� �������� (���������) ��������� � ���
	uint32_t period_drive1 		=	(period_drive2*100)/HandleCoilData->gear_ratio;			//������ �������� ������� (�����������) ���������
	Drives_PWM.Compare_Drive1	=	(period_drive1/2)-1; 	//������������ �������� ������� (�����������) ��������� 1
	Drives_PWM.Period_Drive1	=	(period_drive1-1); 	//���������� ������� ������� (�����������) ��������� 1
	Drives_PWM.Compare_Drive2	=	(period_drive2/2) - 1; //������������ �������� �������� (���������) ��������� 2
	Drives_PWM.Period_Drive2	=	(period_drive2-1); 	//���������� ������� �������� (���������) ��������� 2
	Drives_PWM.turn_number 		= (HandleCoilData->remains_coil[number]); //���������� ���������� ������ �������� 
	Drives_PWM.number_cnt_PWM_DR1 = (PULSE_IN_TURN*HandleCoilData->gear_ratio)/100; //������� ��� ������� ���������� ���������� ��������� ������� ������������ ������ ����������
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "ratio=%u,Period=%u,Compare=%u\r\n", HandleCoilData->gear_ratio, Drives_PWM.Period_Drive1, Drives_PWM.Compare_Drive1);
		DBG_PutString(DBG_buffer);
	#endif
}

//---------------------������������� � ������ �������� ������� ������� ����������---------------------//
static void drives_one_full_turn (void) 
{
	DRIVE2_ENABLE(START);		//���������� ��������� ��������� 2
	DIR_DRIVE2 (BACKWARD); //����������� �������� ��������� 2
	DRIVE1_ENABLE(START); //���������� ��������� ��������� 1
	DIR_DRIVE1 (status_drives.direction); //����������� �������� ��������� 1
	delay_us (3);	
	
	Drives_PWM_start (&Drives_PWM); //�������� �������� � �������
}

//-----------------------������������� � ������ �������� ����������� ��������� 1-----------------------//
static void drive1_turn (void) 
{
	DRIVE1_ENABLE(START); //������ ���������� ������� ��������� 1
	DIR_DRIVE1 (status_drives.direction); //����������� �������� ��������� 1
	delay_us (2);		
	Drive1_PWM_start (&Drives_PWM); //�������� �������� � �������
}

//-----------------------------------------------------------------------------------------------------//
void settings_preset_reset (uint8_t numb_preset, coil_data_t * HandleCoilData)
{
	for (uint8_t count = 0; count < HandleCoilData->set_numb_winding; count++)
	{
		HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count]; //����� ���������� ������ �������
	}
	HandleCoilData->complet_winding = 0; //���������� ���������� ������� ����� 0
	turn_drive_menu (numb_preset, HandleCoilData, HandleCoilData->complet_winding); //����� ����
}

//-----------------------------------------------------------------------------------------------------//
int8_t start_drives_turn (uint8_t numb_preset, coil_data_t * HandleCoilData)
{
	uint8_t count = 0;
	if(HandleCoilData->complet_winding < HandleCoilData->set_numb_winding) //���� ���������� ���������� ������� ������ ������������ ���������� �������
	{
		count = HandleCoilData->complet_winding; //������� ����� �������
		init_drives_setting (HandleCoilData, count); //������ �������� ����������
		while (HandleCoilData->remains_coil[count] > 0) //���� ���������� ���������� ������ ������� ������ ����
		{
			if ((status_drives.end_turn_drive2 == DRIVE_FREE) && (status_drives.end_turn_drive1 == DRIVE_FREE)) 
				// && (status_drives.stop_drives == START)) //�������� ������ (��������� �������� � �� ���������)
			{
				HandleCoilData->remains_coil[count] = (Drives_PWM.turn_number-1); //���������� ���������� ���������� �������				
				status_drives.end_turn_drive2 = DRIVE_BUSY; //��������� 2 ����� (�������)
				status_drives.end_turn_drive1 = DRIVE_BUSY; //��������� 1 ����� (�������)
				drives_one_full_turn(); //������ �������� �� ���� ������ ������
				
				#ifdef __USE_DBG
					sprintf (DBG_buffer,  "turn_start, numb=%u\r\n", Drives_PWM.turn_number);
					DBG_PutString(DBG_buffer);
				#endif
				
				turn_drive_menu (numb_preset, HandleCoilData, count); //����� ����			
			}
			if	(scan_button_PEDAL() == OFF) //���� ������ ���� ��������
			{	
				//status_drives.stop_drives = STOP; //��������� ����� ��������� ����������	
				return (HandleCoilData->set_numb_winding - HandleCoilData->complet_winding); //������� ���������� ���������� �������			
			}
		}
		HandleCoilData->complet_winding++; //���������� �� 1 ���������� ���������� �������			
		while	((status_drives.end_turn_drive2 != DRIVE_FREE) && 
		(status_drives.end_turn_drive1 != DRIVE_FREE)) {} //�������� ���������� �������� ����������
		status_drives.stop_drives = STOP; //��������� ����� ��������� ����������		
		return (HandleCoilData->set_numb_winding - HandleCoilData->complet_winding);	
	}
	return -1;
}

//-----------------------------------------------------------------------------------------------------//
int8_t select_direction(void)
{
	__IO uint16_t key_code = NO_KEY;
	while ((key_code = scan_keys()) == NO_KEY) {} //�������� ������� ������
	switch (key_code) //��������� ���� ������� ������
	{	
			case KEY_MODE_SHORT:
			case KEY_MODE_LONG:
				status_drives.direction = LEFT;
				break;
			
			case KEY_NULL_SHORT:
			case KEY_NULL_LONG:
				status_drives.direction = RIGHT;
				break;
			
			default:
				return ERROR;
				break; 
	}
	return status_drives.direction;
}


//-----------------------------------------------------------------------------------------------------//
void setup_null_position (void)
{		
	while (scan_button_PEDAL() != ON) {} //�������� ������� ������
	while ((scan_keys()) == NO_KEY) //���� �� ������ ������� ������
	{
		if (status_drives.end_turn_drive1 == DRIVE_FREE) //�������� ����� ��������� 1
		{
			status_drives.end_turn_drive1 = DRIVE_BUSY; //������ ����� ��������� 1 - ����� 
			drive1_turn(); //������ �������� �� ���� ������ ������
			while (scan_button_PEDAL() == ON){} //�������� ���������� ������
			Drive1_PWM_stop(); //��������� ��������� ����
			DRIVE1_ENABLE(STOP); //������ ������� ��������� 1
			status_drives.end_turn_drive1 = DRIVE_FREE; //������ ���� ��������� 1 - ��������
		}
	}
}

//------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive1_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive1_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(STOP); //���������� ��������� 1
	status_drives.end_turn_drive1 = DRIVE_FREE; //��������� 1 �� �������
}

//------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive2_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive2_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE2_ENABLE(STOP);  //���������� ��������� 2
	status_drives.end_turn_drive2 = DRIVE_FREE; //��������� 2 �� �������
	if (Drives_PWM.turn_number > 0) //���� ���������� �������� �������� ��������� ������ ����
	{	Drives_PWM.turn_number--; } //���������� ���������� �������� �� �������
}

//-------------------------------------------LL_EXTI_LINE_12-------------------------------------------//
void Left_Sensor_Callback (void)
{
	DRIVE1_ENABLE(STOP);  //���������� ��������� 1
	status_drives.direction = (RIGHT); //����� ������ ������ ����������� ������� �������
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(START);  //��������� ��������� 1
}

//--------------------------------------------LL_EXTI_LINE_2--------------------------------------------//
void Right_Sensor_Callback (void)
{	
	DRIVE1_ENABLE(STOP); //���������� ��������� 1
	status_drives.direction = (LEFT); //������ ������ ������ ����������� ������� ������
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(START); //��������� ��������� 1
}
