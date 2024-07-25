#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "stm32f10x.h"

extern uint8_t Display_Buf[64][32];

void Display_Init(void);
void Display_Enable(uint8_t EnableFlag);
void Display_ClearBuf(void);
void Display_ClearAreaBuf(uint8_t StartX,uint8_t EndX,uint8_t StartY,uint8_t EndY);
void Display_Update(void);
void Display_SetBrightness(uint16_t Brightness);
void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color);
void Display_Clear(void);
void Display_ShowChar(uint8_t X,uint8_t Y,char Char,uint8_t SizeColor);
void Display_ShowString(uint8_t X,uint8_t Y,char *String,uint8_t SizeColor);
void Display_ShowNumber(uint8_t X,uint8_t Y,uint16_t Number,uint8_t Length,uint8_t SizeColor);
void Display_ShowPoint(uint8_t X,uint8_t Y,uint8_t Color);

#endif
