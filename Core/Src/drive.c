
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
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)

//----------------------------------поворот вала на один микрошаг----------------------------------//
void rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //количество микрошагов (импульсов)
	{
			STEP_DRIVE1(ON);
			delay_us (3); //3 мкс
			STEP_DRIVE1(OFF);
			delay_us (3); //3 мкс
	}
}

//------------------------------------------------------------------------------------------------//
void milling_step (uint8_t dir)
{
	uint32_t quant = 	step*STEP_DIV;; 
	
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (dir); //направление вращения
	delay_us (6);	
	
	rotate_step (quant);
	DRIVE1_ENABLE(OFF);
}

//------------------------------------------------------------------------------------------------//
void one_full_turn (void) 
{
	DRIVE1_ENABLE(ON);
	DIR_DRIVE1 (FORWARD); //направление вращения
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
	int32_t currCounter = 0; //текущее показание энкодера
	int32_t currDelta = 0; //разница между полученным показаниям энкодера и предыдущим
	//int32_t changeAng = 0; //изменение угла уставки (щелчки энкодера * угол хода вала)
	
	currCounter = LL_TIM_GetCounter(TIM3); //получение текущего показания энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счёт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //если показания энкодера изменились
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick); //разница между полученным и предыдущим показанием энкодера
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение текущего показания энкодера    	
		if (currDelta != 0) //если изменилось положение энкодера
		{		
			HandleEncData->delta += currDelta; //сохранение изменения показаний энкодера (кол-во щелчков энкодера)
			if (currDelta > 0) //если повернулся по часовой стрелке
			{	
				step = currDelta;
			}
			else
			{
				if (currDelta < 0) //если энкодер повернулся против часовой стрелке
				{	
					step = (-1)*currDelta;
				}
			}
			return ON;
		}
	}
	return OFF; 
}

//------------------------------------сброс показаний энкодера------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //переменная для хранения данных энкодера
	encCounter = LL_TIM_GetCounter(TIM3); //сохранение текущего показания энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((encCounter-1) & 0xFFFF))/2; //преобразование полученного показания энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение преобразованного текущего показания энкодера в структуру установки шага поворота	
	HandleEncData->delta = 0; //показания от энкодера обнуляются
}
