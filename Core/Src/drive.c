
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
static void dr1_rotate_step (uint8_t );
static void drives_one_full_turn (void);
//static void PWM_turn_drive (void);

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)
PWM_data_t Drive2_PWM = {0};
uint8_t flag_turn_end = 1;
uint8_t direction = LEFT;
//----------------------------������� ���� ��������� 1 �� ���� ��������----------------------------//
static void dr1_rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //���������� ���������� (���������)
	{
		//	STEP_DRIVE1(ON);
			delay_us (3); //3 ���
	//		STEP_DRIVE1(OFF);
			delay_us (3); //3 ���
	}
}

//------------------------------------------------------------------------------------------------//
void dr1_one_full_turn (void) 
{
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (direction); //����������� ��������
	delay_us (6);	
	
	for (uint16_t count = 0; count < STEP_IN_TURN; count++)
	{
		dr1_rotate_step (STEP_DIV);
		delay_us (750);
	}
	DRIVE1_ENABLE(OFF);
}

//--------------------------------------------------------------------------------------------------------------//
void init_drive_turn (coil_data_t * HandleCoilData)
{
	Drive2_PWM.turn_number = (HandleCoilData->set_coil[0]); //���������� ���������� �������
	while (HandleCoilData->remains_coil[0] > 0) //���� ���������� ���������� ������ ������� ������ ����
	{
		if (flag_turn_end == 1) //���� ��������� �� �������
		{
			HandleCoilData->remains_coil[0] = (Drive2_PWM.turn_number-1); //���������� ���������� ���������� �������
			flag_turn_end = 0; //��������� �������
			drives_one_full_turn(); //������ �������� �� ���� ������ ������
			turn_drive_menu (HandleCoilData); //����� ����
		}
	}	
}


//--------------------------������������� � ������ �������� ������� ������� ���������--------------------------//
static void drives_one_full_turn (void) 
{
	DRIVE2_ENABLE(ON);
	DIR_DRIVE2 (BACKWARD); //����������� �������� ��������� 2
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (direction); //����������� �������� ��������� 1
	delay_us (5);	
	
	Drive2_PWM.Compare_Drive1 = 249;
	Drive2_PWM.Compare_Drive2 = 499;
	Drive2_PWM.value_Period = 999;
	Drive2_PWM.count_start_PWM_TIM = PULSE_IN_TURN/PULSE_IN_TIM_RCR;
	
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "full_turn_start\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	PWM_start (&Drive2_PWM);
}

//---------------------------------------------------------------------------------------------------------------//
void TIM_counter_PWM_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "full_turn_end\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(OFF);
	DRIVE2_ENABLE(OFF);
	flag_turn_end = 1; 				//��������� �� �������
	if (Drive2_PWM.turn_number > 0)
	{	Drive2_PWM.turn_number--; }
}

//------------------------------------------------LL_EXTI_LINE_12------------------------------------------------//
void Left_Sensor_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "Left_sensor\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(OFF);
	direction = (RIGHT);
	DIR_DRIVE1 (direction);
	DRIVE1_ENABLE(ON);
}

//-------------------------------------------------LL_EXTI_LINE_2-------------------------------------------------//
void Right_Sensor_Callback (void)
{
	#ifdef __USE_DBG
		sprintf (DBG_buffer,  "Right_sensor\r\n");
		DBG_PutString(DBG_buffer);
	#endif
	
	DRIVE1_ENABLE(OFF);
	direction = (LEFT);
	DIR_DRIVE1 (direction);
	DRIVE1_ENABLE(ON);
}
