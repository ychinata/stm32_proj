#include "stm32f10x.h"
#include "ADC1.h"
#include "DMA1.h"
#include "TIM2.h"

uint16_t *Sample_Value;

void Sample_Init(void)
{
	ADC1_Init();
	DMA1_Init();
	TIM2_Init();
	Sample_Value=DMA1_ADCValue;
}

void Sample_GetValue(void)
{
	DMA1_Start();
	while(DMA1_GetFinishFlag()!=SET);
	DMA1_ClearFinishFlag();
}
