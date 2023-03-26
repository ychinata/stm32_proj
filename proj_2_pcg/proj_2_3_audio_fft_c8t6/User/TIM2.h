#ifndef __TIM2_H
#define __TIM2_H
#include "stm32f10x.h"

void TIM2_Init(void);
void TIM2_SetIRQHandler(void (*Function)(void));
void TIM2_SetPeriod(uint16_t Period);

#endif
