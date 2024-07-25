#ifndef __LIGHT_H
#define __LIGHT_H
#include "stm32f10x.h"

void Light_Init(void);
uint16_t Light_GetValue(void);
void Light_Loop(void);

#endif
