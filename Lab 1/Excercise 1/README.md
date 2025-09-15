# Exercise 1: Two-LED Blinking

## 1. Requirements

This exercise extends the basic blinking LED project by making both LEDs blink alternately.

* Connect **LED-RED** to pin **PA5** and **LED-YELLOW** to pin **PA6** of the **STM32F103C6** microcontroller.
* Two LEDs should switch their states every 2 seconds.

## 2. Project Files

This folder contains the following project files:

* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the blinking logic.

## 3. Schematic

The following schematic shows the circuit connections for this exercise. 

## 4. Source Code

The following code is implemented inside the `while(1)` loop in `main.c`, using a counter to manage the timing as required.

```c
/* USER CODE BEGIN WHILE */
int counter = 0;
while (1)
{
    if (counter == 0)
    {
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
    }
    else if (counter == 2)
    {
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
    }
    HAL_Delay(1000);
    counter++;

    // Reset counter
    if (counter >= 4)
    {
        counter = 0;
    }
}
/* USER CODE END WHILE */
