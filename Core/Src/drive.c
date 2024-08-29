
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "menu.h"
#include "typedef.h"
#include "button.h"
#include "eeprom.h"

// Functions --------------------------------------------------------------------------------------//
void rotate_step (uint8_t );
static uint8_t step = 0;

// Variables --------------------------------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)

//----------------------------------������� ���� �� ���� ��������----------------------------------//
void rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //���������� ���������� (���������)
	{
			STEP_DRIVE1(ON);
			delay_us (3); //3 ���
			STEP_DRIVE1(OFF);
			delay_us (3); //3 ���
	}
}

//------------------------------------------------------------------------------------------------//
void milling_step (uint8_t dir)
{
	uint32_t quant = 	step*STEP_DIV;; 
	
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (dir); //����������� ��������
	delay_us (6);	
	
	rotate_step (quant);
	DRIVE1_ENABLE(OFF);
}

//------------------------------------------------------------------------------------------------//
void one_full_turn (void) 
{
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (FORWARD); //����������� ��������
	delay_us (6);	
	
	for (uint16_t count = 0; count < STEP_IN_TURN; count++)
	{
		rotate_step (STEP_DIV);
		delay_us (750);
	}
	DRIVE1_ENABLE(OFF);
}

//------------------------------------------------------------------------------------------------//
void turn_coil (coil_data_t * HandleCoilData) 
{
	uint16_t turn_number = HandleCoilData->remains_coil[0];
	for(uint16_t count = 0; count < turn_number; count++)
	{
		one_full_turn();
		HandleCoilData->remains_coil[0]--;
		main_menu (HandleCoilData);
		delay_us (10000);
	}
}

//------------------------------------------------------------------------------------------------//
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //������� ��������� ��������
	int32_t currDelta = 0; //������� ����� ���������� ���������� �������� � ����������
	//int32_t changeAng = 0; //��������� ���� ������� (������ �������� * ���� ���� ����)
	
	currCounter = LL_TIM_GetCounter(TIM3); //��������� �������� ��������� ��������
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //���� ��������� �������� ����������
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick); //������� ����� ���������� � ���������� ���������� ��������
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //���������� �������� ��������� ��������    	
		if (currDelta != 0) //���� ���������� ��������� ��������
		{		
			HandleEncData->delta += currDelta; //���������� ��������� ��������� �������� (���-�� ������� ��������)
			if (currDelta > 0) //���� ���������� �� ������� �������
			{	
				step = currDelta;
			}
			else
			{
				if (currDelta < 0) //���� ������� ���������� ������ ������� �������
				{	
					step = (-1)*currDelta;
				}
			}
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
