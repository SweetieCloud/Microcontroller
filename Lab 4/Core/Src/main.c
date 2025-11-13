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
#include "scheduler.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*
 * GIA SU TICK 10ms
 * 0.5s = 500ms  = 50 ticks
 * 1.0s = 1000ms = 100 ticks
 * 1.5s = 1500ms = 150 ticks
 * 2.0s = 2000ms = 200 ticks
 * 2.5s = 2500ms = 250 ticks
 */
#define TASK_A_PERIOD 50
#define TASK_B_PERIOD 100
#define TASK_C_PERIOD 150
#define TASK_D_PERIOD 200
#define TASK_E_PERIOD 250

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// KHAI BAO HANDLE CHO TIMER 2
TIM_HandleTypeDef htim2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
// KHAI BAO PROTOTYPE CHO HAM CAU HINH TIM2
static void MX_TIM2_Init(void);

// KHAI BAO PROTOTYPE CHO 5 TASK LED
void task_led_1(void);
void task_led_2(void);
void task_led_3(void);
void task_led_4(void);
void task_led_5(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// DINH NGHIA 5 HAM TASK
// (Dung cac ten pin a1_Pin, a2_Pin... tu file main.h goc cua ban)

void task_led_1(void) {
	// Task 0.5s
	HAL_GPIO_TogglePin(GPIOB, a1_Pin);
}

void task_led_2(void) {
	// Task 1.0s
	HAL_GPIO_TogglePin(GPIOB, a2_Pin);
}

void task_led_3(void) {
	// Task 1.5s
	HAL_GPIO_TogglePin(GPIOB, a3_Pin);
}

void task_led_4(void) {
	// Task 2.0s
	HAL_GPIO_TogglePin(GPIOB, a4_Pin);
}

void task_led_5(void) {
	// Task 2.5s
	HAL_GPIO_TogglePin(GPIOB, a5_Pin);
}

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
  // GOI HAM CAU HINH TIM2
  MX_TIM2_Init();

  // KHOI TAO SCHEDULER
  SCH_Init();

  // THEM 5 TASK VAO HANG DOI
  // (Yeu cau Lab 4)
  SCH_Add_Task(task_led_1, 0, TASK_A_PERIOD); // 0.5s
  SCH_Add_Task(task_led_2, 50, TASK_B_PERIOD); // 1.0s
  SCH_Add_Task(task_led_3, 100, TASK_C_PERIOD); // 1.5s
  SCH_Add_Task(task_led_4, 150, TASK_D_PERIOD); // 2.0s
  SCH_Add_Task(task_led_5, 200, TASK_E_PERIOD); // 2.5s

  // BAT DAU TIMER (de tao ngat 10ms)
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// GOI HAM DISPATCHER LIEN TUC
	// Ham nay se kiem tra co task nao can chay hay khong
	SCH_Dispatch_Tasks();
  }
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /* If you use a button on PAx, enable GPIOA clock as well:
     __HAL_RCC_GPIOA_CLK_ENABLE(); */

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a1_Pin|a2_Pin|a3_Pin|a4_Pin
                          |a5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : a1_Pin a2_Pin a3_Pin a4_Pin
                           a5_Pin */
  GPIO_InitStruct.Pin = a1_Pin|a2_Pin|a3_Pin|a4_Pin
                          |a5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
 * @brief CAU HINH TIMER 2
 * Ham nay cau hinh TIM2 de tao ngat 10ms
 * GIA SU SystemClock (HSI) = 16MHz
 *
 * Tinh toan:
 * Timer Clock = 16,000,000 Hz
 * 1. Prescaler (PSC) = 16000 - 1 = 15999
 * -> Timer Freq = 16,000,000 / 16,000 = 1000 Hz (1ms moi tick)
 * 2. Period (ARR) = 10 - 1 = 9
 * -> Interrupt Freq = 1000 Hz / 10 = 100 Hz
 * -> Thoi gian ngat = 1 / 100 Hz = 0.01s = 10ms
 */
static void MX_TIM2_Init(void) {
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* Enable TIM2 peripheral clock */
	__HAL_RCC_TIM2_CLK_ENABLE();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 15999; // Bo chia 16000
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 9; // Dem den 10 (tu 0 toi 9)
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}

	/* Enable TIM2 interrupt in NVIC so that HAL_TIM_Base_Start_IT triggers IRQ */
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief HAM CALLBACK NGAT TIMER
 * Day la ham se duoc goi moi khi Timer hoan thanh chu ky (moi 10ms)
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// Kiem tra xem co dung la ngat tu TIM2 khong
	if (htim->Instance == TIM2) {
		// Goi ham SCH_Update() de cap nhat bo lap lich
		SCH_Update();
	}
}




/* USER CODE END 4 */

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
  *         where the assert_param error has occurred.
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
