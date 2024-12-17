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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// Define Event Group Bits
#define travelMug_ISR_BIT (1UL << 0UL)		// bit0
#define largeMug_ISR_BIT (1UL << 1UL)		// bit1
#define smallMug_ISR_BIT (1UL << 2UL)		// bit2
#define smallCup_ISR_BIT (1UL << 3UL)		// bit3

/* Buffer for incoming commands */
#define CMD_BUFFER_SIZE 32

/* Command IDs */
#define CMD_POWER_ON_OFF       0x01
#define CMD_TOGGLE_STRONG_BREW 0x02
#define CMD_TOGGLE_PWR         0x03
#define CMD_SET_WATER_LEVEL    0x04
#define CMD_GET_PWR_STATUS     0x05




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
#define BUTTON_PWR_Pin GPIO_PIN_0
#define BUTTON_PWR_GPIO_Port GPIOA
#define BUTTON_PWR_EXTI_IRQn EXTI0_IRQn
#define BUTTON_STR_BRW_Pin GPIO_PIN_1
#define BUTTON_STR_BRW_GPIO_Port GPIOA
#define BUTTON_STR_BRW_EXTI_IRQn EXTI1_IRQn
#define BUTTON_LARG_MUG_Pin GPIO_PIN_3
#define BUTTON_LARG_MUG_GPIO_Port GPIOA
#define BUTTON_LARG_MUG_EXTI_IRQn EXTI3_IRQn
#define LED_TRAV_MUG_Pin GPIO_PIN_4
#define LED_TRAV_MUG_GPIO_Port GPIOA
#define LED_SMALL_CUP_Pin GPIO_PIN_5
#define LED_SMALL_CUP_GPIO_Port GPIOA
#define LED_SMALL_MUG_Pin GPIO_PIN_6
#define LED_SMALL_MUG_GPIO_Port GPIOA
#define LED_LARG_MUG_Pin GPIO_PIN_7
#define LED_LARG_MUG_GPIO_Port GPIOA
#define LED_PWR_ON_Pin GPIO_PIN_1
#define LED_PWR_ON_GPIO_Port GPIOB
#define BUTTON_SMALL_CUP_Pin GPIO_PIN_2
#define BUTTON_SMALL_CUP_GPIO_Port GPIOB
#define BUTTON_SMALL_CUP_EXTI_IRQn EXTI2_IRQn
#define BUTTON_TRAV_MUG_Pin GPIO_PIN_14
#define BUTTON_TRAV_MUG_GPIO_Port GPIOD
#define BUTTON_TRAV_MUG_EXTI_IRQn EXTI15_10_IRQn
#define LED_STR_BRW_Pin GPIO_PIN_15
#define LED_STR_BRW_GPIO_Port GPIOA
#define BUTTON_SMALL_MUG_Pin GPIO_PIN_4
#define BUTTON_SMALL_MUG_GPIO_Port GPIOB
#define BUTTON_SMALL_MUG_EXTI_IRQn EXTI4_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
