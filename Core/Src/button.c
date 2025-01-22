
// Includes ------------------------------------------------------------------//
#include "button.h"
#include "typedef.h"
#include "tim.h"
#include "usart.h"

// Exported types -------------------------------------------------------------//
static struct KEY_MACHINE_t Key_Machine;

// Prototypes ---------------------------------------------------------------------------------------//
static uint8_t scan_buttons_GPIO (uint16_t );

//Private defines -----------------------------------------------------------------------------------//
#define KEY_BOUNCE_TIME 			50 				// ����� �������� � ��
#define KEY_AUTOREPEAT_TIME 	100 			// ����� ����������� � ��
#define COUNT_REPEAT_BUTTON 	5

// Private variables -------------------------------------------------------------------------------//
uint8_t count_autorepeat = 0; //������� ��������� ������

//--------------------------------------------------------------------------------------------------//
uint16_t start_scan_key_PEDAL (void)
{
	static __IO uint8_t key_state = KEY_STATE_OFF; // ��������� ��������� ������ - �� ������
	static __IO uint16_t key_code;
	static __IO uint16_t key_repeat_time; // ������� ������� �������
	
	if(key_state == KEY_STATE_OFF) //���� ������ ���� �������� - �������� �������
	{
		if (LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin) == OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
		{
			key_state =  	KEY_STATE_BOUNCE; //������� �� ��������� ������ - ����� �������� ��������� ��������
			key_code 	= 	KEY_PEDAL_SHORT;
			repeat_time (KEY_BOUNCE_TIME); //������ �������� ��������� ��������
		}
	}
	
	if (key_state ==  KEY_STATE_BOUNCE)  //����� �������� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			if(scan_buttons_GPIO(key_code) == OFF)	//���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return NO_KEY; //������� 0 
			}	
		}
		else
		{
			repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			key_state = KEY_STATE_AUTOREPEAT;   //������� � ����� ����������� 
		}		
	}
	
	if(key_state == KEY_STATE_AUTOREPEAT) //����� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			if(scan_buttons_GPIO(key_code) == OFF)	//���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return NO_KEY; //������� 0 
			}	
			else //���� ������ ���������� ������������
			{	
				key_state = KEY_STATE_WAIT_TURNOFF; //������ �������� ���������� ������
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				return key_code;
			}
		}
	}
	
	if (key_state == KEY_STATE_WAIT_TURNOFF) //�������� ���������� ������
	{	
		if (end_bounce == SET) //���� ���� ��������� �������� ���������� (��������������� � ���������� �������)
		{
			if(scan_buttons_GPIO(key_code) == OFF)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{				
				key_code = NO_KEY;
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
			}
			else
			{
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			}
		}
		return key_code;
	}
	return NO_KEY;
}


//--------------------------------------------------------------------------------------------------//
uint16_t scan_keys (void)
{
	static __IO uint8_t key_state = KEY_STATE_OFF; // ��������� ��������� ������ - �� ������
	static __IO uint16_t key_code;
	static __IO uint16_t key_repeat_time; // ������� ������� �������
	
	if(key_state == KEY_STATE_OFF) //���� ������ ���� �������� - �������� �������
	{
		if(LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin) == OFF)	//��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
		{
			key_state =  KEY_STATE_ON; //������� �� ��������� ������ - ����� �������� ������� ������
			key_code = KEY_MODE_SHORT; //���������� ���� ������
		}
		else
		{
			if (LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin) == OFF)
			{
				key_state =  KEY_STATE_ON;
				key_code = KEY_NULL_SHORT;
			}
			else
			{
				if (LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin) == OFF)
				{
					key_state =  	KEY_STATE_ON;
					key_code 	= 	KEY_PEDAL_SHORT;
				}
			}
		}
	}
	
	if (key_state ==  KEY_STATE_ON)  //����� �������� ������� ������
	{
		repeat_time (KEY_BOUNCE_TIME); //������ ������� �������� ��������� ��������
		key_state = KEY_STATE_BOUNCE; // ������� � ����� ��������� ��������
	}
	
	if(key_state == KEY_STATE_BOUNCE) //����� ��������� ��������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� ����������
		{
			if(scan_buttons_GPIO(key_code) == OFF)	 // ���� ������ �������� (������� ����� 50 �� ��� �������)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return NO_KEY; //������� 0 
			}	
			else //���� ������ ���������� ������������
			{	
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
				key_state = KEY_STATE_AUTOREPEAT;   //������� � ����� ����������� 
				count_autorepeat = 0;
			}
		}
	}
	if (key_state == KEY_STATE_AUTOREPEAT) //����� �����������
	{
		if (end_bounce == SET) //���� ���� ��������� �������� (��������������� � ���������� �������)
		{
			if(scan_buttons_GPIO(key_code) == OFF)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
				return key_code; //�������� ������ ������
			}
			else //���� ������ ���������� ������������
			{			
				if (count_autorepeat < COUNT_REPEAT_BUTTON) //�������� 500 ��
				{	count_autorepeat++;	}
				else //���� ������ ������������ ����� 650 ��
				{	
					switch (key_code)
					{
						case KEY_PEDAL_SHORT:
							key_code = KEY_PEDAL_LONG;	
							break;
						
						case KEY_NULL_SHORT:
							key_code = KEY_NULL_LONG;	
							break;
						
						case KEY_MODE_SHORT:
							key_code = KEY_MODE_LONG;	
							break;	
						
						default:
							break;	
					}
					key_state = KEY_STATE_WAIT_TURNOFF; //������ �������� ���������� ������
					repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
					return key_code;
				}
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			} 	
		}					
	}
	
	if (key_state == KEY_STATE_WAIT_TURNOFF) //�������� �������� ���������� ������
	{	
		if (end_bounce == SET) //���� ���� ��������� �������� ���������� (��������������� � ���������� �������)
		{
			if(scan_buttons_GPIO(key_code) == OFF)	 // ���� ������ ���� �������� (�������� ������� ������ < 150 ��)
			{				
				key_code = NO_KEY;
				key_state = KEY_STATE_OFF; //������� � ��������� ��������� �������� ������� ������
			}
			else
			{
				repeat_time (KEY_AUTOREPEAT_TIME); //��������� ������� �������� ���������� ������
			}
		}
	}
	return NO_KEY;
//	return key_code;
}

//-------------------------------------------------------------------------------------------------//
static uint8_t scan_buttons_GPIO (uint16_t key_code)
{
	uint8_t pin_status = OFF; //������ ������ - ������ �� ������
	
	switch (key_code)
	{
						
		case KEY_MODE_SHORT:
			if ((LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin))	== OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
			{	pin_status = ON;	} //������ ������ - ������ ������
			break;
						
		case KEY_MODE_LONG:
			if ((LL_GPIO_IsInputPinSet(ENC1_BTN_GPIO_Port, ENC1_BTN_Pin))	== OFF)
			{	pin_status = ON;	}
			break;	
						
		case KEY_NULL_SHORT:
			if ((LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin))	== OFF)
			{	pin_status = ON;	}
			break;
						
		case KEY_NULL_LONG:
			if ((LL_GPIO_IsInputPinSet(CENTER_BTN_GPIO_Port, CENTER_BTN_Pin))	== OFF)
			{	pin_status = ON;	}
			break;
		
		case KEY_PEDAL_SHORT:
			if ((LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin))	== OFF) 
			{	pin_status = ON;	}
			break;
						
		case KEY_PEDAL_LONG:
			if ((LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin))	== OFF)
			{	pin_status = ON;	}
			break;
		
		default:
			break;	
	}
	
	return pin_status;			
}

//-------------------------------------------------------------------------------------------------//
uint8_t scan_button_PEDAL (void)
{
	uint8_t status_PEDAL = OFF;
	if (LL_GPIO_IsInputPinSet(MODE_BTN_GPIO_Port, MODE_BTN_Pin) == OFF) //��������� ������ - ���� ������ ������, ������� LL_GPIO_IsInputPinSet ���������� 0
	{	status_PEDAL = ON; }
	return status_PEDAL;
}