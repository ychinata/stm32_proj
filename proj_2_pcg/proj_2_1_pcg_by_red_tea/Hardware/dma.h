#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"


extern u8 g_Uart1DmaFinishFlag;
extern u8 g_Uart2DmaFinishFlag;
extern u8 g_Uart3DmaFinishFlag;
extern u8 g_Uart4DmaFinishFlag;

extern u8 SPI1_DMA_Finish;
extern u8 SPI2_DMA_Finish;

extern u8 ADC_DMA_Finish;

//´®¿ÚDMA
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar);
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);

void DMA_UART1_TX_NVIC_Config(u8 cmd);
void DMA_UART2_TX_NVIC_Config(u8 cmd);
void DMA_UART3_TX_NVIC_Config(u8 cmd);
void DMA_UART4_TX_NVIC_Config(u8 cmd);


//ADC
void DMA_ADC_Config(u16 *buffer ,u16 len);
void ADC_DMA_Enable(void);
void DMA_ADC_NVIC_Config(u8 cmd);


//SPI1 DMA
u8 SPI1_DMA_Config(void);
u8 SPI2_DMA_Config(void);
u8 SPI3_DMA_Config(void);

void SPI1_DMA_NVIC_Config(u8 cmd);
void SPI2_DMA_NVIC_Config(u8 cmd);
void SPI3_DMA_NVIC_Config(u8 cmd);

u8 SPI1_DMA_RX(u8 *buffer,u16 len);
u8 SPI2_DMA_RX(u8 *buffer,u16 len);
u8 SPI3_DMA_RX(u8 *buffer,u16 len);

u8 SPI1_DMA_TX(u8 *buffer,u16 len);
u8 SPI2_DMA_TX(u8 *buffer,u16 len);
u8 SPI3_DMA_TX(u8 *buffer,u16 len);
		 

#endif

