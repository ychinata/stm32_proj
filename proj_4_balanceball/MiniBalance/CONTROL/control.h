#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define FILTERING_TIMES  10
#define ZERO_X 133
#define ZERO_Y 100

void TIM1_UP_IRQHandler(void);
void Key(void);
u8 Turn_Off( int voltage);
void Set_Pwm(int motor_x,int motor_y);
int myabs(int a);
void Control(float Step);
int balanceX(float Angle);
int balanceY(float Angle);
void Get_RC(void);
void Adjust(void);
void Setting_Move(void);
#endif
