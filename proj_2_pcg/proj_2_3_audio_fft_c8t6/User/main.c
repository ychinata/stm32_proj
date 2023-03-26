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

/*硬件
 *STM32F103C8T6最小系统模块
 *OLED 0.96寸显示屏:PB6/PB7
 *MAX9814麦克风：OUT-PA0
 *KEY:PB0/PB1
 */

/* 定时器
 * TIM2:定时触发傅里叶计算、填充显存，刷新OLED显示；
 * TIM3:用于ADC DMA, 数据存储于DMA1_ADCValue
 * TIM4:按键检测
 */

/*引脚配置：
 *ADC:  PA0     - ADC1_PIN
 *OLED: PB6/PB7 - OLED_PIN_SCL
 *KEY:  PB0/PB1 - Key1_Read
*/

/* 参数修改：
 * SPECTRUM_LEVEL_DIV
 */




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
