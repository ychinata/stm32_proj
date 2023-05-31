/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/
#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�

//�����������������ܽ�
#define ENCODER_LEFT_IN_RCC    RCC_APB2Periph_GPIOA
#define ENCODER_LEFT_IN_GPIO      GPIOA    //�����ĸ�INΪͬһGPIO��
#define ENCODER_LEFT_PIN_A  GPIO_Pin_0      // ��ȷ���ĸ���A���ĸ���B��
#define ENCODER_LEFT_PIN_B  GPIO_Pin_1

#define ENCODER_RIGHT_IN_RCC    RCC_APB2Periph_GPIOB
#define ENCODER_RIGHT_IN_GPIO      GPIOB    //�����ĸ�INΪͬһGPIO��
#define ENCODER_RIGHT_PIN_A  GPIO_Pin_6      // ��ȷ���ĸ���A���ĸ���B��
#define ENCODER_RIGHT_PIN_B  GPIO_Pin_7
//�����������������ܽ�

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
#endif
