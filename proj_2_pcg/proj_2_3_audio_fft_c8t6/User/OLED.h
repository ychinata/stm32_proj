#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

void OLED_SetPointer(uint8_t X, uint8_t Y);
void OLED_WriteData(uint8_t Data);

#endif
