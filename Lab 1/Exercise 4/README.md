# Exercise 4: Four-Way Traffic Light with Countdown

## 1. Requirements

This exercise builds upon Exercise 3 by adding a **7-segment display** to show the countdown timer for a selected traffic light.
* Connect **12 LEDs** to the GPIOA pins of the **STM32F103C6** microcontroller to represent the lights for four intersections.
* Connect a 7-segment common-anode display to the GPIOB pins **PB0** through **PB6**.
* The traffic lights for each pair of opposing intersections will operate in sync.

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

**7-Segment Display Function**
```c
/* USER CODE BEGIN 4 */
void display7SEG(int num)
{
    // Common-anode 7-segment display a-g segments connected to PB0-PB6
    uint16_t segments[] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F  // 9
    };
    
    // Invert the segments for common-anode logic (active-low)
    uint16_t invertedSegments = ~segments[num];

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (GPIO_PinState)((invertedSegments >> 0) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState)((invertedSegments >> 1) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)((invertedSegments >> 2) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)((invertedSegments >> 3) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (GPIO_PinState)((invertedSegments >> 4) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (GPIO_PinState)((invertedSegments >> 5) & 1));
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)((invertedSegments >> 6) & 1));
}
/* USER CODE END 4 */
```
**Main `while(1)` Loop**
```c
/* USER CODE BEGIN WHILE */
  while (1)
  {
	  	  HAL_GPIO_WritePin(GPIOA, LED_REDA1_Pin | LED_REDA7_Pin | LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_RESET);
	  	  HAL_GPIO_WritePin(GPIOA, LED_REDA4_Pin | LED_REDA10_Pin | LED_YELLOWA2_Pin | LED_YELLOWA8_Pin | LED_GREENA3_Pin | LED_GREENA9_Pin | LED_YELLOWA5_Pin | LED_YELLOWA11_Pin, GPIO_PIN_SET);

	      int counter = 3;
	      while(counter >= 0){
	          display7SEG(counter);
	          HAL_Delay(1000);
	          counter--;
	      }

	      HAL_GPIO_WritePin(GPIOA, LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOA, LED_YELLOWA5_Pin | LED_YELLOWA11_Pin, GPIO_PIN_RESET);

	      counter = 2;
	      while(counter >= 0){
	          display7SEG(counter);
	          HAL_Delay(1000);
	          counter--;
	      }

	      HAL_GPIO_WritePin(GPIOA, LED_YELLOWA2_Pin | LED_YELLOWA8_Pin | LED_REDA1_Pin | LED_REDA7_Pin | LED_YELLOWA5_Pin | LED_YELLOWA11_Pin |LED_GREENA6_Pin | LED_GREENA12_Pin, GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOA, LED_REDA4_Pin | LED_REDA10_Pin | LED_GREENA3_Pin | LED_GREENA9_Pin, GPIO_PIN_RESET);

	      counter = 5;
	      while(counter >= 0){
	          display7SEG(counter);
	          HAL_Delay(1000);
	          counter--;
	      }
  }
