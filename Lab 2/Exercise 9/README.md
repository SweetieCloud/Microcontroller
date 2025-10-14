# Exercise 9: Integrated LED Matrix & Scanning Optimization

## 1. Requirements

The objective of this exercise is to **integrate an 8x8 LED Matrix** into the system and optimize the scanning process for both the four 7-segment displays and the LED matrix. All application logic (Clock, Blink, 7-SEG scan, and Matrix scan) is fully moved into the **`while(1)`** loop, ensuring the Interrupt Handler remains maximally optimized, dedicated only to time management.

* **LED Matrix Integration:** The system includes an 8x8 LED Matrix (using the ULN2803 IC) and corresponding control pins.
* **Extended Software Timers:** The system utilizes **four** independent Software Timers to manage tasks with different periodic cycles.
* **Optimal Interrupt:** The Interrupt Service Routine (`HAL_TIM_PeriodElapsedCallback`) retains only one core task: **managing the software timers** (calling `timer_run()`).
* **Character Display:** The system displays the character **'A'** on the LED Matrix.

---

### Final Task Allocation

| Timer Index | Timer Name | Duration (Activation Period) | Frequency | Task Executed in **`while(1)`** |
| :--- | :--- | :--- | :--- | :--- |
| **Timer 0** | Clock | `1000ms` | `1Hz` | **Clock Logic** (Update second, minute, hour) |
| **Timer 1** | Blink | `1000ms` | `1Hz` | **Blink Logic** (Toggle DOT/LED Red) |
| **Timer 2** | 7-SEG Scan | `250ms` | `4Hz` | **LED Scanning 7-SEG** (`update7SEG`) |
| **Timer 3** | Matrix Scan | **`10ms`** | **`100Hz`** | **LED Scanning Matrix** (`updateLEDMatrix`) |

---

## 2. Source Code Implementation

### HAL\_TIM\_PeriodElapsedCallback (Optimized Interrupt Handler)

The interrupt handler is fully optimized, containing only the single function call necessary to update all Software Timers.

```c
/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    timer_run();
}
/* USER CODE END 4 */
```
### `Main Loop (while(1)`

All application logic, including clock updates and display scanning, is executed in the main loop using the Software Timer flags, ensuring non-blocking operation.
```c
/* USER CODE BEGIN 2 */
// Initial Timer Setup
HAL_TIM_Base_Start_IT(&htim2);
updateClockBuffer();
update7SEG(0);
updateLEDMatrix(0);
setTimer0(1000); // Clock (1s)
setTimer1(1000); // Blink (1s)
setTimer2(250);  // 7-SEG Scan (250ms per LED)
setTimer3(10);   // Matrix Scan (10ms per column)
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1){
    if(timer0_flag == 1){
        second++;
        if(second >= 60){ second = 0; minute++; }
        if(minute >= 60){ minute = 0; hour++; }
        if(hour >= 24) hour = 0;
        updateClockBuffer();
        setTimer0(1000);
    }

    if(timer1_flag == 1){
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
        setTimer1(1000);
    }

    // Timer 2: 7-SEG Scanning Logic (250ms)
    if(timer2_flag == 1){
        index_led = (index_led + 1) % MAX_LED;
        update7SEG(index_led);
        setTimer2(250);
    }

    if(timer3_flag == 1){
        index_led_matrix = (index_led_matrix + 1) % MAX_LED_MATRIX;
        updateLEDMatrix(index_led_matrix);
        setTimer3(10);
    }
}
/* USER CODE END WHILE */
```
### Function `updateLEDMatrix(int index)`

The scanning function displays the data `matrix_buffer[index]` onto the current column `index`
```c
void updateLEDMatrix(int index){
    HAL_GPIO_WritePin(GPIOA, Q0_Pin|Q1_Pin|Q2_Pin|Q3_Pin|Q4_Pin|Q5_Pin|Q6_Pin|Q7_Pin, GPIO_PIN_SET);

    uint8_t column_data = matrix_buffer[index];

    for (int i = 0; i < 8; i++) {
        // R_PINS is an array holding the pin definitions
        if ((column_data >> i) & 0x01) {
            HAL_GPIO_WritePin(GPIOB, R_PINS[i], GPIO_PIN_SET); // LED ON
        } else {
            HAL_GPIO_WritePin(GPIOB, R_PINS[i], GPIO_PIN_RESET); // LED OFF
        }
    }
    HAL_GPIO_WritePin(GPIOA, Q_PINS[index], GPIO_PIN_RESET);
}
```
