
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"
#include "usart.h"

// Functions --------------------------------------------------------------------------------------//
static uint32_t Get_Encoder_data (void) ;
// Variables --------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
static uint32_t Get_Encoder_data (void) 
{
	return (LL_TIM_GetCounter(TIM_ENC));
}

//------------------------------------------------------------------------------------------------//
uint8_t read_encoder (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //������� ��������� ��������
	int32_t currDelta = 0; //������� ����� ���������� ���������� �������� � ����������
	
	currCounter = Get_Encoder_data (); //��������� �������� ��������� ��������
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //���� ��������� �������� ����������
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick);
		HandleEncData->delta = currDelta; 
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //���������� �������� ��������� ��������
		return ON;
	}
	return OFF; 
}

//------------------------------------����� ��������� ��������------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //���������� ��� �������� ������ ��������
	currCounter = LL_TIM_GetCounter(TIM_ENC);  //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->delta = 0; //��������� �� �������� ����������
}

//---------------------------------------------------------------------------------------------------------------//
