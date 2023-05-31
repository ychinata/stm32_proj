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
#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 


//定义电机驱动管脚：TB6612
#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOB
#define MOTOR_IN_GPIO      GPIOB    //假设四个IN为同一GPIO组
#define MOTOR_PIN_AIN1  GPIO_Pin_14
#define MOTOR_PIN_AIN2  GPIO_Pin_15
#define MOTOR_PIN_BIN1  GPIO_Pin_13
#define MOTOR_PIN_BIN2  GPIO_Pin_12

#define MOTOR_PWM_RCC       RCC_APB2Periph_GPIOA
#define MOTOR_PWM_GPIO      GPIOA       ////假设2个PWM为同一GPIO组
#define MOTOR_PIN_PWMA      GPIO_Pin_8
#define MOTOR_PIN_PWMB      GPIO_Pin_11

#define MOTOR_PX_OUT PBout
//宏的名字建议加MOTOR_前缀
#define AIN1   MOTOR_PX_OUT(MOTOR_PIN_AIN1)
#define AIN2   MOTOR_PX_OUT(MOTOR_PIN_AIN2)
#define BIN1   MOTOR_PX_OUT(MOTOR_PIN_BIN1)
#define BIN2   MOTOR_PX_OUT(MOTOR_PIN_BIN2)

#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11

//定义电机驱动管脚：TB6612

void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
