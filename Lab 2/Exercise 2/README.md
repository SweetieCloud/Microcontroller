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
int seg_data[4] = {1, 2, 3, 0}; 
int scan_counter = 50;        
int blink_counter = 100;       
int led_index = 0;           
/* USER CODE END PV */
```

**HAL_TIM_PeriodElapsedCallback** `/* USER CODE BEGIN 4 */`

```c
/* USER CODE BEGIN 4 */
const uint16_t LED_ENABLE_PINS[] = {E1_Pin, E2_Pin, E3_Pin, E4_Pin}; 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    scan_counter--;
    if (scan_counter <= 0) {
        scan_counter = 50; 

        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_SET); 
        led_index = (led_index + 1) % 4; 
        display7SEG(seg_data[led_index]); 
        HAL_GPIO_WritePin(GPIOA, LED_ENABLE_PINS[led_index], GPIO_PIN_RESET);
    }

    blink_counter--;
    if (blink_counter <= 0) {
        blink_counter = 100; 
        HAL_GPIO_TogglePin(GPIOA, DOT_Pin);
        HAL_GPIO_TogglePin(GPIOA, LED1_Pin);
    }
}
/* USER CODE END 4 */
```
## 5. Short Question Answer

**Short question: What is the frequency of the scanning process?**

The frequency of the scanning process ($f$) is the reciprocal of the total time required to display data on all LEDs ($T_{total}$).

1.  **Timer Interrupt Period:** $10ms$.
2.  **Switching Time ($T_S$):** The time each individual LED is enabled is 50 interrupt cycles, as `counter_led` is set to 50:
    $$T_S = 50 \times 10ms = 500ms \text{ hay } 0.5s \text{.}$$
3.  **Total Scanning Period ($T_{total}$):** Since there are two 7-segment LEDs:
    $$T_{total} = 4 \times T_S = 4 \times 0.5s = 2s \text{.}$$
4.  **Scanning Frequency ($f$):**
    $$f = 1 / T_{total} = 1 / 2s = 0.5Hz \text{.}$$

[cite_start]Answer: The frequency of the scanning process is **0.5Hz**[cite: 297, 298].
