/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   : main.c
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

// Định nghĩa số lượng LED và số lượng Software Timers
#define MAX_LED 4
#define NO_OF_TIMERS 2

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
// Biến đồng hồ số
int hour = 15, minute = 8, second = 50;

// Cấu hình quét LED từ Bài tập 4 (Tần số quét tổng 1Hz)
int led_buffer[MAX_LED] = {1, 5, 0, 8}; // Giá trị khởi tạo HH:MM (15:08)
int scan_counter = 25; // T_S = 250ms
int index_led = 0;

// --- SOFTWARE TIMER ARRAY ---
// Timer 0: Clock update (1000ms)
// Timer 1: DOT/LED Blink (1000ms)
int timer_counter[NO_OF_TIMERS] = {0};
int timer_flag[NO_OF_TIMERS] = {0};
int TIMER_CYCLE = 10; // Chu kỳ ngắt Timer 2 là 10ms
// --- END SOFTWARE TIMER ARRAY ---

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void);
// KHAI BÁO THÊM SOFTWARE TIMER (Đã được cập nhật thành setTimer)
void setTimer ( int index, int duration );
void timer_run (void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Mảng chứa các chân Enable (E1 đến E4) được kết nối với PA6 đến PA9
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin};

// BỘ ĐẾM VÀ HÀM CỦA MULTI-SOFTWARE TIMER
/**
 * @brief Đặt thời gian (duration) cho Software Timer theo index
 * @param index Chỉ mục của timer (0 đến NO_OF_TIMERS - 1)
 * @param duration Thời gian cần đếm (tính bằng ms)
 * @retval None
 */
void setTimer ( int index, int duration ){
    if (index >= 0 && index < NO_OF_TIMERS) {
        timer_counter[index] = duration / TIMER_CYCLE ;
        timer_flag[index] = 0;
    }
}

/**
 * @brief Giảm bộ đếm cho TẤT CẢ các timers và đặt flag khi đếm hết
 * @retval None
 */
void timer_run (){
    for (int i = 0; i < NO_OF_TIMERS; i++) {
        if( timer_counter[i] > 0){
            timer_counter[i] --;
            if( timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}
// KẾT THÚC SOFTWARE TIMER IMPLEMENTATION

/**
 * @brief Chuyển đổi giá trị hour và minute thành 4 chữ số trong led_buffer.
 * @retval None
 */
void updateClockBuffer(void) {
    // 1. Xử lý Giờ (HH)
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;

    // 2. Xử lý Phút (MM)
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

/**
 * @brief Hiển thị một chữ số (0-9) lên các chân segment (a-g)
 * @param num Chữ số cần hiển thị
 * @retval None
 */
void display7SEG(int num) {
    // Tắt tất cả các đoạn (segments) (Common Anode: HIGH để tắt)
    HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);

    // Bật các đoạn cần thiết (Common Anode: LOW để bật)
    switch (num) {
        case 0: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|d_Pin|e_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin, GPIO_PIN_RESET); break;
        case 8: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 9: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break; // Đã sửa lỗi số 9
        default: break;
    }
}

/**
 * @brief Hàm chính thực hiện logic quét 4 LED 7 đoạn
 * @param index Chỉ mục của LED cần bật (0 đến 3)
 * @retval None
 */
void update7SEG(int index) {
    // 1. TẮT TẤT CẢ các chân Enable (Tránh hiện tượng "Ghosting")
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin | E3_Pin | E4_Pin, GPIO_PIN_SET);

    // 2. Cài đặt các chân Segment (PB0-PB6) với giá trị của LED hiện tại
    display7SEG(led_buffer[index % MAX_LED]);

    // 3. BẬT chỉ chân Enable tương ứng với index
    HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[index % MAX_LED], GPIO_PIN_RESET);
}
/* USER CODE END 0 */

/**
  * @brief The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // Khởi động Timer 2 ở chế độ ngắt
  HAL_TIM_Base_Start_IT(&htim2);

  // Khởi tạo buffer lần đầu
  updateClockBuffer();

  // KHỞI TẠO CÁC SOFTWARE TIMERS
  setTimer(0, 1000); // Timer 0: Clock update (1s)
  setTimer(1, 1000); // Timer 1: DOT/LED Blink (1s toggle period)
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Vòng lặp chính sử dụng Software Timer (NON-BLOCKING)
  while (1)
  {
    // --- TASK 1: LOGIC ĐỒNG HỒ (Timer 0) ---
    if (timer_flag[0] == 1) {

        // 1. Tăng giây, phút, giờ (Logic Đồng hồ)
        second ++;
        if ( second >= 60) {
            second = 0;
            minute ++;
        }
        if( minute >= 60) {
            minute = 0;
            hour ++;
        }
        if( hour >=24){
            hour = 0;
        }

        // 2. Cập nhật giá trị giờ và phút vào led_buffer
        updateClockBuffer ();

        // 3. Đặt lại Timer cho chu kỳ tiếp theo (1000ms)
        setTimer (0, 1000);
    }

    // --- TASK 2: LOGIC NHẤP NHÁY DOT/LED RED (Timer 1) ---
    if (timer_flag[1] == 1) {

        // Đảo trạng thái của DOT_Pin (PA4) và LED1_Pin (PA5)
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);

        // Đặt lại Timer cho chu kỳ tiếp theo (1000ms)
        setTimer (1, 1000);
    }


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

  /*Configure GPIO pins : a_Pin b_Pin c_Pin d_Pin e_Pin f_Pin g_Pin (PB0 - PB6) */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin
                          |f_Pin|g_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
 * @brief Hàm callback được gọi khi Timer 2 hết chu kỳ (mỗi 10ms)
 * @param htim Con trỏ đến cấu trúc Timer
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    // 1. Quản lý TẤT CẢ Software Timers (Cập nhật 10ms)
    timer_run();

    // 2. Logic Quét LED (Tần số tổng 1Hz)
    scan_counter--;
    if (scan_counter <= 0) {
        // Đặt lại bộ đếm cho 250ms (T_S = 25 * 10ms)
        scan_counter = 25;

        // Cập nhật index và cuốn chiếu (0 -> 1 -> 2 -> 3 -> 0)
        index_led = (index_led + 1) % MAX_LED;

        // Gọi hàm quét và hiển thị
        update7SEG(index_led);
    }

    // Logic Nhấp nháy DOT/LED Red đã được chuyển sang main loop (SỬA ĐỔI BÀI 7)
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
