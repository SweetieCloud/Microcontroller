# Exercise 1: Two 7-Segment LED Scanning.

## 1. Requirements

The goal of this exercise is to design and implement a circuit and its corresponding source code to control two common anode 7-segment LEDs using a timer interrupt. The primary objective is to display the number **"1"** on the first LED and **"2"** on the second LED, with a switching time of **500ms** (half a second). The frequency of the scanning process is **1 Hz**.

## 2. Project Files

This folder contains the following project files:

* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the blinking logic.

## 3. Schematic

The schematic is implemented in Proteus, as shown in the provided image. The following components are used in the schematic:

* STM32F103C6

* 7SEG-COM-ANODE

* PNP

* RES

* LED-RED

The two 7-segment LEDs are connected to the microcontroller as follows:

* Signal Pins: The individual cathode pins are connected to pins **PB0** through **PB6** of the **STM32F103C6**.

* Enable Pins: The common anode pins of each LED are controlled by **PNP transistors**, which are driven by pins **PA6** and **PA7**. 

## 4. Source Code
The solution is implemented in the main.c file. The logic for displaying the numbers and switching between the LEDs is handled within the  `void display7SEG(int num)` and `HAL_TIM_PeriodElapsedCallback` functions.
```c
int counter_led = 50;
int led_index = 0;
void display7SEG(int num) {
    HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
    switch (num) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|d_Pin|e_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin, GPIO_PIN_SET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 9:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  counter_led--;
  if (counter_led <= 0) {
    counter_led = 50;
    led_index = !led_index;
    HAL_GPIO_WritePin(GPIOA, E1_Pin | E2_Pin, GPIO_PIN_SET);
    if (led_index == 0) {
      display7SEG(1);
      HAL_GPIO_WritePin(GPIOA, E1_Pin, GPIO_PIN_RESET);
    } else {
      display7SEG(2);
      HAL_GPIO_WritePin(GPIOA, E2_Pin, GPIO_PIN_RESET);
    }
  }
}
/* USER CODE END 4 */
int main(void)
{
  HAL_Init();
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  MX_GPIO_Init();
  MX_TIM2_Init();
  HAL_TIM_Base_Start_IT (& htim2 );
  /* USER CODE BEGIN WHILE */
  while (1)
  {}
  /* USER CODE END WHILE */
}
```
## 5. Short Question Answer

**Short question: What is the frequency of the scanning process?**

The frequency of the scanning process ($f$) is the reciprocal of the total time required to display data on all LEDs ($T_{total}$).

1.  **Timer Interrupt Period ($T_{ngắt}$):** $10ms$ (from $8MHz / (7999+1) / (9+1)$).
2.  **Switching Time ($T_S$):** The time each individual LED is enabled is 50 interrupt cycles, as `counter_led` is set to 50:
    $$T_S = 50 \times 10ms = 500ms \text{ hay } 0.5s \text{.}$$
3.  **Total Scanning Period ($T_{total}$):** Since there are 2 7-segment LEDs:
    $$T_{total} = 2 \times T_S = 2 \times 0.5s = 1s \text{.}$$
4.  **Scanning Frequency ($f$):**
    $$f = 1 / T_{total} = 1 / 1s = 1Hz \text{.}$$

[cite_start]Answer: The frequency of the scanning process is **1Hz** because each LED is turned ON for 0.5s, and the total time to scan both LEDs is 1s[cite: 297, 298].
