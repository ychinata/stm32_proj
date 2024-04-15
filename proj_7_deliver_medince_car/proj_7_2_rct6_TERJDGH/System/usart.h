#ifndef __USART_H
#define __USART_H

extern unsigned char k210_num;
extern int16_t Serial_RxNUM;
void USART2_Init(void);
void Get_k210_NUM(uint8_t num);
extern int16_t k210_Rx;

#endif


