/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MODE_BTN_Pin LL_GPIO_PIN_0
#define MODE_BTN_GPIO_Port GPIOC
#define LEFT_BTN_Pin LL_GPIO_PIN_1
#define LEFT_BTN_GPIO_Port GPIOC
#define CENTER_BTN_Pin LL_GPIO_PIN_2
#define CENTER_BTN_GPIO_Port GPIOC
#define RIGHT_BTN_Pin LL_GPIO_PIN_3
#define RIGHT_BTN_GPIO_Port GPIOC
#define ENC1_BTN_Pin LL_GPIO_PIN_2
#define ENC1_BTN_GPIO_Port GPIOA
#define RST_Pin LL_GPIO_PIN_0
#define RST_GPIO_Port GPIOB
#define DC_Pin LL_GPIO_PIN_1
#define DC_GPIO_Port GPIOB
#define CS_Pin LL_GPIO_PIN_2
#define CS_GPIO_Port GPIOB
#define DISP_SCK_Pin LL_GPIO_PIN_13
#define DISP_SCK_GPIO_Port GPIOB
#define DISP_MOSI_Pin LL_GPIO_PIN_15
#define DISP_MOSI_GPIO_Port GPIOB
#define PULSE_MOTOR2_Pin LL_GPIO_PIN_8
#define PULSE_MOTOR2_GPIO_Port GPIOA
#define PULSE_MOTOR1_Pin LL_GPIO_PIN_9
#define PULSE_MOTOR1_GPIO_Port GPIOA
#define DIR1_Pin LL_GPIO_PIN_10
#define DIR1_GPIO_Port GPIOA
#define DIR2_Pin LL_GPIO_PIN_11
#define DIR2_GPIO_Port GPIOA
#define ENABLE1_Pin LL_GPIO_PIN_12
#define ENABLE1_GPIO_Port GPIOA
#define ENABLE2_Pin LL_GPIO_PIN_15
#define ENABLE2_GPIO_Port GPIOA
#define EEPROM_SCL_Pin LL_GPIO_PIN_8
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin LL_GPIO_PIN_9
#define EEPROM_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define ON 												1
#define OFF 											0
#define FORWARD 									1
#define BACKWARD 									0
#define DISP_CLEAR 								1
#define DISP_NOT_CLEAR 						0
#define EEPROM_NUMBER_BYTES 			14

#define CPU_CLOCK_VALUE			(72000000UL)	// Дастота контроллера //
#define TICKS_PER_SECOND		1000 
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
