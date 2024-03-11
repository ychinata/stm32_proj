#include "stm32f10x.h"                  // Device header

//void PWM2_Init(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	TIM_InternalClockConfig(TIM4);
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
//	
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 3000;		//CCR
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//	
//	TIM_ARRPreloadConfig(TIM4, ENABLE);  
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
//}
void PWM2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;   					 				//初始化TIM1设置ARR,PSC控制输出PWM的周期
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //PWM通道设置
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);     			  //使能定时器4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);      				//AFIO复用功能时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   				//GPIOA使能
	
	
	//设置该引脚为复用输出功能
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	       //TIM1_CH1/CH2/CH3/CH4
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                                     //复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);                                               //初始化GPIO
	
	
	//初始化TIM1,设置TIM4的ARR和PSC
	TIM_TimeBaseStruct.TIM_Period = 20000 - 1;                   					 //设置自动重装载周期值   //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;                					 //设置预分频值           //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;               				 //设置时钟分割TDTS = Tck_tim	
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;         //TIM向上计数模式
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStruct);                      //根据指定参数初始化TIMx  //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_OCInitStructure.TIM_Pulse = 3000;


	
	//初始化出输出比较参数
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;          //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);                            //根据TIMX的参数设定初始化外设 TIM1 ch1 ch2 ch3 ch4	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);                   //使能预装载寄存器
  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);                                
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);    
	
	

}




void PWM_SetCompare5(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}

void PWM_SetCompare6(uint16_t Compare)
{
	TIM_SetCompare2(TIM1, Compare);
}

