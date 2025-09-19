# Exercise 8: _setNumberOnClock()_ Function

## 1. Requirements

This exercise requires you to create a function that turns on a single LED on the analog clock based on a numerical input.

Implement a function named **_setNumberOnClock(int num)_**.

The function takes an integer num as input, with a value from 0 to 11.

The function should turn on the single LED corresponding to the input number.

## 2. Source Code

The following code is implemented in `main.c`, using a bit-shifting technique to select the correct GPIO pin.

_**setNumberOnClock()**_ **Function**
```c
/* USER CODE BEGIN 0 */
void setNumberOnClock(int num)
{
    // Kiểm tra đầu vào hợp lệ
    if (num >= 0 && num <= 11) {
        // Tạo mask bit để chọn đúng chân GPIO cần bật.
        // Chân PA4 tương ứng với num = 0
        // Chân PA15 tương ứng với num = 11
        uint16_t pin_to_set = (1 << (4 + num));
        HAL_GPIO_WritePin(GPIOA, pin_to_set, GPIO_PIN_SET);
    }
}
/* USER CODE END 0 */
