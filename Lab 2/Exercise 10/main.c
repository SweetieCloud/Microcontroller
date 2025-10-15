/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Digital clock + LED Matrix (Optimized with VSync Double Buffering)
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define MAX_LED 4
#define MAX_LED_MATRIX 8
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
// ==== Digital Clock Variables (Giữ nguyên) ====
int hour = 15, minute = 8, second = 54;
int led_buffer[MAX_LED] = {1, 5, 0, 8};
int scan_counter = 25;
int blink_counter = 100;
int index_led = 0;





// ==== LED Matrix Variables (Tối ưu với VSync Double Buffering) ====
int index_led_matrix = 0;
int animation_counter = 5;

uint8_t matrix_buffer_A[MAX_LED_MATRIX] = {
    0x18, 0x3C, 0x66, 0x66, 0x7E, 0x7E, 0x66, 0x66
};

uint8_t matrix_buffer_B[MAX_LED_MATRIX] = {0};

uint8_t *current_display_buffer = matrix_buffer_A;
uint8_t *current_draw_buffer = matrix_buffer_B;

//(draw buffer) buffer đang vẽ (chuẩn bị frame mới)







// Cờ báo hiệu khi một frame mới đã được vẽ xong và sẵn sàng để tráo đổi
// "volatile" là từ khóa quan trọng để trình biên dịch không tối ưu hóa biến này sai cách
volatile uint8_t frame_is_ready = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void);
void updateLEDMatrix(int index);
void updateScrollingAnimation(void);
void setTimer0 (int duration);
void timer_run (void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin};
const uint16_t Q_PINS[8] = {Q0_Pin, Q1_Pin, Q2_Pin, Q3_Pin, Q4_Pin, Q5_Pin, Q6_Pin, Q7_Pin};
const uint16_t R_PINS[8] = {R7_Pin, R6_Pin, R5_Pin, R4_Pin, R3_Pin, R2_Pin, R1_Pin, R0_Pin};

// ===== Software Timer (Giữ nguyên) =====
int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 10;

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}
void timer_run(){
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
}

// ===== Clock 7-seg (Giữ nguyên) =====
void updateClockBuffer(void){
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}

void display7SEG(int num){
    HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
    switch(num){
        case 0: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|d_Pin|e_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin, GPIO_PIN_RESET); break;
        case 8: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        case 9: HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

void update7SEG(int index){
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin | E3_Pin | E4_Pin, GPIO_PIN_SET);
    display7SEG(led_buffer[index % MAX_LED]);
    HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[index % MAX_LED], GPIO_PIN_RESET);
}

// ===== LED Matrix (Tối ưu với VSync) =====








				// Hàm animation chỉ tính toán và bật cờ, không tráo đổi buffer
				void updateScrollingAnimation() {
					for (int i = 0; i < MAX_LED_MATRIX; i++) {
						current_draw_buffer[i] = (current_display_buffer[i] >> 1) | (current_display_buffer[i] << 7);
					}
					// Sau khi vẽ xong, bật cờ báo hiệu frame mới đã sẵn sàng
					frame_is_ready = 1;
				}









// Hàm quét ma trận (Không thay đổi)
void updateLEDMatrix(int colIndex) {
    HAL_GPIO_WritePin(GPIOA,
        Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin,
        GPIO_PIN_SET);
    uint8_t data = current_display_buffer[colIndex];
    for (int row = 0; row < 8; row++) {
        int bit = (data >> row) & 0x01;
        HAL_GPIO_WritePin(GPIOB, R_PINS[row], bit ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(GPIOA, Q_PINS[colIndex], GPIO_PIN_RESET);
}
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  HAL_TIM_Base_Start_IT(&htim2);
  updateClockBuffer();
  update7SEG(0);
  setTimer0(1000);

  while (1){
      if(timer0_flag == 1){
          second++;
          if(second >= 60){ second = 0; minute++; }
          if(minute >= 60){ minute = 0; hour++; }
          if(hour >= 24) hour = 0;
          updateClockBuffer();
          setTimer0(1000);
      }
  }
}

/* --- Clock + Timer init (Giữ nguyên) --- */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim2);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOA,
      Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin,
      GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, DOT_Pin|LED1_Pin|E1_Pin|E2_Pin|E3_Pin|E4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,
      a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin,
      GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB,
      R0_Pin|R1_Pin|R2_Pin|R3_Pin|R4_Pin|R5_Pin|R6_Pin|R7_Pin,
      GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = Q0_Pin|Q1_Pin|DOT_Pin|LED1_Pin|E1_Pin|E2_Pin|E3_Pin|E4_Pin|
                        Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin|
                        R0_Pin|R1_Pin|R2_Pin|R3_Pin|R4_Pin|R5_Pin|R6_Pin|R7_Pin;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* --- Timer interrupt callback (Tối ưu với VSync) --- */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    timer_run();

    // Quét LED 7 đoạn
    scan_counter--;
    if(scan_counter <= 0){
        scan_counter = 25;
        index_led = (index_led + 1) % MAX_LED;
        update7SEG(index_led);
    }

    // Nhấp nháy LED DOT
    blink_counter--;
    if(blink_counter <= 0){
        blink_counter = 100;
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }

    // Xử lý animation
    animation_counter--;
    if(animation_counter <= 0){
        animation_counter = 20;
        // Chỉ tính toán và bật cờ, không tráo đổi buffer tại đây
        if (frame_is_ready == 0) { // Chỉ tính frame mới nếu frame cũ đã được hiển thị
            updateScrollingAnimation();
        }
    }

    // Quét ma trận LED và thực hiện VSync
    updateLEDMatrix(index_led_matrix);







						// Nếu frame mới đã sẵn sàng VÀ chúng ta vừa quét xong cột cuối cùng (cột 7)
						if (frame_is_ready == 1 && index_led_matrix == 7) {
							// Đây là thời điểm an toàn để tráo đổi buffer
							uint8_t *temp = current_display_buffer;
							current_display_buffer = current_draw_buffer;
							current_draw_buffer = temp;
							// Hạ cờ xuống, báo hiệu đã cập nhật xong
							frame_is_ready = 0;
						}





    index_led_matrix = (index_led_matrix + 1) % MAX_LED_MATRIX;
}

void Error_Handler(void){
  __disable_irq();
  while (1) {}
}
