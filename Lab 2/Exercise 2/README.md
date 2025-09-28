# Exercise 2: Four 7-Segment LED Scanning with DOT Blink

## 1. Requirements

The goal of this exercise is to design and implement a circuit and its corresponding source code to control **four common anode 7-segment LEDs** using a timer interrupt for scanning. Additionally, **two centralized indicator LEDs (DOT and LED Red)** must blink periodically.

The primary objectives are:
* Display the number **"12:30"** across the four 7-segment LEDs (L1=1, L2=2, L3=3, L4=0).
* The **switching time ($T_S$)** for each 7-segment LED must be **500ms** (half a second). Due to the slow switching time, the numbers will appear to flash sequentially.
* The **two indicator LEDs (DOT and LED Red)** must blink independently every **1 second**.

| Parameter | Value | Calculation |
| :--- | :--- | :--- |
| Timer Interrupt Period | $10\text{ms}$ | $(9+1) / 1\text{kHz}$ |
| Scanning Switching Time ($T_S$) | $500\text{ms}$ | $50 \times 10\text{ms}$ |
| Scanning Frequency ($f$) | $0.5\text{Hz}$ | $1 / (4 \times 500\text{ms}) = 1 / 2\text{s}$ |
| DOT/LED Blink Period | $1\text{s}$ | $100 \times 10\text{ms}$ |

---

## 2. Project Files

This folder contains the following project files:
* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`) with 4 7-segment LEDs, 4 PNP transistors, and 2 indicator LEDs (DOT and LED Red).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the LED scanning and blinking logic.

---

## 3. Schematic Overview

The schematic is an expansion of Exercise 1, using four 7-segment LEDs (Common Anode) and two additional indicator LEDs.

| Component | Function | MCU Pin |
| :--- | :--- | :--- |
| **Segments (a-g)** | Digital display signal | **PB0 - PB6** |
| **Enable 1 (E1)** | Scan the 1st 7-segment LED | **PA6** (Transistor PNP) |
| **Enable 2 (E2)** | Scan the 2nd 7-segment LED | **PA7** (Transistor PNP) |
| **Enable 3 (E3)** | Scan the 3rd 7-segment LED | **PA8** (Transistor PNP) |
| **Enable 4 (E4)** | Scan the 4th 7-segment LED | **PA9** (Transistor PNP) |
| **DOT** | Indicator LED 1 (DOT) | **PA4** |
| **LED Red** | Indicator LED 2 (LED1) | **PA5**  |

---

## 4. Source Code

The core logic for controlling the 4-digit display and the indicator LEDs is handled within the `HAL_TIM_PeriodElapsedCallback` function, which is executed every $10\text{ms}$.

**Global Variables** `/* USER CODE BEGIN PV */` 

```c
/* USER CODE BEGIN PV */
// Giá trị hiển thị cố định trên 4 LED: L1=1, L2=2, L3=3, L4=0 (tạo hiệu ứng "12:30")
int seg_data[4] = {1, 2, 3, 0}; 
int scan_counter = 50;          // Bộ đếm 500ms (50 * 10ms) cho việc quét LED 7 đoạn
int blink_counter = 100;        // Bộ đếm 1000ms (100 * 10ms) cho việc nhấp nháy DOT/LED Red
int led_index = 0;              // Chỉ số của LED 7 đoạn đang được quét (0, 1, 2, 3)
/* USER CODE END PV */

**Interrupt Callback** `/* USER CODE BEGIN 4 */`

```c
/* USER CODE BEGIN 4 */

// Mảng chứa các chân Enable (E1 đến E4)
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin}; 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // --- Logic 1: Điều khiển Quét LED (500ms/LED) ---
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 50; // Đặt lại bộ đếm cho 500ms

        // 1. Tắt LED 7 đoạn đang bật (HIGH cho PNP)
        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_SET); 
        
        // 2. Chuyển sang LED 7 đoạn kế tiếp
        led_index = (led_index + 1) % 4; // Chuyển đổi vòng tròn 0 -> 1 -> 2 -> 3 -> 0

        // 3. Hiển thị giá trị mới (1, 2, 3 hoặc 0)
        display7SEG(seg_data[led_index]); 

        // 4. Bật LED 7 đoạn mới (LOW cho PNP)
        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_RESET);
    }
    
    // --- Logic 2: Nhấp nháy DOT và LED Red (1000ms = 1 giây) ---
    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; // Đặt lại bộ đếm cho 1000ms
        
        // Đảo trạng thái của DOT_Pin (PA4)
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        
        // Đảo trạng thái của LED1_Pin (PA5)
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
/* USER CODE END 4 */
