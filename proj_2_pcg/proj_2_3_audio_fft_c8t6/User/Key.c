/***********************************************
	按键采集
************************************************/

#include "MainHeader.h"
#include "Key.h"

extern uint8_t Spectrum_Type;
extern u8 Spectrum_flag; //频谱显示标识

#define Key1_Read()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define Key2_Read()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// 获取键值
uint8_t Key_GetKeyNum(void)
{
	uint8_t KeyNum=0;
	if (Key1_Read()==0) {       // 检测到按下
		Delay_ms(20);           // 延时
		while(Key1_Read()==0);  // 确实按下了
		Delay_ms(20);           // 再延时
		KeyNum=1;               // 赋值
	}
	if (Key2_Read()==0) {
		Delay_ms(20);
		while(Key2_Read()==0);
		Delay_ms(20);
		KeyNum=2;
	}
	return KeyNum;
}

/* 按键共3种频谱样式 */
void Key_Loop(void)
{
	uint8_t KeyNum=0;
	KeyNum = Key_GetKeyNum();
	if (KeyNum == 1) {
		if (Spectrum_flag==0) {
            Spectrum_flag=1;
		} else {
			Spectrum_flag=0;
			Display_Text(); //显示欢迎界面
		}
	}	
	if (KeyNum == 2){
		Spectrum_Type++;
		Spectrum_Type%=3;
	}	
}
