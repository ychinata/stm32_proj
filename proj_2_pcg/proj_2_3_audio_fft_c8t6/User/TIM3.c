//#include "stm32f10x.h"
//#include "TIM3.h"
#include "MainHeader.h"
void (*TIM3_Handler)(void);



void TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;	
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );
	
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_SetIRQHandler(void (*Function)(void))
{
	TIM3_Handler=Function;
}

void TIM3_SetPeriod(uint16_t Period)
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_SetCounter(TIM3,0);
	TIM_SetAutoreload(TIM3,Period-1);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update))
	{
		TIM3_Handler();
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	}
}
