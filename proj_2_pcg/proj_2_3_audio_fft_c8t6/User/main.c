/**********************************************************************************
 * 程序名称:	OLED显示音乐频谱
 * 程序作者:	桔柚京 QQ:21182889
 * 创建时间:	2021.05.05
 * 再次开源请保留此头部信息
***********************************************************************************/

#include "MainHeader.h"


//引脚配置：
//ADC:AP0
//OLED:BP6/BP7


extern u16 TIM2_NUM; //定时器2计次，以此为主循环时间片
// 无按键，先固定为真
u8 Spectrum_flag = 1; //频谱显示标识


int main(void)
{
	Display_Init();
	Key_Init();
	ADC1_DMA1_TIM3_Init();
	Spectrum_Init();
	TIM4_SetIRQHandler(Key_Loop);
	TIM4_Init();
	OLED_Init();

	Display_Text();
	
	while (1) {
		if(TIM2_NUM%300==1&&Spectrum_flag) { //调节频谱显示频率		
			Spectrum_GetValue();
			Spectrum_Show();
		}
	}
}
