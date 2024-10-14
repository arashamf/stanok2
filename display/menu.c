
// Includes ---------------------------------------------------------------------------//
#include "menu.h"
#include "button.h"
#include "drive.h"
#include "typedef.h"
#include "ssd1306.h"
#include "fonts.h"
#include "eeprom.h"
#include "encoder.h"
#include "usart.h"
// Declarations and definitions -------------------------------------------------------//

// Variables -------------------------------------------------------------------------//

// Functions -------------------------------------------------------------------------//
void setup_menu (encoder_data_t * HandleEncData, coil_data_t * HandleCoilData)
{
	uint16_t key_code = NO_KEY;
	//uint8_t wdata_size = sizeof(HandleCoilData->coil_buffer);
	//memset (HandleCoilData->coil_buffer, 0, wdata_size);
	
	//установка передаточного соотношения
	if(HandleCoilData->gear_ratio == 0) 
	{	HandleCoilData->gear_ratio = 100; }
	setup_ratio_screen (&Font_16x26, HandleCoilData->gear_ratio);
	while (1)
	{
		if (read_encoder (HandleEncData) == ON) //если данные от энкодера изменилось
		{
			HandleCoilData->gear_ratio += 5*HandleEncData->delta;
			if (HandleCoilData->gear_ratio > 400)
			{	HandleCoilData->gear_ratio = 25;	}
			else
			{
				if (HandleCoilData->gear_ratio < 25)
				{	HandleCoilData->gear_ratio = 400;	}
			}
			setup_ratio_screen (&Font_16x26, HandleCoilData->gear_ratio);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
		{
			if (key_code ==  KEY_NULL_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
			{				
				break;
			}
		}
	}
	
		//ввод скорости
	if(HandleCoilData->rotation_speed == 0) 
	{	HandleCoilData->rotation_speed = 100; }
	setup_speed_screen (&Font_16x26, HandleCoilData->rotation_speed);
	while (1)
	{
		if (read_encoder (HandleEncData) == ON) //если данные от энкодера изменилось
		{
			HandleCoilData->rotation_speed += 5*HandleEncData->delta;
			if (HandleCoilData->rotation_speed > 400)
			{	HandleCoilData->rotation_speed = 25;	}
			else
			{
				if (HandleCoilData->rotation_speed < 25)
				{	HandleCoilData->rotation_speed = 400;	}
			}
			setup_speed_screen (&Font_16x26, HandleCoilData->rotation_speed);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
		{
			if (key_code ==  KEY_NULL_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
			{				
				break;
			}
		}
	}
	
	//установка количества витков обмоток
	for (uint8_t count = 0; count < MAX_NUMBER_COIL; count++)
	{
		setup_coil_screen (&Font_16x26, count+1, HandleCoilData->set_coil[count]); //заставка режима установки количества витков
		while (1)
		{
			if (read_encoder (HandleEncData) == ON) //если данные от энкодера изменилось
			{
				HandleCoilData->set_coil[count] += HandleEncData->delta;	//прибавление приращения показаний энкодера
				if (HandleCoilData->set_coil[count] < 1)
				{	HandleCoilData->set_coil[count] = 250;	}
				else
				{
					if (HandleCoilData->set_coil[count] > 250)
					{	HandleCoilData->set_coil[count] = 1;	}
				}
				setup_coil_screen (&Font_16x26, count+1, HandleCoilData->set_coil[count]); //заставка режима установки количества витков
			}
			if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
			{
				if (key_code == KEY_NULL_LONG) //длинное нажатие кнопки энкодера - выход из режима установки количества витков
				{		
					HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];					
					HandleCoilData->number_coil = count+1; //количество обмоток
					turn_drive_menu (HandleCoilData, 0); //главное меню программы						
					return;
				}	
				else
				{
					if (key_code ==  KEY_NULL_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
					{
						HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];
						break;
					}
				}
			}			
		}
	}
	HandleCoilData->number_coil = MAX_NUMBER_COIL;
	turn_drive_menu (HandleCoilData, 0); //главное меню программы
	return;
}

//-------------------------------------------------------------------------------------------------------------//
void main_menu (void)
{
	SSD1306_GotoXY(LCD_DEFAULT_X_SIZE, LCD_DEFAULT_Y_SIZE);
	snprintf ((char *)LCD_buff, LCD_BUFFER_SIZE, "SELECT");
	SSD1306_Puts (LCD_buff , &Font_16x26, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

//-------------------------------------------------------------------------------------------------------------//
void dbg_menu (void)
{
	SSD1306_GotoXY(LCD_DEFAULT_X_SIZE, LCD_DEFAULT_Y_SIZE);
	snprintf ((char *)LCD_buff, LCD_BUFFER_SIZE, "START");
	SSD1306_Puts (LCD_buff , &Font_16x26, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
}

//-------------------------------------------------------------------------------------------------------------//
void turn_drive_menu (coil_data_t * HandleCoilData, uint8_t number)
{
	default_screen (&Font_11x17, HandleCoilData->remains_coil[number], HandleCoilData->set_coil[number], number+1);	
}
