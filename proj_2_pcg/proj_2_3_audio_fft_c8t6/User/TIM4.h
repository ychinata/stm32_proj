#ifndef __TIM4_H
#define __TIM4_H
#include "stm32f10x.h"

void TIM4_Init(void);
void TIM4_SetIRQHandler(void (*Function)(void));

#endif
