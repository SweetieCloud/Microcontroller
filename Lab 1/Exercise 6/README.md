# Exercise 6: Analog Clock with 12 LEDs

## 1. Requirements

This exercise requires you to design and program a circuit to simulate an analog clock using 12 LEDs.
* Connect **12 LEDs** to the **GPIOA pins** of the **STM32F103C6** microcontroller, specifically from **PA4** to **PA15**.
* Arrange the 12 LEDs in a circular pattern to represent the positions on a clock face.
* Implement a simple program to test the connections by turning on each LED sequentially.

The testing program should:
* Activate each LED in order.
* Each LED will light for 1 second before turning off and moving on to the next.

## 2. Project Files

This folder contains the following project files: 
* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the four-way traffic light logic.

## 3. Schematic

The following schematic shows the circuit connections for this exercise, including both 7-segment displays.

## 4. Source Code

The following code is implemented in `main.c`, using a counter to manage the timing as required.

**main** `while(1)` **Loop**
```c
/* USER CODE BEGIN WHILE */
int counter = 0;
while (1)
{
  HAL_GPIO_WritePin(GPIOA, 0xFFF0, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, (1 << (4 + counter)), GPIO_PIN_RESET);

  HAL_Delay(1000);
  counter++;
  if(counter > 11) {
    counter = 0;
  }
}
/* USER CODE END WHILE */
