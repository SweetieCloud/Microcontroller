/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : timer.c
  * @brief          : Timer interrupt and scheduling
  ******************************************************************************
  * @author         : BUI THANH QUI
  * @date           : Oct 23, 2025
  ******************************************************************************
  */
/* USER CODE END Header */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

#include "main.h"
#include "input_reading.h"
#include "led_display.h"

// =============================
// Biến extern được sử dụng
// =============================

// Cờ xác định đang ở chế độ chạy LED bình thường (mode 1)
extern int flagInterruptLED_and_SEG;
extern void runTimer_LED(void);
extern void runBlinkMode(void);
extern void fsm_for_input_processing(void);

// =============================
// Hàm tick xử lý 2Hz blinking và gọi hàm debounce
// =============================


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        // 1. Xử lý nút nhấn và debounce (gọi mỗi 10ms)
        button_reading();

        // 2. Xử lý FSM (gọi mỗi 10ms)
        fsm_for_input_processing();

        // 3. Xử lý hiển thị đèn/timer
        if (flagInterruptLED_and_SEG == 1)
        {
            runTimer_LED(); // Mode 1: Chạy đèn giao thông
        }
        else
        {
            runBlinkMode(); // Mode 2, 3, 4: Chớp LED
        }
    }
}
