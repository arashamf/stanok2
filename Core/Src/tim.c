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

#define 	TIM_DELAY_us 										TIM7
#define 	TIM_DELAY_us_APB1_BIT 					LL_APB1_GRP1_PERIPH_TIM7

#define 	TIM_BOUNCE_DELAY 								TIM6
#define 	TIM_BOUNCE_DELAY_APB1_BIT 			LL_APB1_GRP1_PERIPH_TIM6
#define  	TIM_BOUNCE_DELAY_IRQn           TIM6_IRQn
#define		TIM_BOUNCE_DELAY_IRQHandler			TIM6_IRQHandler
uint8_t end_bounce = 0;

static void encoder1_init(void);
static void encoder2_init(void) ;
static void tim_delay_init (void);
static void timer_bounce_init (void);
/* USER CODE END 0 */

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
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
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
    */
  GPIO_InitStruct.Pin = PULSE_MOTOR2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(PULSE_MOTOR2_GPIO_Port, &GPIO_InitStruct);

}
/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**TIM2 GPIO Configuration
  PA0-WKUP   ------> TIM2_CH1
  PA1   ------> TIM2_CH2
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  LL_TIM_SetEncoderMode(TIM2, LL_TIM_ENCODERMODE_X2_TI1);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1_N2);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1_N4);
  LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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

/* USER CODE BEGIN 1 */
//-------------------------------------------------------------------------------------------------//
static void encoder1_init(void) 
{
    
  LL_TIM_SetCounter(TIM3, 32767); // ��������� �������� ��������:
	
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(TIM3,LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(TIM3);     // ��������� �������
}

//-------------------------------------------------------------------------------------------------//
static void encoder2_init(void) 
{
    
  LL_TIM_SetCounter(TIM2, 32767); // ��������� �������� ��������:
	
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH1); //Enable the encoder interface channels 
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);

  LL_TIM_EnableCounter(TIM2);     // ��������� �������
}


//------------------------------------------------------------------------------------------------//
static void tim_delay_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(TIM_DELAY_us_APB1_BIT );   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK_VALUE/1000000)-1); //������������ 72���/72=1���
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM_DELAY_us, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_DELAY_us);
}

//-------------------------------------------------------------------------------------------------//
void delay_us(uint16_t delay)
{
  LL_TIM_SetAutoReload(TIM_DELAY_us, delay); //
	LL_TIM_ClearFlag_UPDATE(TIM_DELAY_us); //����� ����� ���������� �������
	LL_TIM_SetCounter(TIM_DELAY_us, 0); //����� �������� ��������
	LL_TIM_EnableCounter(TIM_DELAY_us); //��������� �������
	while (LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY_us) == 0) {} //�������� ��������� ����� ���������� ������� 
	LL_TIM_DisableCounter(TIM_DELAY_us); //���������� �������		
}

//-------------------------------------------------------------------------------------------------//
static void timer_bounce_init (void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(TIM_BOUNCE_DELAY_APB1_BIT);   // Peripheral clock enable 

  TIM_InitStruct.Prescaler = (uint16_t)((CPU_CLOCK_VALUE/2000)-1); //������������ 72���/36000=2���
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM_BOUNCE_DELAY, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM_BOUNCE_DELAY);	
	
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE_DELAY); //����� ����� ���������� �������
	LL_TIM_EnableIT_UPDATE(TIM_BOUNCE_DELAY);
	NVIC_SetPriority(TIM_BOUNCE_DELAY_IRQn, 0);
  NVIC_EnableIRQ(TIM_BOUNCE_DELAY_IRQn);
}

//-------------------------------------------------------------------------------------------------//
void repeat_time (uint16_t delay)
{
	end_bounce = RESET;	
  LL_TIM_SetAutoReload(TIM_BOUNCE_DELAY, 2*delay); //
	LL_TIM_SetCounter(TIM_BOUNCE_DELAY, 0); //����� �������� ��������
	LL_TIM_ClearFlag_UPDATE(TIM_BOUNCE_DELAY); //����� ����� ���������� �������
	LL_TIM_EnableCounter(TIM_BOUNCE_DELAY); //��������� �������	
}

//-------------------------------------------------------------------------------------------------//
void TIM_BOUNCE_DELAY_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM_BOUNCE_DELAY) == SET)
	{	
		LL_TIM_ClearFlag_UPDATE (TIM_BOUNCE_DELAY); //����� ����� ���������� �������
		LL_TIM_DisableCounter(TIM_BOUNCE_DELAY); //���������� �������
		end_bounce = SET; //��������� ����� ��������� �������� ����������� ��������		
	}
}

//-------------------------------------------------------------------------------------------------//
void timers_ini (void)
{
	encoder1_init();
	encoder2_init();
	tim_delay_init(); 		//������������� TIM7 ��� �������������� ��������
	timer_bounce_init();	//������������� TIM6	��� ������ �������� �������� ������ 							
}
/* USER CODE END 1 */
