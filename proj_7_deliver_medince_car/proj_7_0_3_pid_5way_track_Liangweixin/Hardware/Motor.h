#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"                  // Device header

void Motor_Init(void);
void Motor_Left_SetSpeed(int16_t Speed);
void Motor_Right_SetSpeed(int16_t Speed);
void Motor_Left_Fwd(void);  // 左轮正传
void Motor_Left_Rev(void);  // 左轮反转
void Motor_Right_Fwd(void);  // 右轮正传
void Motor_Right_Rev(void);  // 右轮反转
#endif
