# Exercise 3: Code Refactoring and Function Encapsulation

## 1. Requirements
The goal of this exercise is to **implement a function named `update7SEG(int index)`**.  
This function encapsulates the scanning and display logic for the four 7-segment LEDs.

- The function must:
  * Receive the current scan index (0 → 3).
  * Read the corresponding value from the global `led_buffer[4]` array.
  * Turn OFF all 4 Enable pins.
  * Update the segment pins (PB0–PB6) to display the correct digit.
  * Turn ON only the Enable pin corresponding to the current index.

- The function `update7SEG` will be invoked in the timer interrupt handler.  
- The variable `index_led` is incremented and wrapped around (0–3) to cycle through all digits.

| Function | Responsibility |
|----------|----------------|
| update7SEG(int index) | Encapsulates display logic (data + enable control). |
| HAL_TIM_PeriodElapsedCallback | Handles timing and calls `update7SEG`. |

---

## 2. Project Files
- **Proteus_Circuit**: Simulation file with 4 seven-segment LEDs, transistors, and 2 indicator LEDs.  
- **Source_Code**: STM32CubeIDE C source code.  
  - `main.c`: Contains LED scanning and blinking logic.

---

## 3. Source Code Implementation

### New Function `update7SEG(int index)`  

This function replaces inline display code with a reusable block.

```c
/* USER CODE BEGIN 0 */
const int MAX_LED = 4;
int led_buffer[4] = {1, 2, 3, 4};
int index_led = 0;

void update7SEG(int index) {
    // 1. Turn OFF all 4 Enable pins (assume PNP transistors → HIGH = OFF)
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin | E3_Pin | E4_Pin, GPIO_PIN_SET);

    // 2. Display digit value from buffer
    display7SEG(led_buffer[index % MAX_LED]);

    // 3. Enable the corresponding digit (LOW = ON for PNP transistor)
    HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[index % MAX_LED], GPIO_PIN_RESET);
}
/* USER CODE END 0 */
```
**Updated Interrupt Callback**

Timer interrupt only manages counters and calls the refactored function.
```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 50;  // 500ms switching time from Exercise 2
        index_led = (index_led + 1) % 4;
        update7SEG(index_led);
    }

    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; // 1000ms = 1s
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
/* USER CODE END 4 */
