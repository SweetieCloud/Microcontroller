# Exercise 8: Full Interrupt Optimization (Maximized Responsiveness)

## 1. Requirements

The goal of this exercise is to finalize the interrupt optimization process by moving the last remaining application logic—**LED Scanning**—out of the `HAL_TIM_PeriodElapsedCallback` function.

* **Software Timer Expansion:** The system uses $\mathbf{three}$ independent Software Timers: $\mathbf{Timer 0}$ (Clock), $\mathbf{Timer 1}$ (Blink), and $\mathbf{Timer 2}$ (Scan).
* **Optimal Interrupt:** The interrupt handler is reduced to its single core purpose: **managing the software timers** (calling `timer_run()`).
* **Full Non-Blocking Operation:** All three application tasks (Clock, Blink, and Scan) are executed in the $\mathbf{while(1)}$ loop, utilizing the `timer_flag[index]` mechanism.

### Final Task Allocation

| Timer Index | Duration | Frequency | Task Executed in $\mathbf{while(1)}$ |
| :--- | :--- | :--- | :--- |
| **Timer 0** | $1000\text{ms}$ | $1\text{Hz}$ | **Clock Logic** (Update second, minute, hour) |
| **Timer 1** | $1000\text{ms}$ | $1\text{Hz}$ | **Blink Logic** (Toggle DOT/LED Red) |
| **Timer 2** | $250\text{ms}$ | $4\text{Hz}$ | **LED Scanning** (`update7SEG`) |

---

## 2. Source Code Implementation

### HAL_TIM_PeriodElapsedCallback (Optimized Interrupt Handler)

The interrupt handler is now fully optimized and only retains the single function call necessary to maintain time tracking for all Software Timers.

```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    timer_run(); 
    // Scan and Blink logic have been fully moved to the main loop.
}
/* USER CODE END 4 */
```
### Main Loop `(while(1)`
```c
/* USER CODE BEGIN 2 */
setTimer(0, 1000); 
setTimer(1, 1000); 
setTimer(2, 250); 
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1){
    if (timer_flag[0] == 1) {
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

        updateClockBuffer ();
        setTimer (0, 1000);
    }
    if (timer_flag[1] == 1) {
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        setTimer (1, 1000);
    }
    // The update7SEG() function is now executed in the main loop every 250ms
    if (timer_flag[2] == 1) {
        index_led = (index_led + 1) % MAX_LED; 
        update7SEG(index_led);
        setTimer (2, 250);
    }
}
/* USER CODE END WHILE */
