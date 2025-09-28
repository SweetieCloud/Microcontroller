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
| **Segments (a-g)** | Tín hiệu hiển thị số | **PB0 - PB6** |
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
