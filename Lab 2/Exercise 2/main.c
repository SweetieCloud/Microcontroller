/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - Lab 2: Timer and LED Scanning (Ex 2)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
// Giá trị hiển thị cố định trên 4 LED: L1=0, L2=0, L3=3, L4=0
int seg_data[4] = {1, 2, 3, 0};
int scan_counter = 50;          // Đếm 50 chu kỳ ngắt 10ms = 500ms (Thời gian chuyển đổi Ts)
int blink_counter = 100;        // Đếm 100 chu kỳ ngắt 10ms = 1000ms (Thời gian nhấp nháy DOT/LED Red)
int led_index = 0;              // Chỉ số của LED 7 đoạn đang được quét (0, 1, 2, 3)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
// Khai báo lại hàm
void display7SEG(int num);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG(int num) {
    // Tắt tất cả các đoạn (segments) (Common Anode: HIGH để tắt)
    HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);

    // Bật các đoạn cần thiết (Common Anode: LOW để bật)
    switch (num) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|d_Pin|e_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin, GPIO_PIN_RESET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 9:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        default:
            // Tắt tất cả
            break;
    }
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // Khởi động Timer 2 ở chế độ ngắt
  HAL_TIM_Base_Start_IT (& htim2 );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  * @brief TIM2 Initialization Function (Timer 10ms)
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  // Giả sử HSI = 8MHz, Prescaler = 7999 => Clock = 1kHz
  // Period = 9 => Chu kỳ ngắt = (9+1)/1kHz = 10ms
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  // DOT_Pin(PA4), LED1_Pin(PA5), E1_Pin(PA6), E2_Pin(PA7), E3_Pin(PA8), E4_Pin(PA9)
  // Ban đầu tắt tất cả các LED Enable (HIGH cho transistor PNP)
  HAL_GPIO_WritePin(GPIOA, DOT_Pin|LED1_Pin|E1_Pin|E2_Pin
                          |E3_Pin|E4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level (Segments a-g) */
  // Ban đầu tắt tất cả các segments (HIGH)
  HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin
                          |f_Pin|g_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : DOT_Pin LED1_Pin E1_Pin E2_Pin E3_Pin E4_Pin */
  GPIO_InitStruct.Pin = DOT_Pin|LED1_Pin|E1_Pin|E2_Pin
                          |E3_Pin|E4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : a_Pin (PB0) */
  // Chân a_Pin của bạn đang được cấu hình là ngắt (Mode = GPIO_MODE_IT_RISING).
  // Để điều khiển 7 đoạn, nó phải là Output. Tôi đã sửa lại phần này.
  GPIO_InitStruct.Pin = a_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(a_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : b_Pin c_Pin d_Pin e_Pin f_Pin g_Pin (PB1 - PB6) */
  GPIO_InitStruct.Pin = b_Pin|c_Pin|d_Pin|e_Pin
                          |f_Pin|g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
/* USER CODE BEGIN 4 */

// Mảng chứa các chân Enable (E1 đến E4)
// Giả định E1_Pin=PA6, E2_Pin=PA7, E3_Pin=PA8, E4_Pin=PA9
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // --- Logic 1: Điều khiển Quét LED (500ms/LED) ---
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 50; // Đặt lại bộ đếm cho 500ms

        // 1. Tắt LED 7 đoạn đang bật (Common Anode -> HIGH để tắt)
        // Đây là bước quan trọng để tránh Ghosting (bóng ma)
        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_SET);

        // 2. Chuyển sang LED 7 đoạn kế tiếp
        led_index = (led_index + 1) % 4; // Chuyển đổi vòng tròn 0 -> 1 -> 2 -> 3 -> 0

        // 3. Hiển thị giá trị mới (1, 2, 3 hoặc 0)
        display7SEG(seg_data[led_index]);

        // 4. Bật LED 7 đoạn mới (Common Anode -> LOW để bật)
        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_RESET);
    }

    // --- Logic 2: Nhấp nháy DOT và LED Red (1000ms = 1 giây) ---
    // Yêu cầu: DOT_Pin (PA4) và LED1_Pin (PA5) nhấp nháy độc lập.
    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; // Đặt lại bộ đếm cho 1000ms

        // Đảo trạng thái của DOT_Pin (PA4)
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);

        // Đảo trạng thái của LED1_Pin (PA5)
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
