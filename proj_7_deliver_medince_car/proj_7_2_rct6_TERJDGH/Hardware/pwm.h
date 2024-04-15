#ifndef __PWM_H
#define __PWM_H

void PWM_Init_TIM1(uint16_t Psc, uint16_t Per);
void servo_PWM_Init(void);

void PWM_SetCompare2(uint16_t Compare);
#endif
