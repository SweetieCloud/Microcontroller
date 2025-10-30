/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define R1_Pin GPIO_PIN_3
#define R1_GPIO_Port GPIOA
#define Y1_Pin GPIO_PIN_4
#define Y1_GPIO_Port GPIOA
#define G1_Pin GPIO_PIN_5
#define G1_GPIO_Port GPIOA
#define R2_Pin GPIO_PIN_6
#define R2_GPIO_Port GPIOA
#define Y2_Pin GPIO_PIN_7
#define Y2_GPIO_Port GPIOA
#define s10_Pin GPIO_PIN_0
#define s10_GPIO_Port GPIOB
#define s11_Pin GPIO_PIN_1
#define s11_GPIO_Port GPIOB
#define s12_Pin GPIO_PIN_2
#define s12_GPIO_Port GPIOB
#define s32_Pin GPIO_PIN_10
#define s32_GPIO_Port GPIOB
#define s33_Pin GPIO_PIN_11
#define s33_GPIO_Port GPIOB
#define mode0_Pin GPIO_PIN_12
#define mode0_GPIO_Port GPIOB
#define mode1_Pin GPIO_PIN_13
#define mode1_GPIO_Port GPIOB
#define mode2_Pin GPIO_PIN_14
#define mode2_GPIO_Port GPIOB
#define mode3_Pin GPIO_PIN_15
#define mode3_GPIO_Port GPIOB
#define G2_Pin GPIO_PIN_8
#define G2_GPIO_Port GPIOA
#define but1_Pin GPIO_PIN_9
#define but1_GPIO_Port GPIOA
#define but2_Pin GPIO_PIN_10
#define but2_GPIO_Port GPIOA
#define but3_Pin GPIO_PIN_11
#define but3_GPIO_Port GPIOA
#define s00_Pin GPIO_PIN_12
#define s00_GPIO_Port GPIOA
#define s01_Pin GPIO_PIN_13
#define s01_GPIO_Port GPIOA
#define s02_Pin GPIO_PIN_14
#define s02_GPIO_Port GPIOA
#define s03_Pin GPIO_PIN_15
#define s03_GPIO_Port GPIOA
#define s13_Pin GPIO_PIN_3
#define s13_GPIO_Port GPIOB
#define s20_Pin GPIO_PIN_4
#define s20_GPIO_Port GPIOB
#define s21_Pin GPIO_PIN_5
#define s21_GPIO_Port GPIOB
#define s22_Pin GPIO_PIN_6
#define s22_GPIO_Port GPIOB
#define s23_Pin GPIO_PIN_7
#define s23_GPIO_Port GPIOB
#define s30_Pin GPIO_PIN_8
#define s30_GPIO_Port GPIOB
#define s31_Pin GPIO_PIN_9
#define CYCLE_TIME_MS 10
#define NO_OF_BUTTONS 3
#define s31_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
