/*
 * led_display.h
 *
 *  Created on: Oct 16, 2025
 *      Author: BUI THANH QUI
 */



#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

void setDurationTime_RED_ms(int value);
int  getDurationTime_RED_ms(void);
void setDurationTime_YELLOW_ms(int value);
int  getDurationTime_YELLOW_ms(void);
void setDurationTime_GREEN_ms(int value);
int  getDurationTime_GREEN_ms(void);

void setNumberDisplaySEG(int index, int value);
int  getNumberDisplaySEG(int index);

void displayLED7SEG_MODE(int number);
void displayLED7SEG_LEFT(int number);
void displayLED7SEG_TOP(int number);

void displayLED_RED(int IS_ON, int index);
void displayLED_GREEN(int IS_ON, int index);
void displayLED_YELLOW(int IS_ON, int index);

void disableInterruptLED_and_SEG(void);
void enableInterruptLED_and_SEG(void);
void enableInterruptForMode_2(void);
void enableInterruptForMode_3(void);
void enableInterruptForMode_4(void);
void disableInterruptForMode_2(void);
void disableInterruptForMode_3(void);
void disableInterruptForMode_4(void);

void restartAllFromScratch(void);
void runTimer_LED(void);
#endif


