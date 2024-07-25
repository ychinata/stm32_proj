#ifndef __AT24C02_H
#define __AT24C02_H
#include "stm32f10x.h"

void AT24C02_Init(void);
void AT24C02_WriteByte(uint8_t WordAddress,uint8_t Data);
uint8_t AT24C02_ReadByte(uint8_t WordAddress);

#endif
