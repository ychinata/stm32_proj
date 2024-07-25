#ifndef __DMA1_H
#define __DMA1_H
#include "stm32f10x.h"

extern uint16_t DMA1_ADCValue[];

void DMA1_Init(void);
void DMA1_Start(void);
uint8_t DMA1_GetFinishFlag(void);
void DMA1_ClearFinishFlag(void);

#endif
