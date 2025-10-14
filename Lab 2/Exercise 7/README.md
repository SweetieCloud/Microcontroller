# Exercise 7: Moving DOT Blink Logic to Main Loop (Multi-Timer)

## 1. Requirements

The objective of this exercise is to minimize the processing load on the Timer Interrupt Handler by moving the DOT and LED Red blinking logic out of `HAL_TIM_PeriodElapsedCallback` and into the non-blocking `while(1)` loop. This is achieved by expanding the Software Timer mechanism to handle multiple independent timers.

* **Software Timer Expansion:** Implement a multi-timer structure (using arrays) to manage at least two independent timers: **Timer 0** for the 1s clock tick and **Timer 1** for the 1s DOT/LED Red blink period.
* **Non-Blocking Logic:** Both the Clock update and the Blink logic are now executed in the non-blocking `while(1)` loop, triggered by their respective flags.
* **Simplified Interrupt:** The `HAL_TIM_PeriodElapsedCallback` is simplified, only retaining the most critical logic: **Software Timer management** and **LED Scanning**.

| Timer Index | Duration | Function | Controlled by |
| :--- | :--- | :--- | :--- |
| **Timer 0** | $1000\text{ms}$ | Clock Update (HH:MM) | `while(1)` |
| **Timer 1** | $1000\text{ms}$ | DOT/LED Red Blink | `while(1)` |

---

## 2. Source Code Implementation

The following code sections demonstrate the implementation of the multi-timer array structure and the logic splitting between the interrupt handler and the main loop.

### Global Variables Update (Multi-Timer Structure)

The global variables are updated to support an array of timers, and the redundant `blink_counter` is removed.

```c
/* USER CODE BEGIN PV */
#define NO_OF_TIMERS 2
int timer_counter[NO_OF_TIMERS] = {0};
int timer_flag[NO_OF_TIMERS] = {0};
int TIMER_CYCLE = 10;  
int hour = 15, minute = 8, second = 50;
int led_buffer[MAX_LED] = {1, 5, 0, 8}; 
int scan_counter = 25; 
int index_led = 0;
```
### Timer Helper Functions (Updated for Array)
```c
/* USER CODE BEGIN 0 */
void setTimer (int index, int duration) {
    if (index >= 0 && index < NO_OF_TIMERS) {
        timer_counter[index] = duration / TIMER_CYCLE;
        timer_flag[index] = 0;
    }
}

void timer_run() {
    for (int i = 0; i < NO_OF_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--; 
            if (timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}
/* USER CODE END 0 */
```
### Updated Interrupt Callback (Blink Logic Removed) 
The code handling `blink_counter` and `HAL_GPIO_TogglePin` is removed, simplifying the interrupt handler.
```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    timer_run();  
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 25; 
        index_led = (index_led + 1) % MAX_LED;
        update7SEG(index_led);
    }
}
/* USER CODE END 4 */
```
### Main Loop
```c
/* USER CODE BEGIN 2 */
setTimer(0, 1000); 
setTimer(1, 1000); 
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
    // --- Task 1: Clock Update Logic (Controlled by Timer 0) ---
    if (timer_flag[0] == 1) {
        second++; 
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }
        if (hour >= 24) {
            hour = 0;
        }
        updateClockBuffer();
        setTimer(0, 1000);  
    }

    if (timer_flag[1] == 1) {
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        setTimer(1, 1000); // Reset Timer 1 for the next 1s toggle
    }
}
/* USER CODE END WHILE */

const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin};
/* USER CODE END PV */
