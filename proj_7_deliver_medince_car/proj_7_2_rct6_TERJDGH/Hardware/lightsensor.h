#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#include "sys.h"
void Beep_Init(void);
void Beep_ON(void);
void Beep_OFF(void);
void light_Init(void);
uint8_t wait_pha_ok(void);

#endif
