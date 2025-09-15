# Exercise 2: Traffic Light Simulation

## 1. Requirements

This exercise aims to simulate a traffic light using three LEDs with specific timings.

* A **LED-GREEN** is added to the circuit and connected to pin **PA7** of the STM32F103C6 microcontroller.
* The LEDs (RED on PA5, YELLOW on PA6, and GREEN on PA7) will follow a specific blinking sequence and duration to simulate a traffic light cycle.
* The allowed delay function is **HAL_Delay(1000)**, with no other `HAL_Delay` functions permitted.

The traffic light cycle is as follows:
* **RED** light: **5 seconds**
* **GREEN** light: **3 seconds**
* **YELLOW** light: **2 seconds**

## 2. Project Files

This folder contains the following project files:

* **Proteus_Circuit/**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code/**: Contains the C code from STM32CubeIDE.
    * `main.c`: The main source file with the traffic light logic.

## 3. Schematic

The following schematic shows the circuit connections for this exercise. The green LED is connected to pin PA7 of the STM32F103C6.

!

_The caption of this figure is a downloadable link to the Proteus project file._

## 4. Source Code

The following code is implemented inside the `while(1)` loop in `main.c`, using a counter to manage the timing as required.

```c
/* USER CODE BEGIN WHILE */
int counter = 0; // Initialize the counter variable

while (1)
{
    if (counter == 0)
    {
        // Red light stage: Turn on the RED light and turn off others.
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
    }
    else if (counter == 5)
    {
        // Green light stage: Turn on the GREEN light and turn off others.
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
    }
    else if (counter == 8)
    {
        // Yellow light stage: Turn on the YELLOW light and turn off others.
        HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
    }

    // Increment the counter and create a 1-second delay
    HAL_Delay(1000);
    counter++;

    // Reset the counter at the end of the 10-second cycle (5s + 3s + 2s)
    if (counter >= 10)
    {
        counter = 0;
    }
}
/* USER CODE END WHILE */
