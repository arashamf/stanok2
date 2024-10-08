
// Includes ------------------------------------------------------------------//
#include "encoder.h"
#include "tim.h"

// Functions --------------------------------------------------------------------------------------//

// Variables --------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
uint8_t read_encoder1_rotation (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //текущее показание энкодера
	int32_t currDelta = 0; //разница между полученным показаниям энкодера и предыдущим
	//int32_t changeAng = 0; //изменение угла уставки (щелчки энкодера * угол хода вала)
	
	currCounter = Get_Encoder1_data (); //получение текущего показания энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счёт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //если показания энкодера изменились
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick); //разница между полученным и предыдущим показанием энкодера
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение текущего показания энкодера    	
		if (currDelta != 0) //если изменилось положение энкодера
		{		
			HandleEncData->delta += currDelta; //сохранение изменения показаний энкодера (кол-во щелчков энкодера)
		/*	if (currDelta > 0) //если повернулся по часовой стрелке
			{	
				step = currDelta;
			}
			else
			{
				if (currDelta < 0) //если энкодер повернулся против часовой стрелке
				{	
					step = (-1)*currDelta;
				}
			}*/
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

//---------------------------------------------------------------------------------------------------------------//
