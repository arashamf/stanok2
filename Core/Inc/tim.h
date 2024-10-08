/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "typedef.h"

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */
#define 	TIM_PWM 												TIM1
#define		Drive2_PWM_Channel							LL_TIM_CHANNEL_CH1 
#define		Drive1_PWM_Channel							LL_TIM_CHANNEL_CH2 

#define 	TIM_counter_PWM 								TIM4
#define		TIM_counter_PWM_IRQn						TIM4_IRQn
#define 	TIM_counter_PWM_IRQHandler 			TIM4_IRQHandler

#define 	TIM_ENC1 												TIM3

#define 	TIM_DELAY_us 										TIM7
#define 	TIM_DELAY_us_APB1_BIT 					LL_APB1_GRP1_PERIPH_TIM7

#define 	TIM_BOUNCE_DELAY 								TIM6
#define 	TIM_BOUNCE_DELAY_APB1_BIT 			LL_APB1_GRP1_PERIPH_TIM6
#define  	TIM_BOUNCE_DELAY_IRQn           TIM6_IRQn
#define		TIM_BOUNCE_DELAY_IRQHandler			TIM6_IRQHandler
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

/* USER CODE BEGIN Prototypes */
void PWM_start (PWM_data_t * );
void PWM_repeat(PWM_data_t * ) ;
void PWM_stop(void) ;
uint32_t Get_Encoder1_data (void);
void delay_us(uint16_t );
void repeat_time (uint16_t );
void timers_ini (void);

//Variables -----------------------------------------------------------------------//
extern uint8_t end_bounce;

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

