
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"
#include "button.h"
//#include "ssd1306.h"
#include "eeprom.h"
#include "angle_calc.h"

// Functions -----------------------------------------------------------------//
void rotate_step (uint8_t );

// Variables -----------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //���������� ������ � ����� ���������(1,8��/100=6480/100=64,8)

//---------------------------------------������� ���� �� ���� ��������---------------------------------------//
void rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //���������� ���������� (���������)
	{
		/*	STEP(ON);
			delay_us (4); //4 ���
			STEP(OFF);
			delay_us (4); //4 ���*/
	}
}

//-------------------------------------------------------------------------------------------------------------//
void milling_step (uint8_t dir, uint32_t need_step)
{
	uint32_t quant = 0; //���������� ����� ����� (1,8 ��.)
	__IO uint16_t key_code = NO_KEY;
	
	DIR1_DRIVE (dir); //����������� ��������
	delay_us (6);	
	
	if ((quant = (uint32_t)need_step/STEP_DIV) > 0) //���������� ����� (1,8 ��.)
	{
		for (uint32_t count = 0; count < quant; count++) //������ ����� ����� (1,8 ��.)
		{
			DRIVE1_ENABLE(ON);
			delay_us (5);
			rotate_step (STEP_DIV); //������� �� ���� ��� (1,8 ��., 100 ����������)
			DRIVE1_ENABLE(OFF);
			delay_us (1500); //�������� 1500 ���
		}
	}		
	if ((quant = need_step%STEP_DIV) > 0) //������� - ��������� (<1,8 ��)
	{
		DRIVE1_ENABLE(ON);
		delay_us (5);
		rotate_step (quant);
		DRIVE1_ENABLE(OFF);
	}
}
//--------------------------------------������� ���� �� ������������ ����--------------------------------------//
uint32_t step_angle (uint8_t dir, uint32_t need_step,	angular_data_t * HandleAng, encoder_data_t * HandleEncData)
{
	uint32_t receive_step = 0; //���������� ���������� ���������� ���������� (���������)
	uint32_t quant = 0; //���������� ����� ����� (1,8 ��.)
	__IO uint16_t key_code = NO_KEY;
	uint32_t PassedAngleInSec = 0; //���������� ���������� ���� � �������� 
	
	DIR1_DRIVE (dir); //����������� ��������
	delay_us (6);	
	
	if ((quant = (uint32_t)need_step/STEP_DIV) > 0) //���������� ����� (1,8 ��.)
	{
		for (uint32_t count = 0; count < quant; count++) //������ ����� ����� (1,8 ��.)
		{
			DRIVE1_ENABLE(ON);
			delay_us (5);
			rotate_step (STEP_DIV); //������� �� ���� ��� (1,8 ��., 100 ����������)
			DRIVE1_ENABLE(OFF);
			receive_step += STEP_DIV; //���������� ����������� ���� �� 1 ����� ���
			if ((key_code = scan_keys()) == KEY_MODE_SHORT) //���� ��������� �������� ������� ������ ������ ������
			{	
				encoder_reset (HandleEncData); //����� ��������� ��������
				HandleAng->SetShaftAngleInSec = HandleAng->ShaftAngleInSec; //����� ������� ���� ����
				return receive_step; //��������� �������� � ������� ���������� ����������� ����	
			}		
			if (((count % 100) == 0) && (count != 0)) //������ 100 ����� ����� ��������
			{	
				if (read_enc_shaft_rotation (HandleAng, HandleEncData) == ON) //���� ��������� �������� ����������
				{ continue;	} //������� �����
			}
			delay_us (1499); 			
		}
	}		
	if ((quant = need_step%STEP_DIV) > 0) //������� - ��������� (<1,8 ��)
	{
		DRIVE1_ENABLE(ON);
		delay_us (5);
		rotate_step (quant);
		DRIVE1_ENABLE(OFF);
		receive_step += quant;
	}
	return receive_step;
}

//-----------------------------------------------------------------------------------------------------//
uint8_t read_enc_shaft_rotation (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //������� ��������� ��������
	int32_t currDelta = 0; //������� ����� ���������� ���������� �������� � ����������
	int32_t changeAng = 0; //��������� ���� ������� (������ �������� * ���� ���� ����)
	
	currCounter= LL_TIM_GetCounter(TIM3); //��������� �������� ��������� ��������
	HandleEncData->currCounter_ShaftRotation= (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_ShaftRotation != HandleEncData->prevCounter_ShaftRotation) //���� ��������� �������� ����������
	{
		currDelta = (HandleEncData->currCounter_ShaftRotation - HandleEncData->prevCounter_ShaftRotation); //������� ����� ���������� � ���������� ���������� ��������
    HandleEncData->prevCounter_ShaftRotation = HandleEncData->currCounter_ShaftRotation; //���������� �������� ��������� ��������    
		
		if (currDelta != 0) //���� ���������� ��������� ��������
		{		
			HandleEncData->delta += currDelta; //���������� ��������� ��������� �������� (���-�� ������� ��������)
			if (currDelta > 0) //���� ���������� �� ������� �������
			{
				changeAng = HandleAng->StepAngleInSec * currDelta; //��������� ��������� ���� ������� ����
				HandleAng->SetShaftAngleInSec = HandleAng->SetShaftAngleInSec + changeAng; //������ ���� ������� ����
				if (HandleAng->SetShaftAngleInSec > (CIRCLE_IN_SEC-1)) //���� ���� ��������� ���� 360 ��. ��� ������
				{	HandleAng->SetShaftAngleInSec = HandleAng->SetShaftAngleInSec - CIRCLE_IN_SEC;	} 
			}
			else
			{
				if (currDelta < 0) //���� ������� ���������� ������ ������� �������
				{
					changeAng = -1	* (HandleAng->StepAngleInSec) * currDelta; //��������� ��������� ���� ������� ����
					
					if (HandleAng->SetShaftAngleInSec >= changeAng) //���� ������� ��������� ���� ������ ��� ����� ���� �������
					{	HandleAng->SetShaftAngleInSec = HandleAng->SetShaftAngleInSec - changeAng; }
					else 
					{
						if (HandleAng->SetShaftAngleInSec < changeAng) //���� ������� ��������� ���� ������ ���� �������
						{	HandleAng->SetShaftAngleInSec = (CIRCLE_IN_SEC - (changeAng - HandleAng->SetShaftAngleInSec)); }			 
					}
				}
			}
			GetSetShaftAngle_from_Seconds (HandleAng); //������� ���� ������� � ������ ��./���./� ��� ����������� �� �������
		//	default_screen_mode1 (HandleAng, &Font_11x18); //������� ���� ������ 1
			return ON;
		}
	}
	return OFF; 
}

//--------------------------------------����� ��������� ��������--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //���������� ��� �������� ������ ��������
	encCounter = LL_TIM_GetCounter(TIM3); //���������� �������� ��������� ��������
	HandleEncData->currCounter_SetAngle = (32767 - ((encCounter-1) & 0xFFFF))/2; //�������������� ����������� ��������� �������� � ������ �� -10 �� 10
	HandleEncData->prevCounter_SetAngle = HandleEncData->currCounter_SetAngle; //���������� ���������������� �������� ��������� �������� � ��������� ��������� ���� ��������	
	HandleEncData->prevCounter_ShaftRotation = HandleEncData->currCounter_SetAngle; //���������� ���������������� �������� ��������� �������� � ��������� ������ ��������� ����
	HandleEncData->delta = 0; //��������� �� �������� ����������
}

//----------------------------------------------------------------------------------------------------//
void step_by_step (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0;  //���������� ����������(���������), ������� ���������� ������
	uint8_t dir; //����������� �������� ����
	uint32_t PassedAngleInSec = 0; //���������� ���������� ���� � �������� 
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	
	if (HandleEncData->delta != 0) //���� ���������� ��������� ��������
	{
		if (HandleEncData->delta > 0) //���� ������� ���������� �� ������� �������
		{
		//	need_step = calc_steps_mode1 (HandleAng, step_unit); //������� ���� �������� ���� � ���������� ���������� (���������) 
			receive_step += step_angle (FORWARD, need_step, HandleAng, HandleEncData); //������� �� ������� �������				
			if (HandleEncData->delta != 0) //���� ������ �� ����� ����
			{	HandleEncData->delta--; }	//���������� ������
			
		//	PassedAngleInSec = calc_passed_angle (receive_step, step_unit); //������ ������������ �������� ����
			HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec +	PassedAngleInSec; //������ �������� ��������� ����
			if (HandleAng->ShaftAngleInSec > (CIRCLE_IN_SEC-1)) //���� ���� ��������� ���� 360 ��. ��� ������
			{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - CIRCLE_IN_SEC;	} 							
		}
		else
		{
			if (HandleEncData->delta < 0) //���� ������� ���������� ������ ������� �������
			{	
			//	need_step = calc_steps_mode1 (HandleAng, step_unit); //������ ���������� ����������� ����������
				receive_step += step_angle (BACKWARD, need_step, HandleAng, HandleEncData); //������� ������ ������� �������
				if (HandleEncData->delta != 0)	//���� ������ �� ����� ����
				{	HandleEncData->delta++; }		//���������� ������
			//	PassedAngleInSec = calc_passed_angle (receive_step, step_unit);
				
				if (HandleAng->ShaftAngleInSec >= PassedAngleInSec) //���� ���� ��������� ���� ������ ���������� ���� 
				{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - PassedAngleInSec; }
				else 
				{
					if (HandleAng->ShaftAngleInSec < PassedAngleInSec) //���� ���� ��������� ���� ������ ���� ����
					{	HandleAng->ShaftAngleInSec = (CIRCLE_IN_SEC - (PassedAngleInSec - HandleAng->ShaftAngleInSec)); }	
				}						 
			}
		}
		if (HandleEncData->delta == 0) //���� ��������� ��������� �������� ��������� �������� �����
		{
			GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
		//	default_screen_mode1 (HandleAng, &Font_16x26); //������� ���� ������ 1
		}
		angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
		EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, (EEPROM_NUMBER_BYTES-6)); //������ ������� � ������� ���� �������� �
	}
	else
	{	HandleAng->SetShaftAngleInSec = HandleAng->ShaftAngleInSec;	}  //������� ���� ����� �������� ��������� ����
}

//---------------------------��������� ��������� �������� � ������ ����������� ����----------------------------//
void enc_shaft_rotation (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	int32_t currCounter=0; //������� ��������� ��������
	uint32_t PassedAngleInSec = 0;
	
	currCounter= LL_TIM_GetCounter(TIM3); //������� ��������� ��������
	HandleEncData->currCounter_ShaftRotation= (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_ShaftRotation != HandleEncData->prevCounter_ShaftRotation) //���� ��������� �������� ����������
	{
		HandleEncData->delta = (HandleEncData->currCounter_ShaftRotation - HandleEncData->prevCounter_ShaftRotation); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_ShaftRotation = HandleEncData->currCounter_ShaftRotation; //���������� �������� ��������� ��������    
		if((HandleEncData->delta > -20) && (HandleEncData->delta < 20)) // ������ �� �������� ��������� � ������������ �������� 
		{
			if (HandleEncData->delta != 0) //���� ���������� ��������� ��������
			{ 
				SetAngle_in_Seconds (HandleAng); //������� ������� ������ ���� � �������		
				if (HandleEncData->delta > 0) //���� ������� ���������� �� ������� �������
				{
					while (HandleEncData->delta > 0) //���� ������ ������ ����
					{
				//		need_step = calc_steps_mode1 (HandleAng, step_unit);
						receive_step += step_angle (FORWARD, need_step, HandleAng, HandleEncData); //������� �� ������� �������				
						
						if (HandleEncData->delta != 0) //���� ������ �� ����� ����
						{	HandleEncData->delta--; } 		//���������� ������
					//	PassedAngleInSec = calc_passed_angle (receive_step, step_unit);
						
						HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec +	PassedAngleInSec;
						if (HandleAng->ShaftAngleInSec > (CIRCLE_IN_SEC-1)) //���� ���� ��������� ���� 360 ��. ��� ������
						{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - CIRCLE_IN_SEC;	} 										
					}
				}
				else
				{
					if (HandleEncData->delta < 0) //���� ������� ���������� ������ ������� �������
					{	
						while (HandleEncData->delta < 0) //���� ������ ������ ����
						{
					//		need_step = calc_steps_mode1 (HandleAng, step_unit); //������ ���������� ����������� ����������
							receive_step += step_angle (BACKWARD, need_step, HandleAng, HandleEncData); //������� ������ ������� �������
							
							if (HandleEncData->delta != 0)	//���� ������ �� ����� ����
							{	HandleEncData->delta++; }		//���������� ������
						//	PassedAngleInSec = calc_passed_angle (receive_step, step_unit);
							
							if (HandleAng->ShaftAngleInSec >= PassedAngleInSec) //���� ���� ��������� ���� ������ ���������� ���� 
							{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - PassedAngleInSec; }
							else 
							{
								if (HandleAng->ShaftAngleInSec < PassedAngleInSec) //���� ���� ��������� ���� ������ ���� ����
								{	HandleAng->ShaftAngleInSec = (CIRCLE_IN_SEC - (PassedAngleInSec - HandleAng->ShaftAngleInSec)); }	//������� � ������ 359��. �����.
							}						
						}
					}
				}
				GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
			//	default_screen_mode1 (HandleAng, &Font_11x18); //������� ���� ������ 1 	
				angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
				EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, (EEPROM_NUMBER_BYTES-6)); //������ ������� � ������� ���� �������� � ������					
			}
			else
			{	HandleEncData->delta = 0;	}
		}
	}
}

//------------------------��������� ��������� �������� � ������ ��������� ���� ���� ����------------------------//
void set_angle (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	int32_t currCounter=0;
	int32_t delta = 0;
	currCounter = LL_TIM_GetCounter(TIM3); //������� ��������� ��������
	HandleEncData->currCounter_SetAngle = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetAngle != HandleEncData->prevCounter_SetAngle) //���� ������� �������� �������� �� ����� �����������
	{
		delta = (HandleEncData->currCounter_SetAngle - HandleEncData->prevCounter_SetAngle); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_SetAngle = HandleEncData->currCounter_SetAngle; //���������� �������� ���������� ��������    
    if((delta > -20) && (delta < 20)) // ������ �� �������� ��������� � ������������ �������� (������������ ����� ��������� ����� �����)
		{
			if (delta != 0) //���� ���������� ��������� ��������
			{  
				HandleAng->StepAngleInSec += (delta*SECOND_PER_MINUTE); //����������� ������
				if (HandleAng->StepAngleInSec == 0) 							//���� ���� ���������� ������ 0
				{ HandleAng->StepAngleInSec = (CIRCLE_IN_SEC); } 	//���� ����� 360 ��
				else
				{
					if (HandleAng->StepAngleInSec > (CIRCLE_IN_SEC)) //���� ���� ���������� ������ ������ 359 �� 59 �����
					{ HandleAng->StepAngleInSec = SECOND_PER_MINUTE; } //���� ����� 1 ������
				}
				GetSetAngle_from_Seconds (HandleAng); //������� ���� ���� ���� ���� �� ������ � ������ ��/���/�
			//	setangle_mode_screen (HandleAng, &Font_11x18); //����� ���������� �� �������
			}
		}
		else
		{	delta = 0; }
	}				
}

//----------------------��������� ��������� �������� � ������ �������� ���������� ������----------------------//
void set_teeth_gear (milling_data_t * HandleMil, encoder_data_t * HandleEncData) 
{
	int32_t currCounter=0;

	currCounter = LL_TIM_GetCounter(TIM3); //������� ��������� ��������
	HandleEncData->currCounter_SetAngle = (32767 - ((currCounter-1) & 0xFFFF))/2; //������� �� 2, ���� ������� (������ = 2 ��������)
	
	if(HandleEncData->currCounter_SetAngle != HandleEncData->prevCounter_SetAngle) //���� ������� �������� �������� �� ����� �����������
	{
		HandleEncData->delta = (HandleEncData->currCounter_SetAngle - HandleEncData->prevCounter_SetAngle); //������� ����� ������� � ���������� ���������� ��������
    HandleEncData->prevCounter_SetAngle = HandleEncData->currCounter_SetAngle; //���������� �������� ���������� ��������    
    if((HandleEncData->delta > -20) && (HandleEncData->delta < 20)) // ������ �� �������� ��������� � ������������ �������� (������������ ����� ��������� ����� �����)
		{
			if (HandleEncData->delta != 0) //���� ���������� ��������� ��������
			{  
				HandleMil->teeth_gear_numbers += HandleEncData->delta;
				if (HandleMil->teeth_gear_numbers > 0xFE) //���������� ������ �� ������ 254
				{	HandleMil->teeth_gear_numbers = 0x02;	}	//����� �� ����������� ��������
				else
				{
					if (HandleMil->teeth_gear_numbers < 0x02) //� �� ������ 2
					{	HandleMil->teeth_gear_numbers = 0xFE;	} //����� �� ������������ ��������
				}	
			//	setteeth_mode_screen (HandleMil, &Font_11x18);
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------//
void right_shaft_rotation (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	SetAngle_in_Seconds (HandleAng); //������� ������� ������ ���� � �������
	//need_step = calc_steps_mode1 (HandleAng, step_unit);
	step_angle (FORWARD, need_step, HandleAng, HandleEncData); //������� �� ������� �������
	
	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec + HandleAng->StepAngleInSec;
	if (HandleAng->ShaftAngleInSec > (CIRCLE_IN_SEC-1)) //���� ���� ��������� ���� 360 ��. ��� ������
	{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - CIRCLE_IN_SEC;	} 
	
	GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
	angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
	EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, 8); //������ ������� � ������� ���� �������� � ������					
}

//---------------------------------------------------------------------------------------------------//
void left_shaft_rotation (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	SetAngle_in_Seconds (HandleAng); //������� ������� ������ ���� � �������
	//need_step = calc_steps_mode1 (HandleAng, step_unit);
	step_angle (BACKWARD, need_step, HandleAng, HandleEncData); //������� ������ ������� �������
	
	if (HandleAng->ShaftAngleInSec >= HandleAng->StepAngleInSec) //���� ���� ��������� ���� ������ ���� ����
	{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - HandleAng->StepAngleInSec; }
	else 
	{
		if (HandleAng->ShaftAngleInSec < HandleAng->StepAngleInSec) //���� ���� ��������� ���� ������ ���� ����
		{	HandleAng->ShaftAngleInSec = CIRCLE_IN_SEC - (HandleAng->StepAngleInSec - HandleAng->ShaftAngleInSec); }	//������� � ������ 359��. �����.
	}
	GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
	angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
	EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, 8); //������ ������� � ������� ���� �������� � ������					
}

//---------------------------------------------------------------------------------------------------//
void one_full_turn (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	step_angle (FORWARD, (STEPS_IN_REV*REDUCER), HandleAng, HandleEncData);  //������ ������ �� 360�� � ������ �������� ��������� (360��*40)
}

//----------------------------������� ���� � ������� ������� ������ ������� �������----------------------------//
void left_rotate_to_zero (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	uint32_t PassedAngleInSec = 0;
	
	//need_step = steps_for_back_to_zero (HandleAng->ShaftAngleInSec, step_unit);
	receive_step = step_angle (BACKWARD, need_step, HandleAng, HandleEncData); //������� ������ ������� �������
	//PassedAngleInSec = calc_passed_angle (receive_step, step_unit);
	
	HandleAng->ShaftAngleInSec = 0; //������� ��������� ���� �������
	//HandleAng->ShaftAngleInSec -= PassedAngleInSec;
	GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
	angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
	EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, 8); //������ ������� � ������� ���� �������� � ������
}

//-----------------------------������� ���� � ������� ������� �� ������� �������-----------------------------//
void right_rotate_to_zero (angular_data_t * HandleAng, encoder_data_t * HandleEncData) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	uint32_t PassedAngleInSec = 0;
	
	//need_step = steps_for_back_to_zero ((CIRCLE_IN_SEC - HandleAng->ShaftAngleInSec), step_unit);
	receive_step = step_angle (FORWARD, need_step, HandleAng, HandleEncData); //������� �� ������� �������
	//PassedAngleInSec = calc_passed_angle (receive_step, step_unit);
	
	HandleAng->ShaftAngleInSec = 0; //������� ��������� ���� �������
	//HandleAng->ShaftAngleInSec += PassedAngleInSec;
	GetAngleShaft_from_Seconds(HandleAng); //����������� �������� ���� ���� � �������� � ������ ��/���/���
	angle_to_EEPROMbuf (HandleAng, eeprom_tx_buffer); //������� ������ ���� � ������
	EEPROM_WriteBytes (EEPROM_MEMORY_PAGE, eeprom_tx_buffer, 8); //������ ������� � ������� ���� �������� � ������
}

//---------------------------------������� ���� �� ���� ��� �� ������� �������---------------------------------//
void right_teeth_rotation (milling_data_t * HandleMil, angular_data_t * HandleAng) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	
	if (HandleMil->remain_teeth_gear > 0)
	{
		HandleMil->remain_teeth_gear--; //���������� �� 1 ����������� ���������� ������
		//need_step = calc_steps_milling (HandleMil, step_unit); //������� �� ���� ���� ���� ������ ������� ������� � ������ ���������
		milling_step (FORWARD, need_step); //������� �� ������� �������
		
		HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec + HandleMil->AngleTeethInSec; //����������� ���� �������� ��������� ����
		if (HandleAng->ShaftAngleInSec > (CIRCLE_IN_SEC-1)) //���� ���� ��������� ���� 360 ��. ��� ������
		{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - CIRCLE_IN_SEC;	} //
	}
}

//-------------------------------������� ���� �� ���� ��� ������ ������� �������-------------------------------//
void left_teeth_rotation (milling_data_t * HandleMil, angular_data_t * HandleAng) 
{
	uint32_t need_step = 0; //���������� ����������� ����������(���������)
	uint32_t receive_step = 0; //���������� ���������� ���������� ����������(���������)
	
	if (HandleMil->remain_teeth_gear < HandleMil->teeth_gear_numbers)
	{
		HandleMil->remain_teeth_gear++; //���������� �� 1 ����������� ���������� ������
	//	need_step = calc_steps_milling (HandleMil, step_unit); //������� �� ���� ���� ���� ������ ������� ������� � ������ ���������
		milling_step (BACKWARD, need_step); //������� ������ ������� �������
		
		if (HandleAng->ShaftAngleInSec >= HandleMil->AngleTeethInSec) //���� ���� ��������� ���� ������ ���� ����
		{	HandleAng->ShaftAngleInSec = HandleAng->ShaftAngleInSec - HandleMil->AngleTeethInSec; } //��������� ���� �������� ��������� ����
		else 
		{
			if (HandleAng->ShaftAngleInSec < HandleMil->AngleTeethInSec) //���� ���� ��������� ���� ������ ���� ����
			{	HandleAng->ShaftAngleInSec = CIRCLE_IN_SEC - (HandleMil->AngleTeethInSec - HandleAng->ShaftAngleInSec); }	//������� � ������ 359��. �����.
		}
	}
}

//---------------------------------------------------------------------------------------------------//
void teeth_counter_increment (milling_data_t * HandleMil)
{
	if (HandleMil->remain_teeth_gear < HandleMil->teeth_gear_numbers)
	{
		HandleMil->remain_teeth_gear++;
	}
}

//---------------------------------------------------------------------------------------------------//
void teeth_counter_decrement (milling_data_t * HandleMil)
{
	if (HandleMil->remain_teeth_gear > 0)
	{
		HandleMil->remain_teeth_gear--;
	}
}
