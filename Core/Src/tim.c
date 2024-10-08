/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "typedef.h"
#include "usart.h"
#include "drive.h"
#include "gpio.h"

// Defines ---------------------------------------------------------------------//

// Variables --------------------------------------------------------------------//
uint8_t end_bounce = 0; //флаг окончания паузы на время дребезга и ожидания отпускания кнопки

// Functions --------------------------------------------------------------------//
static void encoder1_init(void);
static void encoder2_init(void) ;
static void tim_delay_init (void);
static void timer_bounce_init (void);
/* USER CODE END 0 */

TIM_HandleTypeDef htim4;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  TIM_InitStruct.Prescaler = 71;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 255;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 499;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH2);
  TIM_OC_InitStruct.CompareValue = 249;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH2, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_OC1REF);
  LL_TIM_EnableMasterSlaveMode(TIM1);
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
  TIM_BDTRInitStruct.DeadTime = 0;
  TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
  TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);
  /* USER CODE BEGIN TIM1_Init 2 */
	
  /* USER CODE END TIM1_Init 2 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    */
  GPIO_InitStruct.Pin = PUL_DRIVE2_Pin|PUL_DRIVE1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**TIM3 GPIO Configuration
  PA6   ------> TIM3_CH1
  PA7   ------> TIM3_CH2
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  LL_TIM_SetEncoderMode(TIM3, LL_TIM_ENCODERMODE_X2_TI1);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1_N8);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV2_N6);
  LL_TIM_IC_SetPolarity(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM3, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM3);
  LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM3);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}
/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 15;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 99;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
	LL_TIM_ClearFlag_UPDATE(TIM_counter_PWM); 			//сброс флага обновления таймера
	LL_TIM_EnableIT_UPDATE(TIM_counter_PWM); //Enable update interrupt
	NVIC_SetPriority(TIM_counter_PWM_IRQn, 1); //приоритет прерывания
  NVIC_EnableIRQ(TIM_counter_PWM_IRQn);
	
	LL_TIM_SetCounter(TIM_counter_PWM, 0); 		//сброс счётного регистра
	LL_TIM_EnableCounter(TIM_counter_PWM); 		//включение таймера	
  /* USER CODE END TIM4_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//-----------------------------------------------------------------------------------------------//
void PWM_start(PWM_data_t * PWM_data) 
{
	LL_TIM_ClearFlag_UPDATE (TIM_PWM);
	LL_TIM_DisableCounter(TIM_PWM); //выключение таймера
//	LL_TIM_CC_DisableChannel(TIM_PWM, (Drive2_PWM_Channel | Drive1_PWM_Channel));
	LL_TIM_SetCounter(TIM_PWM, 0);
	LL_TIM_SetAutoReload(TIM_PWM, PWM_data->value_Period); //Set the Autoreload value
	LL_TIM_OC_SetCompareCH1(TIM_PWM, PWM_data->Compare_Drive2); //Set compare value for output channel 2 (TIMx_CCR2)
	LL_TIM_OC_SetCompareCH2(TIM_PWM, PWM_data->Compare_Drive1);
	LL_TIM_CC_EnableChannel(TIM_PWM,  (Drive2_PWM_Channel | Drive1_PWM_Channel)); //включение канала 1 таймера
  LL_TIM_EnableAllOutputs(TIM_PWM);	//включение таймера  для генерации ШИМ
	LL_TIM_EnableCounter(TIM_PWM); //Enable timer counter
}

//-----------------------------------------------------------------------------------------------//
void PWM_repeat(PWM_data_t * PWM_data) 
{
	LL_TIM_ClearFlag_UPDATE (TIM_PWM);
	LL_TIM_SetCounter(TIM_PWM, 0);
	LL_TIM_EnableCounter(TIM_PWM); //включение таймера  для генерации ШИМ
	LL_TIM_EnableAllOutputs(TIM_PWM);	//включение каналов таймера 
}

//-----------------------------------------------------------------------------------------------//
void PWM_stop(void) 
{
		LL_TIM_ClearFlag_UPDATE (TIM_PWM);
		LL_TIM_DisableCounter(TIM_PWM); //выключение таймера
		LL_TIM_DisableAllOutputs(TIM_PWM); //
		LL_TIM_CC_DisableChannel(TIM_PWM, (Drive2_PWM_Channel | Drive1_PWM_Channel));
		LL_TIM_SetCounter(TIM_PWM, 0);
}

//-----------------------------------------------------------------------------------------------//
static void encoder1_init(void) 
{
    
  LL_TIM_SetCounter(TIM_ENC1, 32767); // начальное значение счетчика:
	
	LL_TIM_CC_EnableChannel(TIM_ENC1, LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(TIM_ENC1, LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(TIM_ENC1);     // включение таймера
}

//-----------------------------------------------------------------------------------------------//
uint32_t Get_Encoder1_data (void) 
{
	return (LL_TIM_GetCounter(TIM_ENC1));
}

//-----------------------------------------------------------------------------------------------//
static void encoder2_init(void) 
{
    
  LL_TIM_SetCounter(TIM2, 32767); // начальное значение счетчика:
	
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(TIM2);     // включение таймера
}


//----------------------------------------------------------------------------------------------//
static void tim_delay_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(TIM_DELAY_us_APB1_BIT );   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK_VALUE/1000000)-1); //предделитель 72МГц/72=1МГц
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM_DELAY_us, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_DELAY_us);
}

//-----------------------------------------------------------------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_DELAY_us, delay); //
	LL_TIM_ClearFlag_UPDATE(TIM_DELAY_us); //сброс флага обновления таймера
	LL_TIM_SetCounter(TIM_DELAY_us, 0); //сброс счётного регистра
	LL_TIM_EnableCounter(TIM_DELAY_us); //включение таймера
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY_us) == 0) {} //ожидание установки флага обновления таймера 
	LL_TIM_DisableCounter(TIM_DELAY_us); //выключение таймера		
}

//-----------------------------------------------------------------------------------------------//
static void timer_bounce_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(TIM_BOUNCE_DELAY_APB1_BIT);   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK_VALUE/2000)-1); //предделитель 72МГц/36000=2КГц
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM_BOUNCE_DELAY, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_BOUNCE_DELAY);	
	
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE_DELAY); //сброс флага обновления таймера
	LL_TIM_EnableIT_UPDATE(TIM_BOUNCE_DELAY);
	NVIC_SetPriority(TIM_BOUNCE_DELAY_IRQn, 4); //приоритет прерывания
  NVIC_EnableIRQ(TIM_BOUNCE_DELAY_IRQn);
}

//-----------------------------------------------------------------------------------------------//
void repeat_time (uint16_t delay)
{
	end_bounce = RESET;	
  LL_TIM_SetAutoReload(TIM_BOUNCE_DELAY, 2*delay); //
	LL_TIM_SetCounter(TIM_BOUNCE_DELAY, 0); //сброс счётного регистра
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE_DELAY); //сброс флага обновления таймера
	LL_TIM_EnableCounter(TIM_BOUNCE_DELAY); //включение таймера	
}

//-----------------------------------------------------------------------------------------------//
void TIM_BOUNCE_DELAY_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_BOUNCE_DELAY) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TIM_BOUNCE_DELAY); //сброс флага обновления таймера
		LL_TIM_DisableCounter(TIM_BOUNCE_DELAY); //выключение таймера
		end_bounce = SET; //установка флага окончания ожидания прекращения дребезга		
	}
}

//---------------------------------------------------------------------------------------------//
void timers_ini (void)
{
	encoder1_init();
	tim_delay_init(); 		//инициализация TIM7 для микросекундных задержек
	timer_bounce_init();	//инициализация TIM6	для отчёта задержек дребезга кнопок 							
}

//---------------------------------------------------------------------------------------------//
void TIM_counter_PWM_IRQHandler	(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_counter_PWM) == SET)
	{
		PWM_stop();	//выключение генерации ШИМ
		LL_TIM_ClearFlag_UPDATE (TIM_counter_PWM);
		TIM_counter_PWM_Callback ();
	}
}

/* USER CODE END 1 */
