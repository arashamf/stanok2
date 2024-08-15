
// Includes ------------------------------------------------------------------//
#include "drive.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"
//#include "button.h"
//#include "eeprom.h"
//#include "angle_calc.h"

// Functions -----------------------------------------------------------------//
void rotate_step (uint8_t );

// Variables -----------------------------------------------------------------//
__IO float step_unit = ((float)STEP18_IN_SEC/(float)STEP_DIV); //количество секунд в одном микрошаге(1,8гр/100=6480/100=64,8)

//---------------------------------------поворот вала на один микрошаг---------------------------------------//
void rotate_step (uint8_t micro_step)
{
	for (uint8_t count = 0; count < micro_step; count++) //количество микрошагов (импульсов)
	{
		/*	STEP(ON);
			delay_us (4); //4 мкс
			STEP(OFF);
			delay_us (4); //4 мкс*/
	}
}


//-----------------------------------------------------------------------------------------------------//
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //текущее показание энкодера
	int32_t currDelta = 0; //разница между полученным показани€м энкодера и предыдущим
	int32_t changeAng = 0; //изменение угла уставки (щелчки энкодера * угол хода вала)
	
	currCounter = LL_TIM_GetCounter(TIM3); //получение текущего показани€ энкодера
	HandleEncData->currCounter_SetAngle = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счЄт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetAngle != HandleEncData->prevCounter_SetAngle) //если показани€ энкодера изменились
	{
		currDelta = (HandleEncData->currCounter_SetAngle - HandleEncData->prevCounter_SetAngle); //разница между полученным и предыдущим показанием энкодера
    HandleEncData->prevCounter_SetAngle = HandleEncData->currCounter_SetAngle; //сохранение текущего показани€ энкодера    
		
		if (currDelta != 0) //если изменилось положение энкодера
		{		
			HandleEncData->delta += currDelta; //сохранение изменени€ показаний энкодера (кол-во щелчков энкодера)
			if (currDelta > 0) //если повернулс€ по часовой стрелке
			{	}
			else
			{
				if (currDelta < 0) //если энкодер повернулс€ против часовой стрелке
				{	}
			}
		//	default_screen_mode1 (HandleAng, &Font_11x18); //главное меню режима 1
			return ON;
		}
	}
	return OFF; 
}

//--------------------------------------сброс показаний энкодера--------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t encCounter = 0; //переменна€ дл€ хранени€ данных энкодера
	encCounter = LL_TIM_GetCounter(TIM3); //сохранение текущего показани€ энкодера
	HandleEncData->currCounter_SetAngle = (32767 - ((encCounter-1) & 0xFFFF))/2; //преобразование полученного показани€ энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetAngle = HandleEncData->currCounter_SetAngle; //сохранение преобразованного текущего показани€ энкодера в структуру установки шага поворота	
	HandleEncData->prevCounter_ShaftRotation = HandleEncData->currCounter_SetAngle; //сохранение преобразованного текущего показани€ энкодера в структуру данных положени€ вала
	HandleEncData->delta = 0; //показани€ от энкодера обнул€ютс€
}

