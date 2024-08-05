#include "stm32f10x.h"                  // Device header



/* 
    TB6612引脚接线
    PWM输出使用定时器1 引脚：PA8（CH1） PA11（CH4）
 
    PWMA——> PA8
    PWMB——> PA11
*/
void PWM_Init(uint16_t arr,uint16_t src)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;     //T1 通道1 通道4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计算
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = src - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); //给结构体赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择输出比较模式PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //High输出 极性不翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //设置输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR的值
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //打开通道1 PA8
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //打开通道4 PA11
	
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH1上的预装载寄存器
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH4上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);     //使能TIMx在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM1,ENABLE);        //MOE 主输出使能,高级定时器必须开启这个
}


/* 设置占空比定时器1通道4 */
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);
}

/* 设置占空比定时器1通道1 */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}
