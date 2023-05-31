/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/
#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

//定义电机编码器驱动管脚
#define ENCODER_LEFT_IN_RCC    RCC_APB2Periph_GPIOA
#define ENCODER_LEFT_IN_GPIO      GPIOA    //假设四个IN为同一GPIO组
#define ENCODER_LEFT_PIN_A  GPIO_Pin_0      // 待确认哪个是A相哪个是B相
#define ENCODER_LEFT_PIN_B  GPIO_Pin_1

#define ENCODER_RIGHT_IN_RCC    RCC_APB2Periph_GPIOB
#define ENCODER_RIGHT_IN_GPIO      GPIOB    //假设四个IN为同一GPIO组
#define ENCODER_RIGHT_PIN_A  GPIO_Pin_6      // 待确认哪个是A相哪个是B相
#define ENCODER_RIGHT_PIN_B  GPIO_Pin_7
//定义电机编码器驱动管脚

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
void TIM4_IRQHandler(void);
void TIM2_IRQHandler(void);
#endif
