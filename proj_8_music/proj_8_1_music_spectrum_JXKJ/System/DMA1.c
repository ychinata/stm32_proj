#include "stm32f10x.h"

uint16_t DMA1_ADCValue[128];

void DMA1_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DMA1_ADCValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 128;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

void DMA1_Start(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE );
	DMA_SetCurrDataCounter(DMA1_Channel1,128);	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

uint8_t DMA1_GetFinishFlag(void)
{
	return DMA_GetITStatus(DMA1_IT_TC1);
}

void DMA1_ClearFinishFlag(void)
{
	DMA_ClearFlag(DMA1_IT_TC1);
}
