/*
 * input_reading.c
 *
 *  Created on: Oct 23, 2025
 *      Author: BUI THANH QUI
 */

#include "main.h"
#include "input_reading.h"

// ===========================================================
// CẤU HÌNH
// ===========================================================
#define N0_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING 100 // 100 * 10ms = 1s

// ===========================================================
// BIẾN TOÀN CỤC
// ===========================================================
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];      // Giá trị ổn định cuối cùng
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer3[N0_OF_BUTTONS];

static uint8_t flagForButtonPressed[N0_OF_BUTTONS];    // Cờ nhấn
static uint8_t flagForButtonPressed1s[N0_OF_BUTTONS];  // Cờ giữ 1s
static uint16_t counterForButtonPressed1s[N0_OF_BUTTONS];

// ===========================================================
// CẤU HÌNH CHÂN GPIO CỦA CÁC NÚT
// ===========================================================
// Chú ý: phải đúng với CubeMX mapping trong main.h
static GPIO_TypeDef* BUTTON_PORT[N0_OF_BUTTONS] = {but1_GPIO_Port, but2_GPIO_Port, but3_GPIO_Port};
static uint16_t BUTTON_PIN[N0_OF_BUTTONS]       = {but1_Pin, but2_Pin, but3_Pin};

// ===========================================================
// HÀM ĐỌC NÚT (GỌI MỖI 10MS TRONG TIM2 INTERRUPT)
// ===========================================================
void button_reading(void){
	for(int i = 0; i < N0_OF_BUTTONS; i++){
		debounceButtonBuffer3[i] = debounceButtonBuffer2[i];
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(BUTTON_PORT[i], BUTTON_PIN[i]);

		if((debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) &&
		   (debounceButtonBuffer2[i] == debounceButtonBuffer3[i])){
			if(buttonBuffer[i] != debounceButtonBuffer3[i]){
				buttonBuffer[i] = debounceButtonBuffer3[i];

				// Nút được nhấn (LOW)
				if(buttonBuffer[i] == GPIO_PIN_RESET){
					flagForButtonPressed[i] = 1;
					counterForButtonPressed1s[i] = DURATION_FOR_AUTO_INCREASING;
				}
			}
			else{
				// Giữ nút
				if(buttonBuffer[i] == GPIO_PIN_RESET){
					if(counterForButtonPressed1s[i] > 0){
						counterForButtonPressed1s[i]--;
						if(counterForButtonPressed1s[i] == 0){
							flagForButtonPressed1s[i] = 1;
						}
					}
				}
				else {
					counterForButtonPressed1s[i] = 0;
				}
			}
		}
	}
}

// ===========================================================
// HÀM KIỂM TRA NÚT NHẤN NGẮN
// ===========================================================
unsigned char is_button_pressed (unsigned char index){
	if(index >= N0_OF_BUTTONS) return 0;
	if(flagForButtonPressed[index] == 1){
		flagForButtonPressed[index] = 0;
		return 1;
	}
	return 0;
}

// ===========================================================
// HÀM KIỂM TRA NÚT GIỮ 1S
// ===========================================================
unsigned char is_button_pressed_1s (unsigned char index){
	if(index >= N0_OF_BUTTONS) return 0;
	if(flagForButtonPressed1s[index] == 1){
		flagForButtonPressed1s[index] = 0;
		return 1;
	}
	return 0;
}
