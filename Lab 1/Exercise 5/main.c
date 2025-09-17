/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  * opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void displayLeftRight7SEG(int num);
void displayUpDown7SEG(int num);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    HAL_GPIO_WritePin(GPIOA, LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_YELLOWA5_Pin | LED_REDA4_Pin | LED_YELLOWA11_Pin | LED_REDA10_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOA, LED_REDA1_Pin | LED_REDA7_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_GREENA3_Pin | LED_YELLOWA8_Pin | LED_GREENA9_Pin, GPIO_PIN_SET);

    int counter_ngang = 3;
    int counter_doc = 5;
    for (int i = 0; i < 5; i++) {
        if (i < 3) {
            displayLeftRight7SEG(counter_ngang);
            counter_ngang--;
        } else if (i == 3) {
            HAL_GPIO_WritePin(GPIOA, LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, LED_YELLOWA5_Pin | LED_YELLOWA11_Pin, GPIO_PIN_RESET);
            displayLeftRight7SEG(2);
        } else {
            displayLeftRight7SEG(1);
        }

        displayUpDown7SEG(counter_doc);
        counter_doc--;
        HAL_Delay(1000);
    }

    HAL_GPIO_WritePin(GPIOA, LED_REDA4_Pin | LED_REDA10_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_YELLOWA5_Pin | LED_GREENA6_Pin | LED_YELLOWA11_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOA, LED_GREENA3_Pin | LED_GREENA9_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_REDA1_Pin | LED_YELLOWA8_Pin | LED_REDA7_Pin, GPIO_PIN_SET);

    counter_ngang = 5;
    counter_doc = 3;
    for (int i = 0; i < 5; i++) {
        displayLeftRight7SEG(counter_ngang);
        counter_ngang--;

        if (i < 3) {
            displayUpDown7SEG(counter_doc);
            counter_doc--;
        } else if (i == 3) {
            HAL_GPIO_WritePin(GPIOA, LED_GREENA3_Pin | LED_GREENA9_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_YELLOWA8_Pin, GPIO_PIN_RESET);
            displayUpDown7SEG(2);
        } else {
            displayUpDown7SEG(1);
        }
        HAL_Delay(1000);
    }
  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_REDA1_Pin|LED_YELLOWA2_Pin|LED_GREENA3_Pin|LED_REDA4_Pin
                            |LED_YELLOWA5_Pin|LED_GREENA6_Pin|LED_REDA7_Pin|LED_YELLOWA8_Pin
                            |LED_GREENA9_Pin|LED_REDA10_Pin|LED_YELLOWA11_Pin|LED_GREENA12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin|h_Pin|i_Pin|j_Pin|k_Pin|l_Pin|m_Pin|n_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_REDA1_Pin LED_YELLOWA2_Pin LED_GREENA3_Pin LED_REDA4_Pin
                            LED_YELLOWA5_Pin LED_GREENA6_Pin LED_REDA7_Pin LED_YELLOWA8_Pin
                            LED_GREENA9_Pin LED_REDA10_Pin LED_YELLOWA11_Pin LED_GREENA12_Pin */
  GPIO_InitStruct.Pin = LED_REDA1_Pin|LED_YELLOWA2_Pin|LED_GREENA3_Pin|LED_REDA4_Pin
                            |LED_YELLOWA5_Pin|LED_GREENA6_Pin|LED_REDA7_Pin|LED_YELLOWA8_Pin
                            |LED_GREENA9_Pin|LED_REDA10_Pin|LED_YELLOWA11_Pin|LED_GREENA12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a_Pin b_Pin c_Pin d_Pin e_Pin f_Pin g_Pin
                            h_Pin i_Pin j_Pin k_Pin l_Pin m_Pin n_Pin */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin
                            |h_Pin|i_Pin|j_Pin|k_Pin|l_Pin|m_Pin|n_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 4 */

/**
 * @brief  Displays a number on the left/right 7-segment displays.
 * @param  num: The number to display (0-9).
 * @retval None
 */
void displayLeftRight7SEG(int num)
{
    // Common-anode 7-segment display pin configuration
    // a-g segments connected to PB0-PB6

    // Hex values for each digit (0-9) for a common-anode display
    uint16_t segments[] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F  // 9
    };

    // Invert the segments for common-anode logic (active-low)
    uint16_t invertedSegments = ~segments[num];

    HAL_GPIO_WritePin(GPIOB, a_Pin, (GPIO_PinState)((invertedSegments >> 0) & 1));
    HAL_GPIO_WritePin(GPIOB, b_Pin, (GPIO_PinState)((invertedSegments >> 1) & 1));
    HAL_GPIO_WritePin(GPIOB, c_Pin, (GPIO_PinState)((invertedSegments >> 2) & 1));
    HAL_GPIO_WritePin(GPIOB, d_Pin, (GPIO_PinState)((invertedSegments >> 3) & 1));
    HAL_GPIO_WritePin(GPIOB, e_Pin, (GPIO_PinState)((invertedSegments >> 4) & 1));
    HAL_GPIO_WritePin(GPIOB, f_Pin, (GPIO_PinState)((invertedSegments >> 5) & 1));
    HAL_GPIO_WritePin(GPIOB, g_Pin, (GPIO_PinState)((invertedSegments >> 6) & 1));
}

/**
 * @brief  Displays a number on the up/down 7-segment displays.
 * @param  num: The number to display (0-9).
 * @retval None
 */
void displayUpDown7SEG(int num)
{
    // Common-anode 7-segment display pin configuration
    // a-g segments connected to PB7-PB13

    // Hex values for each digit (0-9) for a common-anode display
    uint16_t segments[] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F  // 9
    };


    uint16_t invertedSegments = ~segments[num];

    HAL_GPIO_WritePin(GPIOB, h_Pin, (GPIO_PinState)((invertedSegments >> 0) & 1));
    HAL_GPIO_WritePin(GPIOB, i_Pin, (GPIO_PinState)((invertedSegments >> 1) & 1));
    HAL_GPIO_WritePin(GPIOB, j_Pin, (GPIO_PinState)((invertedSegments >> 2) & 1));
    HAL_GPIO_WritePin(GPIOB, k_Pin, (GPIO_PinState)((invertedSegments >> 3) & 1));
    HAL_GPIO_WritePin(GPIOB, l_Pin, (GPIO_PinState)((invertedSegments >> 4) & 1));
    HAL_GPIO_WritePin(GPIOB, m_Pin, (GPIO_PinState)((invertedSegments >> 5) & 1));
    HAL_GPIO_WritePin(GPIOB, n_Pin, (GPIO_PinState)((invertedSegments >> 6) & 1));
}
/* USER CODE END 4 */
