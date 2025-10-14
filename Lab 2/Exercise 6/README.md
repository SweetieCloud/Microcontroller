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

**Timer Structure**

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
## 5. Short Question Answers (Software Timer Analysis)

The following answers analyze the behavior of the system based on changes to the initial `setTimer0()` call, given that the Timer Interrupt period (`TIMER_CYCLE`) is **10ms**.

---

### Report 1: If `setTimer0(1000)` (line 1 of Program 1.8) is missing, what happens after that and why?

If the initial call to `setTimer0(1000)` is missing, the clock logic inside the `while(1)` loop will **never execute**.

**What Happens:**
The variables `timer0_counter` and `timer0_flag` will retain their initial values (typically `0` for global uninitialized variables).

**Why:**
Since `timer0_counter` starts at `0`, the condition

```c
if (timer0_counter > 0)
```

inside the `timer_run()` function will always be **false**. Consequently, `timer0_counter` will never decrement, and `timer0_flag` will never be set to `1`. The main loop's check

```c
if (timer0_flag == 1)
```

will therefore always fail, and the clock update logic (incrementing seconds, minutes, and hours) will be permanently bypassed.

---

### Report 2: If `setTimer0(1000)` is changed to `setTimer0(1)`, what happens after that and why?

If `setTimer0(1)` is used, the system attempts to set the duration to `1ms`, causing the clock to update **almost instantaneously** or **update every 10ms** (if using integer division).

**What Happens:**
The clock will run **100 times faster than real-time**. The `second` variable will update every `10ms`, meaning `100` clock seconds pass for every `1` real second.

**Why:**
The `setTimer0(1)` call calculates the counter value:

```text
timer0_counter = floor(1ms / 10ms) = 0
```

Due to integer division resulting in `0`, the timer flag may be set immediately or in the very next interrupt cycle. Since the flag is set every **10ms** (the interrupt frequency), the clock logic will execute every `10ms`, speeding up the time progression by a factor of 100.

---

### Report 3: If `setTimer0(1000)` is changed to `setTimer0(10)`, what is changed compared to the 2 first questions and why?

Changing the duration to `10ms` results in the clock running **100 times faster** than real-time, which is a predictable, extreme speed-up compared to the first two scenarios.

**What Happens:**
The clock will update its time logic (incrementing the `second` variable) every **10ms**. The display will show time racing by extremely fast.

**Why:**
The `setTimer0(10)` call calculates the counter value:

```text
timer0_counter = 10ms / 10ms = 1
```

The `timer_run()` function will decrement this counter from `1` to `0` after exactly **one interrupt cycle** (`10ms`). Since the timer is reset to `1` immediately after the flag is raised, the entire clock update logic runs every `10ms`, causing the digital clock to speed up by a factor of 100.
