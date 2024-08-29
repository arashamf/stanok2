
// Includes ---------------------------------------------------------------------------//
#include "menu.h"
#include "button.h"
#include "drive.h"
#include "typedef.h"
#include "ssd1306.h"
#include "fonts.h"
#include "eeprom.h"
#include "usart.h"
// Declarations and definitions -------------------------------------------------------//

// Variables -------------------------------------------------------------------------//

// Functions -------------------------------------------------------------------------//
void setup_menu (encoder_data_t * HandleEncData, coil_data_t * HandleCoilData)
{
	uint16_t key_code = NO_KEY;
	uint8_t wdata_size = sizeof(HandleCoilData->coil_buffer);
	memset (HandleCoilData->coil_buffer, 0, wdata_size);
	
	for (uint8_t count = 0; count < MAX_NUMBER_COIL; count++)
	{
		setup_coil_screen (&Font_16x26, count+1, HandleEncData->currCounter_SetClick); //заставка режима установки
		while (1)
		{
			if (read_encoder1_rotation (HandleEncData) == ON)
			{
				setup_coil_screen (&Font_16x26, count+1, HandleEncData->currCounter_SetClick); //заставка режима установки
			}
			if ((key_code = scan_keys()) != NO_KEY)
			{
				if (key_code == KEY_ENC_LONG) //длинное нажатие кнопки энкодера - выход из режима установки 
				{
					if (HandleEncData->currCounter_SetClick > 0)
					{	
						HandleCoilData->set_coil[count] = HandleEncData->currCounter_SetClick;	
						HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];
					}
					
					HandleCoilData->number_coil = count+1; //количество обмоток
					SaveCoilData (HandleCoilData->coil_buffer, wdata_size, EEPROM_MEMORY_PAGE);
					main_menu (HandleCoilData); //главное меню дисплея							
					return;
				}	
				else
				{
					if (key_code ==  KEY_ENC_SHORT) //короткое нажатие кнопки энкодера - переход к вводу количества витков следующей обмотки
					{
						if (HandleEncData->currCounter_SetClick > 0)
						{	
							HandleCoilData->set_coil[count] = HandleEncData->currCounter_SetClick;	
							HandleCoilData->remains_coil[count] = HandleCoilData->set_coil[count];
						}
						break;
					}
				}
			}			
		}
	}
	HandleCoilData->number_coil = MAX_NUMBER_COIL;
	SaveCoilData (HandleCoilData->coil_buffer, wdata_size, EEPROM_MEMORY_PAGE);
	main_menu (HandleCoilData);
	return;
}

//--------------------------------------------------------------------------------------------//
void main_menu (coil_data_t * HandleCoilData)
{
	default_screen (&Font_16x26, HandleCoilData->remains_coil[0], HandleCoilData->set_coil[0]);	
}