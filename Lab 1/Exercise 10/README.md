# Exercise 10: Full Analog Clock Simulation

## 1. Requirements
This exercise is the culmination of the previous exercises, combining the functions you've created to simulate a full analog clock with three moving hands: a second hand, a minute hand, and an hour hand.
Use the functions **_clearAllClock()_, _setNumberOnClock()_, and _clearNumberOnClock()_** from previous exercises.



## 2. Project Files
This folder contains the following project files:
* **Proteus_Circuit**: Contains the Proteus simulation file (`.pdsprj`).
* **Source_Code**: Contains the C code from STM32CubeIDE.
                 * `main.c`:  The main source file with the logic to run the clock.

## 3. Schematic
The schematic for this exercise remains the same as in Exercise 6, 7, 8, and 9.

## 4. Source Code
The following code is implemented in main.c, using a counter-based logic to manage the timing and movement of all three hands.

Main while(1) Loop
```c
/* USER CODE BEGIN WHILE */
  // Khởi tạo vị trí ban đầu của 3 kim
  // Vị trí 0 tương ứng với đèn số 1 (PA4), vị trí 11 tương ứng với đèn số 12 (PA15)
  int second_counter = 0; 
  int minute_counter = 11;  
  int hour_counter = 5;    

  while (1)
  {
    // Bước 1: Xóa vị trí cũ của tất cả các kim để chuẩn bị vẽ lại
    clearAllClock();
    
    // Bước 2: Bật 3 đèn tương ứng với vị trí hiện tại của 3 kim
    setNumberOnClock(second_counter);
    setNumberOnClock(minute_counter);
    setNumberOnClock(hour_counter);

    HAL_Delay(1000);

    // Cập nhật kim giây sau mỗi giây
    second_counter++;
    if (second_counter >= 12) {
        second_counter = 0; // Reset về 0 sau khi quay hết một vòng (12 lần nhảy)
        
        // Khi kim giây quay hết 1 vòng, cập nhật kim phút
        minute_counter++;
        if (minute_counter >= 12) {
            minute_counter = 0; // Reset sau khi quay hết một vòng (12 lần nhảy)
            
            // Khi kim phút quay hết 1 vòng, cập nhật kim giờ
            hour_counter++;
            if (hour_counter >= 12) {
                hour_counter = 0; // Reset sau khi quay hết một vòng (12 lần nhảy)
            }
        }
    }
  }
/* USER CODE END WHILE */
