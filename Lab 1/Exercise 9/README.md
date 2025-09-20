# Exercise 9: Sequential LED Turn-Off

## 1. Requirements
This exercise is the inverse of Exercise 8. You are required to implement a program that turns off the 12 LEDs on the analog clock circuit one by one.
The program should begin with all **12 LEDs** on.
The LEDs should then turn off sequentially.

## 2. Source Code
The following code is implemented in `main.c`, using a for loop to manage the sequential turn-off of the LEDs.
**main** `while(1)` **Loop**
C

/* USER CODE BEGIN WHILE */
  while (1)
  {
    // Bật tất cả 12 đèn LED cùng lúc
    HAL_GPIO_WritePin(GPIOA, 0xFFF0, GPIO_PIN_SET);
    
    // Dừng 1 giây để người dùng thấy tất cả các đèn đang sáng
    HAL_Delay(1000);

    // Vòng lặp để lần lượt tắt từng đèn, từ đèn cuối cùng về đèn đầu tiên
    for (int counter = 11; counter >= 0; counter--)
    {
        // Gọi hàm clearNumberOnClock() để tắt đèn hiện tại
        clearNumberOnClock(counter);
        HAL_Delay(1000);
    }
    // Sau khi tất cả 12 đèn đã tắt, chương trình sẽ dừng lại tại đây
    // để giữ nguyên trạng thái không có đèn nào sáng.
    while(1){}
  }
/* USER CODE END WHILE */
