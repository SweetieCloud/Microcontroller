Exercise 6: Analog Clock with 12 LEDs
1. Requirements
This exercise requires you to design and program a circuit to simulate an analog clock using 12 LEDs.

Connect 12 LEDs to the GPIOA pins of the STM32F103C6 microcontroller, specifically from PA4 to PA15.

Arrange the 12 LEDs in a circular pattern to represent the positions on a clock face.

Implement a simple program to test the connections by turning on each LED sequentially.

The testing program should:

Bật từng đèn LED một theo thứ tự từ 1 giờ đến 12 giờ.

Mỗi đèn LED sẽ sáng trong 1 giây trước khi tắt và chuyển sang đèn tiếp theo.

2. Project Files
This folder contains the following project files:

Proteus_Circuit: Contains the Proteus simulation file (.pdsprj).

Source_Code: Contains the C code from STM32CubeIDE.

main.c: The main source file with the logic for testing the LED connections.

3. Schematic
The following schematic shows the circuit connections for this exercise, including the 12 LEDs arranged in a clock pattern.

4. Source Code
The following code is implemented in main.c, using a counter to manage the sequential lighting of the LEDs as required.

Main while(1) Loop
C

/* USER CODE BEGIN WHILE */
int counter = 0;
while (1)
{
  // Tắt tất cả các đèn LED trước khi bật đèn mới
  // Các chân từ PA4 đến PA15 tương ứng với 12 đèn LED
  HAL_GPIO_WritePin(GPIOA, 0xFFF0, GPIO_PIN_RESET);

  // Bật đèn LED thứ "counter"
  // (1 << (4 + counter)) sẽ tạo ra một mask bit để bật đúng chân GPIOA tương ứng
  HAL_GPIO_WritePin(GPIOA, (1 << (4 + counter)), GPIO_PIN_SET);

  // Chờ 1 giây
  HAL_Delay(1000);

  // Tăng biến đếm và reset về 0 khi đã đi hết 12 đèn
  counter++;
  if(counter > 11) {
    counter = 0;
  }
}
/* USER CODE END WHILE */
