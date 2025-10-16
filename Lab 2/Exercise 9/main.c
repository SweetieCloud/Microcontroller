/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Digital clock + LED Matrix (display letter 'A')
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
// ==== Digital Clock Variables ====
int hour = 15, minute = 8, second = 55;

int led_buffer[MAX_LED] = {1, 5, 0, 8}; // HH:MM
int index_led = 0;

// ==== LED Matrix Variables ====
int index_led_matrix = 0;

// --- Letter 'A' pattern (Cathode via ULN2803, LOW = ON) ---
uint8_t matrix_buffer[MAX_LED_MATRIX] = {
    0x18, // 00011000
    0x3C, // 00111100
    0x66, // 01100110
    0x66, // 01100110
    0x7E, // 01111110
    0x7E, // 01111110
    0x66, // 01100110
    0x66  // 01100110
};

// ===== Software Timers =====
int timer0_counter = 0;
int timer0_flag = 0;
int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;
int timer2_flag = 0;
int timer3_counter = 0;
int timer3_flag = 0;
int TIMER_CYCLE = 10; // 10 ms

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
void setTimer0 (int duration);
void setTimer1 (int duration);
void setTimer2 (int duration);
void setTimer3 (int duration);
void timer_run (void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin};
const uint16_t Q_PINS[8] = {Q0_Pin, Q1_Pin, Q2_Pin, Q3_Pin, Q4_Pin, Q5_Pin, Q6_Pin, Q7_Pin};
const uint16_t R_PINS[8] = {R7_Pin, R6_Pin, R5_Pin, R4_Pin, R3_Pin, R2_Pin, R1_Pin, R0_Pin};

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

void setTimer1(int duration){
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration){
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

void setTimer3(int duration){
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

void timer_run(){
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
    if(timer1_counter > 0){
        timer1_counter--;
        if(timer1_counter == 0) timer1_flag = 1;
    }
    if(timer2_counter > 0){
        timer2_counter--;
        if(timer2_counter == 0) timer2_flag = 1;
    }
    if(timer3_counter > 0){
        timer3_counter--;
        if(timer3_counter == 0) timer3_flag = 1;
    }
}

// ===== Clock 7-seg =====
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

// ===== LED Matrix =====
void updateLEDMatrix(int index){
    switch (index){
    case 0:
        // Turn off all columns
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        // Set rows for column 0
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[0] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[0] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[0] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[0] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[0] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[0] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[0] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[0] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        // Enable column 0
        HAL_GPIO_WritePin(GPIOA, Q_PINS[0], GPIO_PIN_RESET);
        break;
    case 1:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[1] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[1] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[1] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[1] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[1] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[1] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[1] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[1] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[1], GPIO_PIN_RESET);
        break;
    case 2:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[2] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[2] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[2] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[2] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[2] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[2] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[2] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[2] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[2], GPIO_PIN_RESET);
        break;
    case 3:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[3] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[3] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[3] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[3] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[3] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[3] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[3] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[3] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[3], GPIO_PIN_RESET);
        break;
    case 4:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[4] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[4] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[4] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[4] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[4] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[4] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[4] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[4] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[4], GPIO_PIN_RESET);
        break;
    case 5:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[5] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[5] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[5] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[5] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[5] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[5] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[5] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[5] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[5], GPIO_PIN_RESET);
        break;
    case 6:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[6] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[6] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[6] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[6] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[6] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[6] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[6] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[6] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[6], GPIO_PIN_RESET);
        break;
    case 7:
        HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[0], ((matrix_buffer[7] >> 0) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[1], ((matrix_buffer[7] >> 1) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[2], ((matrix_buffer[7] >> 2) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[3], ((matrix_buffer[7] >> 3) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[4], ((matrix_buffer[7] >> 4) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[5], ((matrix_buffer[7] >> 5) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[6], ((matrix_buffer[7] >> 6) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, R_PINS[7], ((matrix_buffer[7] >> 7) & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, Q_PINS[7], GPIO_PIN_RESET);
        break;
    default:
        break;
    }
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
  updateLEDMatrix(0);
  setTimer0(1000);
  setTimer1(1000);
  setTimer2(250);
  setTimer3(10);

  while (1){
      if(timer0_flag == 1){
          second++;
          if(second >= 60){ second = 0; minute++; }
          if(minute >= 60){ minute = 0; hour++; }
          if(hour >= 24) hour = 0;
          updateClockBuffer();
          setTimer0(1000);
      }
      if(timer1_flag == 1){
          HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
          HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
          setTimer1(1000);
      }
      if(timer2_flag == 1){
          index_led = (index_led + 1) % MAX_LED;
          update7SEG(index_led);
          setTimer2(250);
      }
      if(timer3_flag == 1){
          index_led_matrix = (index_led_matrix + 1) % MAX_LED_MATRIX;
          updateLEDMatrix(index_led_matrix);
          setTimer3(10);
      }
  }
}

/* --- Clock + Timer init --- */
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
  htim2.Init.Period = 9; // chính xác 10ms
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
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
      GPIO_PIN_SET); // HIGH = off
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

/* --- Timer interrupt callback --- */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    timer_run();
}

void Error_Handler(void){
  __disable_irq();
  while (1) {}
}
