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

/* USER CODE BEGIN Private defines */
// TIM_PWM_Drive2 - управление верхним двигателем (намоточный)
#define 	TIM_PWM_Drive2 											TIM1
#define		Drive2_PWM_Channel									LL_TIM_CHANNEL_CH1 

// TIM_PWM_Drive1 - управление нижним двигателем (сдвигающий)
#define 	TIM_PWM_Drive1 											TIM3
#define		Drive1_PWM_Channel									LL_TIM_CHANNEL_CH2 

#define 	TIM_cnt_PWM_DRIVE2 									TIM4
#define		TIM_cnt_PWM_DRIVE2_IRQn							TIM4_IRQn
#define 	TIM_cnt_PWM_DRIVE2_IRQHandler 			TIM4_IRQHandler

#define 	TIM_cnt_PWM_DRIVE1 									TIM5
#define		TIM_cnt_PWM_DRIVE1_IRQn							TIM5_IRQn
#define 	TIM_cnt_PWM_DRIVE1_IRQHandler 			TIM5_IRQHandler

#define 	TIM_ENC 												TIM2

#define 	TIM_DELAY_us 										TIM7
#define 	TIM_DELAY_us_APB1_BIT 					LL_APB1_GRP1_PERIPH_TIM7

#define 	TIM_BOUNCE_DELAY 								TIM6
#define 	TIM_BOUNCE_DELAY_APB1_BIT 			LL_APB1_GRP1_PERIPH_TIM6
#define  	TIM_BOUNCE_DELAY_IRQn           TIM6_IRQn
#define		TIM_BOUNCE_DELAY_IRQHandler			TIM6_IRQHandler
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);

/* USER CODE BEGIN Prototypes */
void Drives_PWM_start(PWM_data_t * );
void Drive1_PWM_start(PWM_data_t * ) ;
void Drive1_PWM_repeat(PWM_data_t * ) ;
void Drive1_PWM_stop(void) ;

void Drive2_PWM_start (PWM_data_t * );
void Drive2_PWM_repeat(PWM_data_t * ) ;
void Drive2_PWM_stop(void);

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

