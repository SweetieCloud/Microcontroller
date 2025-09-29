# Exercise 6: Software Timer for Non-Blocking Delay

## 1. Requirements
The goal of this exercise is to replace the blocking function `HAL_Delay(1000)` in the main loop with a **Software Timer mechanism**.  
This technique reduces the processing burden on the main loop and enhances the system's responsiveness by achieving **non-blocking delay**.

- **Implementation**: Implement the software timer logic (variables and functions) as specified in *Program 1.6*.  
- **Integration**: The `timer_run()` function must be invoked every **10ms** within the `HAL_TIM_PeriodElapsedCallback` handler.  
- **Clock Synchronization**: The clock logic (increasing hour, minute, second, and calling `updateClockBuffer()`) is now triggered only when the `timer0_flag` is set in the `while(1)` loop.  
- **Non-Blocking**: The main loop can now execute other tasks while waitingc
/* USER CODE BEGIN WHILE */
setTimer0(1000); 
while (1)
{
    if (timer0_flag == 1) {
        second++; 
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }
        if (hour >= 24) {
            hour = 0;
        }
        updateClockBuffer();
        setTimer0(1000); 
    }
}
/* USER CODE END WHILE */
