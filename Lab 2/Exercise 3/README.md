# Exercise 3: Code Refactoring and Function Encapsulation

## 1. Requirements

The goal of this exercise is to implement a code refactoring strategy by encapsulating the 7-segment LED scanning logic into a dedicated, reusable function named **`update7SEG(int index)`**.

This function is responsible for:

* Receiving the current scan index (0 to 3).

* Reading the corresponding display value from the global **`seg_data[4]`** array.

* Turning OFF all 4 Enable pins.

* Setting the segment pins (PB0-PB6) to display the correct digit.

* Turning ON only the specific Enable pin corresponding to the index.

This standardizes the code, making the **`HAL_TIM_PeriodElapsedCallback`** function cleaner and more focused solely on time-based counting.

| Function | Responsibility |
| :--- | :--- | 
| update7SEG(int index) | Handles the entire display logic (data and enable control).
| HAL_TIM_PeriodElapsedCallback | Handles time counting and calling update7SEG once per scan cycle.

## 2. Project Files

This folder contains the following project files:
* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`) with 4 7-segment LEDs, 4 PNP transistors, and 2 indicator LEDs (DOT and LED Red).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the LED scanning and blinking logic.

---


## 3. Source Code Implementation
**New Function: update7SEG(int index) `/* USER CODE BEGIN 0 */`**

This function encapsulates the scanning and display process for the four LED digits (12:30).
```c
/* USER CODE BEGIN 0 */
void update7SEG(int index) {
    // 1. Tắt TẤT CẢ các chân Enable (E1, E2, E3, E4) để tránh "Ghosting"
    // Giả định chân Enable nằm trên GPIOA và là PNP (HIGH để tắt)
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin | E3_Pin | E4_Pin, GPIO_PIN_SET);

    // 2. Gửi dữ liệu số tương ứng với vị trí index
    // Gọi hàm mã hóa 7 đoạn với giá trị từ mảng seg_data
    display7SEG(seg_data[index % 4]); 
    
    // 3. Bật chân Enable tương ứng với vị trí index (LOW để bật PNP)
    HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[index % 4], GPIO_PIN_RESET);
}

/* USER CODE END 0 */
```
**Updated Interrupt Callback `/* USER CODE BEGIN 4 */`**

The callback is simplified to manage counters and call the new function.
```c
/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 50; 
        led_index = (led_index + 1) % 4; 
        update7SEG(led_index); 
    }
    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; 
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
/* USER CODE END 4 */
