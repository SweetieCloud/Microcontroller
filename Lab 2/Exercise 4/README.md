# Exercise 4: Adjusting Scanning Frequency to 1 Hz

## 1. Requirements
The goal of this exercise is to adjust the Timer configuration to change the total scanning frequency (**f_total**) of the four 7-segment LEDs to **1 Hz**.  

- The four 7-segment LEDs must be scanned at a total frequency of 1 Hz.  
- The indicator LEDs (DOT and LED Red) must maintain their independent blinking cycle of **1 second**.  

This requires calculating the new switching time (**T_S**) per LED and updating the `scan_counter` value accordingly.

---

## 2. Analysis and Calculation
The **Timer 2 interrupt period** remains fixed at **10 ms**.

| Parameter                   | Calculation       | New Value |
|------------------------------|------------------|-----------|
| Required Total Period (T_total) | 1 / 1Hz           | 1000 ms   |
| Switching Time per LED (T_S) | T_total / 4 LEDs  | 250 ms    |
| New Scan Counter Value       | 250 ms / 10 ms    | 25        |
| DOT Blink Counter            | 1000 ms / 10 ms   | 100       |

---

## 3. Project Files
The implementation builds upon the structure of **Exercise 3**.  
No changes to the **Proteus circuit** are required.  

* **Proteus_Circuit**: Circuit remains the same as Exercise 2/3.  
* **Source_Code**:  
  * `main.c`: Updated with the new `scan_counter` value.  

---

## 4. Source Code Implementation

**Global Variables Update `/* USER CODE BEGIN PV */`**

The `scan_counter` is changed **from 50 → 25** to reduce the switching time per LED from **500 ms → 250 ms**.  

```c
/* USER CODE BEGIN PV */
int seg_data[4] = {1, 2, 3, 4}; 
int scan_counter = 25;          
int blink_counter = 100;        
int led_index = 0;              
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin}; 
/* USER CODE END PV */
```
**The HAL_TIM_PeriodElapsedCallback function `/* USER CODE BEGIN 4 */`**
```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // --- Logic 1: Điều khiển Quét LED (Tần số tổng 1Hz) ---
    scan_counter--;
    if (scan_counter <= 0) {
        // Đặt lại bộ đếm cho 250ms (T_S)
        scan_counter = 25; 

        // 1. Tăng index và gọi hàm quét (từ Ex 3)
        led_index = (led_index + 1) % 4; 
        update7SEG(led_index); 
    }
    
    // --- Logic 2: Nhấp nháy DOT và LED Red (1000ms = 1 giây) ---
    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; 
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
/* USER CODE END 4 */
```

