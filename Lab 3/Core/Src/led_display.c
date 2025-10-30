/*
 * led_display.c
 *
 * Created on: Oct 16, 2025
 * Author: BUI THANH QUI
 */

#include "main.h"
#include "led_display.h"
#include "input_reading.h"

// ========================================================
// GLOBAL VARIABLES
// ========================================================

// Trạng thái đèn (cho 2 hướng)
int flagRed[2] = {1, 0};
int flagYellow[2] = {0, 0};
int flagGreen[2] = {0, 1};

// Biến nhấp nháy trong các mode chỉnh sửa
int flagMode_RED_BLINK = 0;
int flagMode_YELLOW_BLINK = 0;
int flagMode_GREEN_BLINK = 0;

// Cờ xác định đang ở mode 1 (chạy bình thường)
int flagInterruptLED_and_SEG = 1;

// Thời gian mỗi màu (ms) - Giá trị mặc định
static int duration_RED_ms = 5000;
static int duration_YELLOW_ms = 2000;
static int duration_GREEN_ms = 3000;

// Biến hiển thị 7 đoạn (giá trị thời gian)
static int numberDisplaySEG[2] = {0, 0};

// Bộ đếm thời gian cho đèn giao thông (Không cần dùng trong code hiện tại, nhưng giữ lại)
static int counter_RED = 0;
static int counter_YELLOW = 0;
static int counter_GREEN = 0;

// ========================================================
// SETTERS & GETTERS
// ========================================================

void setDurationTime_RED_ms(int value) { duration_RED_ms = value; }
int  getDurationTime_RED_ms(void) { return duration_RED_ms; }

void setDurationTime_YELLOW_ms(int value) { duration_YELLOW_ms = value; }
int  getDurationTime_YELLOW_ms(void) { return duration_YELLOW_ms; }

void setDurationTime_GREEN_ms(int value) { duration_GREEN_ms = value; }
int  getDurationTime_GREEN_ms(void) { return duration_GREEN_ms; }

void setNumberDisplaySEG(int index, int value) {
	if (index >= 0 && index < 2) numberDisplaySEG[index] = value;
}
int getNumberDisplaySEG(int index) {
	if (index >= 0 && index < 2) return numberDisplaySEG[index];
	return 0;
}

// ========================================================
// HIỂN THỊ LED MÀU
// ========================================================

void displayLED_RED(int IS_ON, int index){
	switch(index){
	case 0:
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	}
}

void displayLED_YELLOW(int IS_ON, int index){
	switch(index){
	case 0:
		HAL_GPIO_WritePin(Y1_GPIO_Port, Y1_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(Y2_GPIO_Port, Y2_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	}
}

void displayLED_GREEN(int IS_ON, int index){
	switch(index){
	case 0:
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	case 1:
		HAL_GPIO_WritePin(G2_GPIO_Port, G2_Pin, IS_ON ? GPIO_PIN_RESET : GPIO_PIN_SET);
		break;
	}
}

// ========================================================
// HIỂN THỊ LED 7 ĐOẠN (SỬA LỖI LOGIC BCD)
// ========================================================

void displayLED7SEG_LEFT(int number){
	int divPart = number / 10;
	int modPart = number % 10;
	int arr_1[4] = {0}, arr_2[4] = {0};

	// *** SỬA LỖI: Trích xuất bit từ LSB -> MSB (i=0 là LSB, i=3 là MSB) ***
	for(int i = 0; i < 4; ++i){
		arr_1[i] = divPart % 2; // arr_1[0] là LSB
		divPart /= 2;
		arr_2[i] = modPart % 2; // arr_2[0] là LSB
		modPart /= 2;
	}

	// Hàng CHỤC (s00 -> s03)
	// s00_Pin (Chân A/D0) nhận arr_1[0] (LSB)
	HAL_GPIO_WritePin(GPIOA, s00_Pin, arr_1[0]);
	HAL_GPIO_WritePin(GPIOA, s01_Pin, arr_1[1]);
	HAL_GPIO_WritePin(GPIOA, s02_Pin, arr_1[2]);
	// s03_Pin (Chân D/D3) nhận arr_1[3] (MSB)
	HAL_GPIO_WritePin(GPIOA, s03_Pin, arr_1[3]);

	// Hàng ĐƠN VỊ (s10 -> s13)
	// s10_Pin (Chân A/D0) nhận arr_2[0] (LSB)
	HAL_GPIO_WritePin(GPIOB, s10_Pin, arr_2[0]);
	HAL_GPIO_WritePin(GPIOB, s11_Pin, arr_2[1]);
	HAL_GPIO_WritePin(GPIOB, s12_Pin, arr_2[2]);
	// s13_Pin (Chân D/D3) nhận arr_2[3] (MSB)
	HAL_GPIO_WritePin(GPIOB, s13_Pin, arr_2[3]);
}

void displayLED7SEG_TOP(int number){
	int divPart = number / 10;
	int modPart = number % 10;
	int arr_1[4] = {0}, arr_2[4] = {0};

	// *** SỬA LỖI: Trích xuất bit từ LSB -> MSB (i=0 là LSB, i=3 là MSB) ***
	for(int i = 0; i < 4; ++i){
		arr_1[i] = divPart % 2;
		divPart /= 2;
		arr_2[i] = modPart % 2;
		modPart /= 2;
	}

	// Hàng CHỤC (s20 -> s23)
	// s20_Pin (Chân A/D0) nhận arr_1[0] (LSB)
	HAL_GPIO_WritePin(GPIOB, s20_Pin, arr_1[0]);
	HAL_GPIO_WritePin(GPIOB, s21_Pin, arr_1[1]);
	HAL_GPIO_WritePin(GPIOB, s22_Pin, arr_1[2]);
	// s23_Pin (Chân D/D3) nhận arr_1[3] (MSB)
	HAL_GPIO_WritePin(GPIOB, s23_Pin, arr_1[3]);

	// Hàng ĐƠN VỊ (s30 -> s33)
	// s30_Pin (Chân A/D0) nhận arr_2[0] (LSB)
	HAL_GPIO_WritePin(GPIOB, s30_Pin, arr_2[0]);
	HAL_GPIO_WritePin(GPIOB, s31_Pin, arr_2[1]);
	HAL_GPIO_WritePin(GPIOB, s32_Pin, arr_2[2]);
	// s33_Pin (Chân D/D3) nhận arr_2[3] (MSB)
	HAL_GPIO_WritePin(GPIOB, s33_Pin, arr_2[3]);
}

void displayLED7SEG_MODE(int number){
	int arr[4] = {0};
	for(int i = 3; i >= 0; --i){
		arr[i] = number % 2;
		number /= 2;
	}
	HAL_GPIO_WritePin(GPIOB, mode0_Pin, arr[3]);
	HAL_GPIO_WritePin(GPIOB, mode1_Pin, arr[2]);
	HAL_GPIO_WritePin(GPIOB, mode2_Pin, arr[1]);
	HAL_GPIO_WritePin(GPIOB, mode3_Pin, arr[0]);
}

// ========================================================
// QUẢN LÝ NGẮT / CHẾ ĐỘ
// ========================================================

void disableInterruptLED_and_SEG(void){
	flagInterruptLED_and_SEG = 0;
	flagMode_RED_BLINK = 0;
	flagMode_YELLOW_BLINK = 0;
	flagMode_GREEN_BLINK = 0;
}

void enableInterruptLED_and_SEG(void){
	flagInterruptLED_and_SEG = 1;
	flagMode_RED_BLINK = 0;
	flagMode_YELLOW_BLINK = 0;
	flagMode_GREEN_BLINK = 0;
}

void enableInterruptForMode_2(void){
	flagMode_RED_BLINK = 1;
	flagMode_YELLOW_BLINK = 0;
	flagMode_GREEN_BLINK = 0;
}

void enableInterruptForMode_3(void){
	flagMode_RED_BLINK = 0;
	flagMode_YELLOW_BLINK = 1;
	flagMode_GREEN_BLINK = 0;
}

void enableInterruptForMode_4(void){
	flagMode_RED_BLINK = 0;
	flagMode_YELLOW_BLINK = 0;
	flagMode_GREEN_BLINK = 1;
}

void disableInterruptForMode_2(void){ flagMode_RED_BLINK = 0; }
void disableInterruptForMode_3(void){ flagMode_YELLOW_BLINK = 0; }
void disableInterruptForMode_4(void){ flagMode_GREEN_BLINK = 0; }

// ========================================================
// RESET HỆ THỐNG KHI CHUYỂN VỀ MODE 1
// ========================================================

void restartAllFromScratch(void){
	// Pha ban đầu: Hướng 1 R, Hướng 2 G
	flagRed[0] = 1; flagRed[1] = 0;
	flagYellow[0] = 0; flagYellow[1] = 0;
	flagGreen[0] = 0; flagGreen[1] = 1;
	counter_RED = counter_YELLOW = counter_GREEN = 0;
	displayLED7SEG_MODE(1); // Hiển thị Mode 1
}

// ========================================================
// CHẾ ĐỘ CHỈNH THỜI GIAN — BLINK ĐÈN
// ========================================================
void runBlinkMode(void){
    static int blinkCounter = 0;
    blinkCounter += CYCLE_TIME_MS; // CYCLE_TIME_MS = 10ms

    if(blinkCounter >= 250){
        blinkCounter = 0;

        // BLINK RED
        if(flagMode_RED_BLINK){
            static int redState = 0;
            redState = !redState;

            // Bật tắt R1, R2 và đảm bảo các đèn khác tắt
            displayLED_RED(redState, 0);
            displayLED_RED(redState, 1);
            displayLED_YELLOW(0,0);
            displayLED_YELLOW(0,1);
            displayLED_GREEN(0,0);
            displayLED_GREEN(0,1);
        }

        // BLINK YELLOW
        if(flagMode_YELLOW_BLINK){
            static int yellowState = 0;
            yellowState = !yellowState;

            displayLED_YELLOW(yellowState,0);
            displayLED_YELLOW(yellowState,1);
            displayLED_RED(0,0);
            displayLED_RED(0,1);
            displayLED_GREEN(0,0);
            displayLED_GREEN(0,1);
        }

        // BLINK GREEN
        if(flagMode_GREEN_BLINK){
            static int greenState = 0;
            greenState = !greenState;

            displayLED_GREEN(greenState,0);
            displayLED_GREEN(greenState,1);
            displayLED_RED(0,0);
            displayLED_RED(0,1);
            displayLED_YELLOW(0,0);
            displayLED_YELLOW(0,1);
        }
    }
}


// ========================================================
// HÀM CẬP NHẬT TRẠNG THÁI ĐÈN GIAO THÔNG (MODE 1)
// ========================================================
void runTimer_LED(void){
    // elapsed (ms) cho từng hướng
    static int elapsed1 = 0; // cho hướng 1 (R1/Y1/G1)
    static int elapsed2 = 0; // cho hướng 2 (R2/Y2/G2)

    // prev color code để detect edge (-1 = chưa khởi tạo)
    static int prevColor1 = -1;
    static int prevColor2 = -1;

    int curColor1 = -1;
    int curColor2 = -1;

    // ======= Xác định màu hiện tại của từng hướng =======
    if(flagRed[0])   curColor1 = 0;
    else if(flagGreen[0]) curColor1 = 1;
    else if(flagYellow[0]) curColor1 = 2;

    if(flagRed[1])   curColor2 = 0;
    else if(flagGreen[1]) curColor2 = 1;
    else if(flagYellow[1]) curColor2 = 2;

    // Nếu màu mới khác màu trước => reset elapsed tương ứng
    if(curColor1 != prevColor1){
        elapsed1 = 0;
        prevColor1 = curColor1;
    }
    if(curColor2 != prevColor2){
        elapsed2 = 0;
        prevColor2 = curColor2;
    }

    // Tăng elapsed (gọi mỗi CYCLE_TIME_MS)
    elapsed1 += CYCLE_TIME_MS;
    elapsed2 += CYCLE_TIME_MS;

    // Hàm helper tính remaining (ceil)
    #define REM_MS_TO_SEC_CEIL(ms_remain) ( ( (ms_remain) <= 0 ) ? 0 : ( ((ms_remain) + 999) / 1000 ) )

    int remaining1 = 0;
    int remaining2 = 0;

    // ====== Xử lý trạng thái & chuyển pha cho hướng 2 khi đang pha 0 (R1-G2-Y2) ======
    // Hướng 2:
    if(flagGreen[1]){ // G2 đang chạy
        int ms_left2 = duration_GREEN_ms - elapsed2;
        remaining2 = REM_MS_TO_SEC_CEIL(ms_left2);
        if(remaining2 < 1) remaining2 = 1;

        // Hướng 1 lúc này phải đang RED (R1)
        int ms_left1 = duration_RED_ms - elapsed1;
        remaining1 = REM_MS_TO_SEC_CEIL(ms_left1);
        if(remaining1 < 1) remaining1 = 1;

        // Khi G2 hết -> chuyển sang Y2
        if(elapsed2 >= duration_GREEN_ms){
            elapsed2 = 0;
            flagGreen[1] = 0;
            flagYellow[1] = 1;
        }
    }
    else if(flagYellow[1]){ // Y2 đang chạy
        int ms_left2 = duration_YELLOW_ms - elapsed2;
        remaining2 = REM_MS_TO_SEC_CEIL(ms_left2);
        if(remaining2 < 1) remaining2 = 1;

        // Hướng1 đang RED (R1)
        int ms_left1 = duration_RED_ms - elapsed1;
        remaining1 = REM_MS_TO_SEC_CEIL(ms_left1);
        if(remaining1 < 1) remaining1 = 1;

        // Khi Y2 hết -> chuyển pha: R1 tắt, Y2 tắt, R2 bật, G1 bật (pha 1)
        if(elapsed2 >= duration_YELLOW_ms){
            elapsed2 = 0;
            elapsed1 = 0; // reset elapsed hướng1 vì G1 sẽ bắt đầu
            flagYellow[1] = 0;
            flagRed[0] = 0;

            flagRed[1] = 1;
            flagGreen[0] = 1;
        }
    }
    // ====== Xử lý trạng thái & chuyển pha cho hướng 1 khi đang pha 1 (R2-G1-Y1) ======
    if(flagGreen[0]){ // G1 đang chạy
        int ms_left1 = duration_GREEN_ms - elapsed1;
        remaining1 = REM_MS_TO_SEC_CEIL(ms_left1);
        if(remaining1 < 1) remaining1 = 1;

        // Hướng 2 lúc này phải đang RED (R2)
        int ms_left2 = duration_RED_ms - elapsed2;
        remaining2 = REM_MS_TO_SEC_CEIL(ms_left2);
        if(remaining2 < 1) remaining2 = 1;

        // Khi G1 hết -> chuyển sang Y1
        if(elapsed1 >= duration_GREEN_ms){
            elapsed1 = 0;
            flagGreen[0] = 0;
            flagYellow[0] = 1;
        }
    }
    else if(flagYellow[0]){ // Y1 đang chạy
        int ms_left1 = duration_YELLOW_ms - elapsed1;
        remaining1 = REM_MS_TO_SEC_CEIL(ms_left1);
        if(remaining1 < 1) remaining1 = 1;

        // Hướng 2 đang RED (R2)
        int ms_left2 = duration_RED_ms - elapsed2;
        remaining2 = REM_MS_TO_SEC_CEIL(ms_left2);
        if(remaining2 < 1) remaining2 = 1;

        // Khi Y1 hết -> chuyển pha: R2 tắt, Y1 tắt, R1 bật, G2 bật (quay lại pha 0)
        if(elapsed1 >= duration_YELLOW_ms){
            elapsed1 = 0;
            elapsed2 = 0;
            flagYellow[0] = 0;
            flagRed[1] = 0;

            flagRed[0] = 1;
            flagGreen[1] = 1;
        }
    }

    // ====== HIỂN THỊ LED MÀU ======
    displayLED_RED(flagRed[0], 0);
    displayLED_RED(flagRed[1], 1);
    displayLED_YELLOW(flagYellow[0], 0);
    displayLED_YELLOW(flagYellow[1], 1);
    displayLED_GREEN(flagGreen[0], 0);
    displayLED_GREEN(flagGreen[1], 1);

    // ====== HIỂN THỊ 7 ĐOẠN ======
    // Hiển thị tối thiểu 1 (nếu <= 0)
    if(remaining1 <= 0) remaining1 = 1;
    if(remaining2 <= 0) remaining2 = 1;

    displayLED7SEG_LEFT(remaining1);
    displayLED7SEG_TOP(remaining2);

    #undef REM_MS_TO_SEC_CEIL
}
