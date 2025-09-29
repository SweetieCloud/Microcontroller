# Exercise 6: Software Timer for Non-Blocking Delay 

## 1. Requirements
The goal of this exercise is to replace the blocking function `HAL_Delay(1000)` in the main loop with a **Software Timer mechanism**.  
This technique reduces the processing burden on the main loop and enhances the system's responsiveness by achieving **non-blocking delay**.

- **Implementation**: Implement the software timer logic (variables and functions) as specified in *Program 1.6*.  
- **Integration**: The `timer_run()` function must be invoked every **10ms** within the `HAL_TIM_PeriodElapsedCallback` handler.  
- **Clock Synchronization**: The clock logic (increasing hour, minute, second, and calling `updateClockBuffer()`) is now triggered only when the `timer0_flag` is set in the `while(1)` loop.  
- **Non-Blocking**: The main loop can now execute other tasks while waiting for the 1-second tick, without being halted by a delay function.  

### Function Responsibilities

| Function | Responsibility |
|----------|----------------|
| `setTimer0(int duration)` | Initializes the counter based on the required delay (in ms). |
| `timer_run()` | Decrements the counter every 10ms (the interrupt cycle). |
| `HAL_TIM_PeriodElapsedCallback` | Manages display scanning and calls `timer_run()`. |
| `main → while(1)` | Checks `timer0_flag` to execute 1s clock logic, then resets the timer. |

---

## 2. Project Files
- **Proteus_Circuit**: Continues to use the 4 LED 7-segment setup with DOT LED and Red LED.  
- **Source_Code**: STM32CubeIDE C source code.  
  - `main.c`: Integrated the Software Timer mechanism to manage the 1-second clock tick.  

---

## 3. Source Code Implementation

### Software Timer Structure `/* USER CODE BEGIN 0 */`

The timer variables and helper functions are defined outside the main loop, managed by the Timer Interrupt.

```c
/* USER CODE BEGIN 0 */
int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 10; 

void setTimer0 (int duration) {
    timer0_counter = duration / TIMER_CYCLE; 
    timer0_flag = 0;
}

void timer_run() {
    if (timer0_counter > 0) {
        timer0_counter--; 
        if (timer0_counter == 0) timer0_flag = 1;
    }
}
/* USER CODE END 0 */
```
**Updated Interrupt Callback**

The `timer_run()` function is called at the highest frequency (every 10ms) to ensure accurate non-blocking counting.
```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    timer_run(); 
}
/* USER CODE END 4 */
```
**Updated Main Loop**

The blocking `HAL_Delay` is removed. The clock logic now executes only when the 1s flag is raised, then immediately schedules the next 1s tick.

```c
/* USER CODE BEGIN WHILE */
setTimer0(1000); 
while (1)
{
    if (timer0_flag == 1) {
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
        setTimer0(1000); 
    }
}
/* USER CODE END WHILE */
```
