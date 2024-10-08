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

#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_gpio.h"

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
#define ENABLE2_Pin LL_GPIO_PIN_8
#define ENABLE2_GPIO_Port GPIOC
#define DIR2_Pin LL_GPIO_PIN_9
#define DIR2_GPIO_Port GPIOC
#define PUL_DRIVE2_Pin LL_GPIO_PIN_8
#define PUL_DRIVE2_GPIO_Port GPIOA
#define PUL_DRIVE1_Pin LL_GPIO_PIN_9
#define PUL_DRIVE1_GPIO_Port GPIOA
#define ENABLE1_Pin LL_GPIO_PIN_10
#define ENABLE1_GPIO_Port GPIOA
#define DIR1_Pin LL_GPIO_PIN_11
#define DIR1_GPIO_Port GPIOA
#define ENABLE2A15_Pin LL_GPIO_PIN_15
#define ENABLE2A15_GPIO_Port GPIOA
#define RIGHT_IN_Pin LL_GPIO_PIN_12
#define RIGHT_IN_GPIO_Port GPIOC
#define RIGHT_IN_EXTI_IRQn EXTI15_10_IRQn
#define LEFT_IN_Pin LL_GPIO_PIN_2
#define LEFT_IN_GPIO_Port GPIOD
#define LEFT_IN_EXTI_IRQn EXTI2_IRQn
#define EEPROM_SCL_Pin LL_GPIO_PIN_8
#define EEPROM_SCL_GPIO_Port GPIOB
#define EEPROM_SDA_Pin LL_GPIO_PIN_9
#define EEPROM_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define __USE_DBG
#define __SPI_DMA_MODE

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
