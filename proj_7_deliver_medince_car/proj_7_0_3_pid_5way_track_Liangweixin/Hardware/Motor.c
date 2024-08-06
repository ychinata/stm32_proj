#include "stm32f10x.h"                  // Device header
#include "pwm.h"

/*  
    TB6612引脚图
    BIN2——>  PB12       IN1 IN2
    BIN1——>  PB13       0    0  停止
    AIN1——>  PB14       0    1  正传
    AIN2——>  PB15       1    0  反转  
    
*/

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
                           
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Motor_Left_SetSpeed(int16_t Speed)    //左轮PWM设置
{
	PWM_SetCompare1(Speed);
}

void Motor_Right_SetSpeed(int16_t Speed)  // 右轮PWM设置
{
	PWM_SetCompare4(Speed);
}

void Motor_Right_Stop(void)  // 右轮停止
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	PWM_SetCompare4(0);
} 


void Motor_Left_Stop(void)  // 左轮停止
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_14); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_15); 
	PWM_SetCompare1(0);
}
    

void Motor_Left_Fwd(void)  // 左轮正传
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void Motor_Left_Rev(void)  // 左轮反转
{
    GPIO_SetBits(GPIOB, GPIO_Pin_14);  
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}


void Motor_Right_Fwd(void)  // 右轮正传
{
    GPIO_SetBits(GPIOB, GPIO_Pin_13); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
}

void Motor_Right_Rev(void)  // 右轮反转
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

