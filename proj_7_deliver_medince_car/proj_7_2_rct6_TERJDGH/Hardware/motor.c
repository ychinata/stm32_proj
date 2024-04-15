#include "motor.h"

int PWM_MAX = 4500, PWM_MIN = -4500;


void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//限幅函数
void Limit(int *motorA, int *motorB)
{
	if(*motorA > PWM_MAX)
	{
		*motorA = PWM_MAX;
	}
	if(*motorA < PWM_MIN)
	{
		*motorA = PWM_MIN;
	}
	if(*motorB > PWM_MAX)
	{
		*motorB = PWM_MAX;
	}
	if(*motorB < PWM_MIN)
	{
		*motorB = PWM_MIN;
	}
}
//刹车
void brake(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

//绝对值函数
int ZDS_abs(int p)
{
	int q;
	q = p>0 ? p : (-q);
	return q;
}

int Dead_Zone = 0;//死区

//赋值函数，PID运算完之后
void Load(int motor1, int motor2)
{
	if(motor1>0)
	{
		Ain1 = 0;
		Ain2 = 1;
	}
	else
	{
		Ain1 = 1;
		Ain2 = 0;
	}
	TIM_SetCompare1(TIM1, ZDS_abs(motor1)+Dead_Zone);
	if(motor2>0)
	{
		Bin1 = 1;
		Bin2 = 0;
	}
	else
	{
		Bin1 = 0;
		Bin2 = 1;
	}
	TIM_SetCompare4(TIM1, ZDS_abs(motor2)+Dead_Zone);
}

