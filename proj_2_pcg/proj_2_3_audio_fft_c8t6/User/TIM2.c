#include "MainHeader.h"
void (*TIM2_Handler)(void);

void TIM2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 100-1;//10k
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_SetIRQHandler(void (*Function)(void))
{
	TIM2_Handler=Function;
}

void TIM2_SetPeriod(uint16_t Period)
{
	TIM_Cmd(TIM2, DISABLE);
	TIM_SetCounter(TIM2,0);
	TIM_SetAutoreload(TIM2,Period-1);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update))
	{
		TIM2_Handler();
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	}
}
