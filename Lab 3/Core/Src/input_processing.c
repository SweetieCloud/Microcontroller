/*
 * input_processing.c
 *
 * Created on: Oct 23, 2025
 * Author: BUI THANH QUI
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"

// ===========================================================
// HÀM CẬP NHẬT TRẠNG THÁI FSM (được gọi mỗi chu kỳ 10ms)
// ===========================================================

void fsm_for_input_processing(void){
    static int currentMode = 1;    // mode hiện tại: 1–4
    static int tempValue = 0;      // giá trị tạm khi chỉnh thời gian

    // ====== NÚT 1: Chuyển MODE ======
    if(is_button_pressed(0)){
        currentMode++;
        if(currentMode > 4) currentMode = 1;

        // Tắt blink của mode cũ và bật logic của mode mới
        disableInterruptForMode_2();
        disableInterruptForMode_3();
        disableInterruptForMode_4();

        displayLED7SEG_MODE(currentMode);

        switch(currentMode){
        case 1: // Mode 1: chạy bình thường
            enableInterruptLED_and_SEG();
            restartAllFromScratch();
            break;

        case 2: // Mode 2: chỉnh RED
            disableInterruptLED_and_SEG();
            enableInterruptForMode_2();  // làm R1,R2 chớp
            // Lấy giá trị hiện tại của RED (chia 1000 để chuyển từ ms sang s)
            tempValue = getDurationTime_RED_ms() / 1000;
            displayLED7SEG_LEFT(tempValue);
            displayLED7SEG_TOP(tempValue);
            break;

        case 3: // Mode 3: chỉnh YELLOW
            disableInterruptLED_and_SEG();
            enableInterruptForMode_3();  // làm Y1,Y2 chớp
            // Lấy giá trị hiện tại của YELLOW
            tempValue = getDurationTime_YELLOW_ms() / 1000;
            displayLED7SEG_LEFT(tempValue);
            displayLED7SEG_TOP(tempValue);
            break;

        case 4: // Mode 4: chỉnh GREEN
            disableInterruptLED_and_SEG();
            enableInterruptForMode_4();  // làm G1,G2 chớp
            // GREEN = RED - YELLOW (Theo logic đèn giao thông 3 pha)
            {
                int red_s = getDurationTime_RED_ms() / 1000;
                int yellow_s = getDurationTime_YELLOW_ms() / 1000;
                int green_s = red_s - yellow_s;
                // Đảm bảo GREEN tối thiểu 1s
                if(green_s < 1) green_s = 1;
                setDurationTime_GREEN_ms(green_s * 1000); // Lưu giá trị đã tính
                tempValue = green_s;
            }
            displayLED7SEG_LEFT(tempValue);
            displayLED7SEG_TOP(tempValue);
            break;
        }
    }

    // ====== NÚT 2: Tăng giá trị (Chỉ hoạt động trong Mode 2, 3, 4) ======
    // Lưu ý: Bài tập yêu cầu chỉ tăng (increase), không yêu cầu giữ 1s (auto-increasing)
    if(is_button_pressed(1)){
        if(currentMode >= 2 && currentMode <= 4){
            tempValue++;
            if(tempValue > 99) tempValue = 1; // Range 1-99

            // Nếu đang chỉnh GREEN, cập nhật lại biến GREEN duration
            if(currentMode == 4){
                setDurationTime_GREEN_ms(tempValue * 1000);
            }

            displayLED7SEG_LEFT(tempValue);
            displayLED7SEG_TOP(tempValue);
        }
    }

    // ====== NÚT 3: Lưu (SET) ======
    if(is_button_pressed(2)){
        int red_ms = getDurationTime_RED_ms();
        int yellow_ms = getDurationTime_YELLOW_ms();
        int green_ms = getDurationTime_GREEN_ms();

        if(currentMode == 2){
            red_ms = tempValue * 1000;
            setDurationTime_RED_ms(red_ms);

            // THÊM: Tính lại GREEN
            green_ms = red_ms - yellow_ms;
            if(green_ms <= 0) green_ms = 1000; // Đảm bảo green tối thiểu 1s
            setDurationTime_GREEN_ms(green_ms);
        }
        else if(currentMode == 3){
            yellow_ms = tempValue * 1000;
            setDurationTime_YELLOW_ms(yellow_ms);

            // THÊM: Tính lại GREEN
            green_ms = red_ms - yellow_ms;
            if(green_ms <= 0) green_ms = 1000; // Đảm bảo green tối thiểu 1s
            setDurationTime_GREEN_ms(green_ms);
        }
        else if(currentMode == 4){
            // Mode 4 đã xử lý đúng: Cập nhật RED = GREEN + YELLOW
            yellow_ms = getDurationTime_YELLOW_ms(); // Lấy yellow hiện tại
            green_ms = getDurationTime_GREEN_ms();   // Lấy green vừa set
            setDurationTime_RED_ms(yellow_ms + green_ms);
        }
    }
}

// ===========================================================
// HÀM KHỞI TẠO FSM (gọi 1 lần trong main)
// ===========================================================
void fsm_init(void){
	displayLED7SEG_MODE(1); // Mặc định Mode 1
	enableInterruptLED_and_SEG();
	restartAllFromScratch();
}
