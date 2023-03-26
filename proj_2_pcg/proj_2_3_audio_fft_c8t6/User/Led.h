#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"

#define LED_R1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)(x))
#define LED_G1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)(x))
#define LED_B1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)(x))
#define LED_R2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)(x))
#define LED_G2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)(x))
#define LED_B2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)(x))
#define LED_A(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)(x))
#define LED_B(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)(x))
#define LED_C(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)(x))
#define LED_D(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)(x))
#define LED_E(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)(x))
#define LED_CLK(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)(x))
#define LED_LAT(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)(x))
#define LED_OE(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)(x))

void Display_Init(void);
void Display(void);
void Display_ClearBuf(void);
void Display_ShowPoint(uint16_t x,uint16_t y,uint8_t color);
void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color);
void Display_SetLevel2(uint8_t X,uint8_t Level,uint8_t Color);
void Display_ShowNumber(uint8_t X,uint8_t Y,uint16_t Number,uint8_t Length,uint8_t Color);
void Display_ShowString(int X,int Y,char *String,uint8_t Color);
void Display_ShowPicture_Single(uint8_t X,uint8_t Y,uint8_t W,uint8_t H,uint8_t Color);
void Display_Update(void);

#endif
