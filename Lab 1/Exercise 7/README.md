# Exercise 7: _clearAllClock()_ Function

## 1. Requirements

This exercise requires you to create a simple function to turn off all 12 LEDs from the analog clock circuit designed in Exercise 6.

Implement a function named **_clearAllClock()_** to turn off all 12 LEDs connected to the **GPIOA** pins (**PA4** to **PA15**).

## 2. Source Code
The following code is implemented in main.c, using a bitmask to turn off all LEDs efficiently.

_`clearAllClock()`_ **Function**
```c
/* USER CODE BEGIN 0 */
void clearAllClock()
{
    // GPIO_PIN_4 đến GPIO_PIN_15 tương ứng với 12 đèn LED
    // 0xFFF0 là mask bit để chọn tất cả các chân này
    HAL_GPIO_WritePin(GPIOA, 0xFFF0, GPIO_PIN_RESET);
}
/* USER CODE END 0 */

