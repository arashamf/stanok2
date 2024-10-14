
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

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)
PWM_data_t Drives_PWM = {0};
volatile STATUS_FLAG_DRIVE_t status_drives; 

//--------------------------------------------------------------------------------------------------------------//
void init_status_flags_drives (void) 
{
	status_drives.direction = LEFT; //��������� �� �������, ����� ������� �����
	status_drives.end_turn_drive1 = DRIVE_FREE;
	status_drives.end_turn_drive2 = DRIVE_FREE;
}

//-------------------------------------------------------------------------------------------------------------//
static void init_drives_setting (coil_data_t * HandleCoilData, uint8_t number)
{
	uint32_t period_drive2 = (BASE_PERIOD_DR2*100)/HandleCoilData->rotation_speed;
	uint32_t period_drive1 =(period_drive2*100)/HandleCoilData->gear_ratio;
	Drives_PWM.Compare_Drive1		= 	(period_drive1/2)-1; 	//������������ �������� ������� (�����������) ���������
	Drives_PWM.Period_Drive1 		= 	period_drive1-1; 	//������ �������� ������� (�����������) ���������
	Drives_PWM.Compare_Drive2 	= 	(period_drive2/2) - 1; //������������ �������� �������� (���������) ���������
	Drives_PWM.Period_Drive2 		=		period_drive2-1; 	//������ �������� �������� (���������) ���������
	Drives_PWM.turn_number = (HandleCoilData->set_coil[number]); //���������� ���������� �������
	Drives_PWM.number_cnt_PWM_DR1 = (PULSE_IN_TURN*HandleCoilData->gear_ratio)/100;
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "ratio=%u,Period=%u,Compare=%u\r\n", HandleCoilData->gear_ratio, Drives_PWM.Period_Drive1, Drives_PWM.Compare_Drive1);
		DBG_PutString(DBG_buffer);
	#endif
}

//--------------------------������������� � ������ �������� ������� ������� ���������--------------------------//
static void drives_one_full_turn (void) 
{
	DRIVE2_ENABLE(ON);
	DIR_DRIVE2 (BACKWARD); //����������� �������� ��������� 2
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (status_drives.direction); //����������� �������� ��������� 1
	delay_us (3);	
	
	Drives_PWM_start (&Drives_PWM);
}

//--------------------------------------------------------------------------------------------------------------//
void start_drives_turn (coil_data_t * HandleCoilData)
{
	
	for (uint8_t count = 0; count < HandleCoilData->number_coil; count++)
	{
		init_drives_setting (HandleCoilData, count);
		while (HandleCoilData->remains_coil[count] > 0) //���� ���������� ���������� ������ ������� ������ ����
		{
			if ((status_drives.end_turn_drive2 == DRIVE_FREE) && (status_drives.end_turn_drive1 == DRIVE_FREE)) //���� ��������� 2 �� �������
			{
				HandleCoilData->remains_coil[count] = (Drives_PWM.turn_number-1); //���������� ���������� ���������� �������				
				status_drives.end_turn_drive2 = DRIVE_BUSY; //��������� �������
				status_drives.end_turn_drive1 = DRIVE_BUSY;
				drives_one_full_turn(); //������ �������� �� ���� ������ ������
				
				#ifdef __USE_DBG
					sprintf (DBG_buffer,  "turn_start, numb=%u\r\n", Drives_PWM.turn_number);
					DBG_PutString(DBG_buffer);
				#endif
				
				turn_drive_menu (HandleCoilData, count); //����� ����
			}
		}	
		while	((status_drives.end_turn_drive2 != DRIVE_FREE) && (status_drives.end_turn_drive1 != DRIVE_FREE)) {}				
	}
}

//---------------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive1_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive1_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(OFF); //���������� ��������� 1
	status_drives.end_turn_drive1 = DRIVE_FREE; //��������� 1 �� �������
}

//---------------------------------------------------------------------------------------------------------------//
void Counter_PWM_Drive2_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "drive2_full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE2_ENABLE(OFF);  //���������� ��������� 2
	status_drives.end_turn_drive2 = DRIVE_FREE; //��������� 2 �� �������
	if (Drives_PWM.turn_number > 0)
	{	Drives_PWM.turn_number--; }
}

//------------------------------------------------LL_EXTI_LINE_12------------------------------------------------//
void Left_Sensor_Callback (void)
{
	DRIVE1_ENABLE(OFF);
	status_drives.direction = (RIGHT);
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(ON);
}

//-------------------------------------------------LL_EXTI_LINE_2-------------------------------------------------//
void Right_Sensor_Callback (void)
{	
	DRIVE1_ENABLE(OFF);
	status_drives.direction = (LEFT);
	DIR_DRIVE1 (status_drives.direction);
	DRIVE1_ENABLE(ON);
}
