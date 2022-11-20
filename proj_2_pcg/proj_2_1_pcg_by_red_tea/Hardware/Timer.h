#include "sys.h"  





extern u8	TIM2_Timing; 
extern u8	TIM3_Timing; 
extern u8	TIM4_Timing;

extern u8 TIM3_Flag;
extern u8 TIM4_Flag;

void TIM1_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);
void TIM6_Init(u16 arr,u16 psc);
void TIM7_Init(u16 arr,u16 psc);
