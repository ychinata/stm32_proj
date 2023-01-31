#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"


extern u8 g_Uart2DmaFinishFlag;

//´®¿ÚDMA
void DMA_UART_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar);
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);

void DMA_UART2_TX_NVIC_Config(u8 cmd);

//ADC
void DMA_ADC_Config(u16 *buffer ,u16 len);
void ADC_DMA_Enable(void);
void DMA_ADC_NVIC_Config(u8 cmd);
	 

#endif

