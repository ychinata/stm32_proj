#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"
void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
int Read_Speed(int TIMX);

#endif
