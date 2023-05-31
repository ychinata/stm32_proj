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
#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 


//�����������ܽţ�TB6612
#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOB
#define MOTOR_IN_GPIO      GPIOB    //�����ĸ�INΪͬһGPIO��
#define MOTOR_PIN_AIN1  GPIO_Pin_14
#define MOTOR_PIN_AIN2  GPIO_Pin_15
#define MOTOR_PIN_BIN1  GPIO_Pin_13
#define MOTOR_PIN_BIN2  GPIO_Pin_12

#define MOTOR_PWM_RCC       RCC_APB2Periph_GPIOA
#define MOTOR_PWM_GPIO      GPIOA       ////����2��PWMΪͬһGPIO��
#define MOTOR_PIN_PWMA      GPIO_Pin_8
#define MOTOR_PIN_PWMB      GPIO_Pin_11

#define MOTOR_PX_OUT PBout
//������ֽ����MOTOR_ǰ׺
#define AIN1   MOTOR_PX_OUT(MOTOR_PIN_AIN1)
#define AIN2   MOTOR_PX_OUT(MOTOR_PIN_AIN2)
#define BIN1   MOTOR_PX_OUT(MOTOR_PIN_BIN1)
#define BIN2   MOTOR_PX_OUT(MOTOR_PIN_BIN2)

#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11

//�����������ܽţ�TB6612

void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
