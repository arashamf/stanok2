
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
	//установка передаточного соотношения
	if(HandleCoilData->gear_ratio == 0) 
	{	HandleCoilData->gear_ratio = 100; }
	
	setup_ratio_screen (HandleCoilData->gear_ratio);
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
			setup_ratio_screen (HandleCoilData->gear_ratio);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
		{
			if (key_code ==  KEY_MODE_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
			{				
				break;
			}
		}
	}
	
		//ввод скорости
	if(HandleCoilData->rotation_speed == 0) 
	{	HandleCoilData->rotation_speed = 100; }
	setup_speed_screen (HandleCoilData->rotation_speed);
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
			setup_speed_screen (HandleCoilData->rotation_speed);
		}	
		if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
		{
			if (key_code ==  KEY_MODE_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
			{				
				break;
			}
		}
	}
	
	//установка количества витков обмоток
	for (uint8_t count = 0; count < MAX_NUMBER_COIL; count++)
	{
		setup_coil_screen (count+1, HandleCoilData->set_coil[count]); //заставка режима установки количества витков
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
				setup_coil_screen (count+1, HandleCoilData->set_coil[count]); //заставка режима установки количества витков
			}
			if ((key_code = scan_keys()) != NO_KEY) //если была нажата кнопка
			{
				if (key_code == KEY_MODE_LONG) //длинное нажатие кнопки энкодера - выход из режима установки количества витков
				{		
					HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];					
					HandleCoilData->number_coil = count+1; //количество обмоток
				//	turn_drive_menu (HandleCoilData, 0); //главное меню программы						
					return;
				}	
				else
				{
					if (key_code == KEY_MODE_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
					{
						HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];
						break;
					}
				}
			}			
		}
	}
	HandleCoilData->number_coil = MAX_NUMBER_COIL;
//	turn_drive_menu (HandleCoilData, 0); //главное меню программы
	return;
}

//-------------------------------------------------------------------------------------------------------------//
uint8_t menu_select_preset (encoder_data_t * HandleEncData, coil_data_t ** HandleBufCoilData)
{
	__IO uint16_t key_code = NO_KEY;
	__IO uint8_t drive_mode = PRESET1;	
	{	
		menu_select_preset_screen (drive_mode);
		while (1)
		{		
			if (read_encoder (HandleEncData) == ON) //чтение данных энкодера
			{
				drive_mode += HandleEncData->delta; //
				if (drive_mode > PRESET4)
				{	drive_mode = PRESET1;	}
				else
				{	
					if (drive_mode < PRESET1) 
					{	drive_mode = PRESET4;	}
				}	
				HandleEncData->delta = 0;	
				menu_select_preset_screen (drive_mode);				
			}		
			
			if ((key_code = scan_keys()) != NO_KEY)
			{
				switch (key_code) //обработка кода нажатой кнопки
				{	
					case KEY_MODE_SHORT:
						setup_menu (HandleEncData, *(HandleBufCoilData+drive_mode-1));
						break;
				
					case KEY_MODE_LONG:
						turn_drive_menu (drive_mode, HandleBufCoilData[drive_mode-1], 0); //отображение номера пресета, и количества оставшихся и установленного количества витков 1 обмотки
						return drive_mode;
						break;
				}
				menu_select_preset_screen (drive_mode);		
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------//
void turn_drive_menu (uint8_t numb_preset, coil_data_t * HandleCoilData, uint8_t numb_coil)
{
	default_screen (numb_preset, HandleCoilData->remains_coil[numb_coil], HandleCoilData->set_coil[numb_coil], numb_coil+1);	
}
