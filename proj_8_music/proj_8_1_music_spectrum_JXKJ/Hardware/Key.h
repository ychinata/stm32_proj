#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

#define KEY1_PRESSED				1
#define KEY1_LONG_PRESSED			3
#define KEY1_LONG_PRESSED_COUNT		5
#define KEY2_PRESSED				2
#define KEY2_LONG_PRESSED			4
#define KEY2_LONG_PRESSED_COUNT		6

void Key_Init(void);
uint8_t Key_GetKeyNum(void);
uint8_t Key_GetKeyNumber(void);
void Key_Loop(void);

#endif
