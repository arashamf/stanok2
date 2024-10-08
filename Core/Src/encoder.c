
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"

// Functions --------------------------------------------------------------------------------------//

// Variables --------------------------------------------------------------------------------------//

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
