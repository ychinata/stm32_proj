/**********************************************************************************
 * ��������:	OLED��ʾ����Ƶ��
 * ��������:	���־� QQ:21182889
 * ����ʱ��:	2021.05.05
 * �ٴο�Դ�뱣����ͷ����Ϣ
***********************************************************************************/

#include "MainHeader.h"


//�������ã�
//ADC:AP0
//OLED:BP6/BP7


extern u16 TIM2_NUM; //��ʱ��2�ƴΣ��Դ�Ϊ��ѭ��ʱ��Ƭ
// �ް������ȹ̶�Ϊ��
u8 Spectrum_flag = 1; //Ƶ����ʾ��ʶ


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
		if(TIM2_NUM%300==1&&Spectrum_flag) { //����Ƶ����ʾƵ��		
			Spectrum_GetValue();
			Spectrum_Show();
		}
	}
}
