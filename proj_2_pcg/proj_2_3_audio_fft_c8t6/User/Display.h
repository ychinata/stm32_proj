#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "stm32f10x.h"

#define LED(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)(x))

void Display_Init(void);
void Display_ClearBuf(void);
void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color);
void Display_SetLevel2(uint8_t X,uint8_t Level,uint8_t Color);
//void Display_ShowNumber(uint8_t X,uint8_t Y,uint16_t Number,uint8_t Length,uint8_t Color);
void Display_Text(void);

#endif
