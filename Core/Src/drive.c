
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
static void dr2_one_full_turn (void);
static void PWM_turn_drive (void);

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)
PWM_data_t Drive2_PWM = {0};
uint8_t flag_turn_end = 1;

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
	DIR_DRIVE1 (FORWARD); //����������� ��������
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
	Drive2_PWM.turn_number = HandleCoilData->remains_coil[0]; //���������� ���������� ������ ������ �������
	while (HandleCoilData->remains_coil[0] > 0) //���� ���������� ���������� ������ ������� ������ ����
	{
		if (flag_turn_end == 1) //��������� ��������
		{
			HandleCoilData->remains_coil[0] = Drive2_PWM.turn_number;
			flag_turn_end = 0; //��������� �������
			turn_drive_menu (HandleCoilData);
			dr2_one_full_turn();
		}
		else
		{
			if (flag_turn_end == 0) 
			{}
		}
	}
}


//--------------------------������������� � ������ �������� ������� ������� ���������--------------------------//
static void dr2_one_full_turn (void) 
{
	DRIVE2_ENABLE(ON);
	DIR_DRIVE2 (BACKWARD); //����������� ��������
	delay_us (6);	
	
	Drive2_PWM.value_Compare = 499;
	Drive2_PWM.value_Period = 999;
	Drive2_PWM.microstep = PULSE_IN_TIM_RCR-2;
	Drive2_PWM.count_start_PWM_TIM = PULSE_IN_TURN/PULSE_IN_TIM_RCR;
	
	PWM_on (&Drive2_PWM);
}

//------------------------------------------------------------------------------------------------//
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //������� ��������� ��������
	int32_t currDelta = 0; //������� ����� ���������� ���������� �������� � ����������
	//int32_t changeAng = 0; //��������� ���� ������� (������ �������� * ���� ���� ����)
	
	currCounter = Get_Encoder1_data (); //��������� �������� ��������� ��������
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //���� ��������� �������� ����������
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick); //������� ����� ���������� � ���������� ���������� ��������
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //���������� �������� ��������� ��������    	
		if (currDelta != 0) //���� ���������� ��������� ��������
		{		
			HandleEncData->delta += currDelta; //���������� ��������� ��������� �������� (���-�� ������� ��������)
		/*	if (currDelta > 0) //���� ���������� �� ������� �������
			{	
				step = currDelta;
			}
			else
			{
				if (currDelta < 0) //���� ������� ���������� ������ ������� �������
				{	
					step = (-1)*currDelta;
				}
			}*/
			return ON;
		}
	}
	return OFF; 
}

//------------------------------------����� ��������� ��������------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //���������� ��� �������� ������ ��������
	encCounter = LL_TIM_GetCounter(TIM3); //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetClick = (32767 - ((encCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->delta = 0; //��������� �� �������� ����������
}

//---------------------------------------------------------------------------------------------------------------//
void TIM_PWM1_Callback(void)
{
	PWM_turn_drive ();
}

//----------------------------------����������� �������� ��������� � ����������----------------------------------//
static void PWM_turn_drive (void) 
{
	LL_TIM_DisableAllOutputs(TIM1);
	if (Drive2_PWM.count_start_PWM_TIM > 0) //���� ���������� ���������� �������� ������� ��������� ������ ����
	{
		delay_us (5000);	
		PWM_on (&Drive2_PWM);
		Drive2_PWM.count_start_PWM_TIM--;		
	}
	else
	{
		Drive2_PWM.turn_number--; //���������� ���������� ���������� ������ �������
		LL_TIM_ClearFlag_UPDATE(TIM1); //����� ����� ���������� �������
		DRIVE2_ENABLE(OFF);
		flag_turn_end = 1; //��������� ��������
	}
}

//---------------------------------------------------------------------------------------------------------------//
