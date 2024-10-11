
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
	int32_t currCounter = 0; //текущее показание энкодера
	int32_t currDelta = 0; //разница между полученным показани€м энкодера и предыдущим
	
	currCounter = Get_Encoder_data (); //получение текущего показани€ энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //деление на 2, счЄт щелчков (щелчок = 2 импульса)
	
	if(HandleEncData->currCounter_SetClick != HandleEncData->prevCounter_SetClick) //если показани€ энкодера изменились
	{
		currDelta = (HandleEncData->currCounter_SetClick - HandleEncData->prevCounter_SetClick);
		HandleEncData->delta = currDelta; 
		HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение текущего показани€ энкодера
		return ON;
	}
	return OFF; 
}

//------------------------------------сброс показаний энкодера------------------------------------//
void encoder_reset (encoder_data_t * HandleEncData) 
{
	int32_t currCounter = 0; //переменна€ дл€ хранени€ данных энкодера
	currCounter = LL_TIM_GetCounter(TIM_ENC);  //сохранение текущего показани€ энкодера
	HandleEncData->currCounter_SetClick = (32767 - ((currCounter-1) & 0xFFFF))/2; //преобразование полученного показани€ энкодера в формат от -10 до 10
	HandleEncData->prevCounter_SetClick = HandleEncData->currCounter_SetClick; //сохранение преобразованного текущего показани€ энкодера в структуру установки шага поворота	
	HandleEncData->delta = 0; //показани€ от энкодера обнул€ютс€
}

//---------------------------------------------------------------------------------------------------------------//
