#ifndef __CLOCK_H
#define __CLOCK_H

void Clock_Init(void);
void Clock_Display(void);
void Clock_TimeSetAdd(uint8_t Select);
void Clock_TimeSetDisplay(uint8_t Select);
void Clock_ChangeColor(void);
void Clock_Loop(void);

#endif
