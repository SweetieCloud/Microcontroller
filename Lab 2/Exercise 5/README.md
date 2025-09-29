# Exercise 5: Digital Clock (HH:MM) Implementation 

## 1. Requirements
The goal of this exercise is to implement a **24-hour digital clock** displaying **Hour** and **Minute** in the `HH:MM` format on the **4 LED 7-segment display**.  

- **Time Logic**: The clock's time logic (second → minute → hour rollover) must be implemented within the `main` function's `while(1)` loop.  
- **Synchronization**: Time advancement is synchronized to **1 second** using the blocking function `HAL_Delay(1000)`.  
- **Data Conversion**: A new function `updateClockBuffer()` must be implemented to convert the integer values of `hour` and `minute` into the 4 individual digits required for the LED display buffer (`H1, H2, M1, M2`). This ensures single-digit values (e.g., `8`) are correctly displayed with a leading zero (e.g., `08`).  
- **Display Scanning**: The display scanning mechanism configured in **Exercise 4** (`T_S = 250ms`, `T_total = 1s`) must remain active in the **Timer Interrupt** to provide a flicker-free display.  

### Function Responsibilities

| Function | Responsibility |
|----------|----------------|
| `updateClockBuffer()` | Converts `hour/minute` integers to 4 digits in `led_buffer`. |
| `main → while(1)` | Implements 1-second time advancement logic and calls `updateClockBuffer`. |
| `HAL_TIM_PeriodElapsedCallback` | Handles 10ms timing, 250ms scanning, and 1s DOT blink. |

---

## 2. Project Files
- **Proteus_Circuit**: Same as Exercise 4 (4 LED 7-segment, DOT LED, LED Red).  
- **Source_Code**: STM32CubeIDE C source code.  
  - `main.c`: Updated with the clock logic, new global variables, and the `updateClockBuffer` function.  

---

## 3. Source Code Implementation

### Global Variables Update `/* USER CODE BEGIN PV */`

```c
/* USER CODE BEGIN PV */
int hour = 15, minute = 8, second = 50; 
int led_buffer[MAX_LED] = {1, 5, 0, 8}; 
int scan_counter = 25;     
int blink_counter = 100;   
int index_led = 0;
/* USER CODE END PV */
```
**Function Prototypes Update**
```c
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(void); // Hàm mới
/* USER CODE END PFP */
```
**New Function** `updateClockBuffer()`
```c
/* USER CODE BEGIN 0 */
void updateClockBuffer(void) {
    led_buffer[0] = hour / 10; 
    led_buffer[1] = hour % 10; 
    led_buffer[2] = minute / 10; 
    led_buffer[3] = minute % 10;
}
/* USER CODE END 0 */
```
**Updated Main Loop**
```c
/* USER CODE BEGIN WHILE */
while (1)
{
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
    HAL_Delay(1000);
}
/* USER CODE END WHILE */

