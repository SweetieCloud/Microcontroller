# Exercise 3: Four-Way Traffic Light 

## 1. Requirements

This exercise focuses on simulating a four-way traffic light system using multiple LEDs.
* Connect 12 LEDs to the GPIOA pins of the STM32F103C6 microcontroller to represent the lights for four intersections.
* The lights for each pair of opposing intersections will operate in sync.

The traffic light sequence is as follows:
* **RED** light: **5 seconds**
* **GREEN** light: **3 seconds**
* **YELLOW** light: **2 seconds**

## 2. Project Files

This folder contains the following project files: 
* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the four-way traffic light logic.

## 3. Schematic

The following schematic shows the circuit connections for this exercise. 

## 4. Source Code

The following code is implemented inside the `while(1)` loop in `main.c`, using a counter to manage the timing as required.

```c
/* USER CODE BEGIN WHILE */
int counter = 0;
while (1)
{
	      HAL_GPIO_WritePin(GPIOA, LED_REDA1_Pin | LED_REDA7_Pin | LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_RESET);
	      HAL_GPIO_WritePin(GPIOA, LED_REDA4_Pin | LED_REDA10_Pin | LED_YELLOWA2_Pin | LED_YELLOWA8_Pin | LED_GREENA3_Pin | LED_GREENA9_Pin | LED_YELLOWA5_Pin | LED_YELLOWA11_Pin, GPIO_PIN_SET);

	      counter = 3;
	      while(counter > 0){
	          HAL_Delay(1000);
	          counter--;
	      }

	      HAL_GPIO_WritePin(GPIOA, LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOA, LED_YELLOWA5_Pin | LED_YELLOWA11_Pin, GPIO_PIN_RESET);

	      counter = 2;
	      while(counter > 0){
	          HAL_Delay(1000);
	          counter--;
	      }

	      HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_YELLOWA8_Pin | LED_REDA1_Pin | LED_REDA7_Pin | LED_YELLOWA5_Pin | LED_YELLOWA11_Pin |LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOA, LED_REDA4_Pin | LED_REDA10_Pin | LED_GREENA3_Pin | LED_GREENA9_Pin, GPIO_PIN_RESET);

	      counter = 3;
	      while(counter > 0){
	          HAL_Delay(1000);
	          counter--;
	      }

	      HAL_GPIO_WritePin(GPIOA, LED_GREENA3_Pin | LED_GREENA9_Pin, GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_YELLOWA8_Pin, GPIO_PIN_RESET);

	      counter = 2;
	      while(counter > 0){
	          HAL_Delay(1000);
	          counter--;
	      }
}
/* USER CODE END WHILE */
