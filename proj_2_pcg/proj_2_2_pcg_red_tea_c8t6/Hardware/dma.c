#include "dma.h"
#include "delay.h"       //��ʱ
#include "myQueue.h"
#include "Timer.h"
#include "usart.h"       //���� 
#include "adc.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2022/02/08
//�汾��V1.2
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////

// ȫ�ֱ���
u8 g_Uart2DmaFinishFlag;
u8 g_AdcDmaFinishFlag;

//DMA�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸� 
/////////////////////////////////////////////////////////////////////////////////////////////

// USART����+DMA: ʹ��DMA1_Channel7,����(����)��Ŀ��
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx,cpar:�����ַ,cmar:�ڴ��ַ
// DMA_CHx�Ƿ�д��ΪDMA1_Channel7,û��Ҫ�������
void DMA_UART_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
    DMA_InitTypeDef DMA_InitStructure;	

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//ʹ��DMA����

    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    //DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
}


//����һ��DMA����
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
    DMA_CHx -> CCR &= ~(1<<0);	//�رյ�ǰ��DMA���䣨������û�д�����ɣ�
    DMA_CHx -> CNDTR = len;			//�������ݴ�����
    DMA_CHx -> CCR |= 1<<0;			//��ʼDMA����  
}
 

// ��ʼ��DMA�ж� cmd ʹ��
// 2022.11.20 RENAME:UART2_DMA_TX_NVIC_Config->DMA_UART2_TX_NVIC_Config
void DMA_UART2_TX_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    if(cmd == ENABLE) {
        DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE); //��������ж�
    } else {
        DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE); //��������ж�
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ADC+DMA:DMA1_Channel1,����(������)��Դ
//��·ADC-ɾ��
//��·ADC
void DMA_ADC_Config(u16 *buffer, u16 len)
{	
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��ʱ��

    DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�ò������Զ���DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴�����Ϊ��Դ
    DMA_InitStructure.DMA_BufferSize = len;//����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ��
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
			
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

// DMA ADC �ж�����
void DMA_ADC_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ClearITPendingBit(DMA_IT_TC); //��������ж�����λ
    if(cmd == ENABLE) {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //��������ж�
    } else {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE); //��������ж�
    }
}


//DMA-ADC1�жϺ���
// 20�����Ӧ�ö���ɺ�,��Send_BlueTooth�е�ͳһ?2023.1.31
u8 cache[2];
void DMA1_Channel1_IRQHandler()  
{  
	if (DMA_GetITStatus(DMA1_IT_TC1)) {                  	//�ж�DMA��������ж�    
		g_AdcDmaFinishFlag = 1;                         	//�ò�����ɱ�־λ  
		
		cache[0] = g_ADCConvertedValue[0] >> 8;
		cache[1] = g_ADCConvertedValue[0];			
		//��������������,��ADC���������ݰ�����UART2,׼�����ڷ���
		QUEUE_DataPush(g_UART_Info->UART_Queue, cache, 2, 20);	

       	DMA_ClearITPendingBit(DMA1_IT_TC1);            		//���DMA�жϱ�־λ
    } 		
}

//DMA-USART2_TX�жϺ���
void DMA1_Channel7_IRQHandler() 
{  
	if (DMA_GetITStatus(DMA1_IT_TC7)) {		//��������ж� 	
		g_Uart2DmaFinishFlag = 0;			//�ò�����ɱ�־λ  
		DMA_Cmd(DMA1_Channel7, DISABLE );  	//�رյ�ǰ��DMA����
		DMA_ClearFlag(DMA1_FLAG_TC7);   	//���DMA��ɱ�־
		DMA_ClearITPendingBit(DMA1_IT_TC7);	//���DMA�жϱ�־λ
	}	
}

///////////////////////////////////////����ɾ����������
//u8 SPI1_DMA_Config(void)
//u8 SPI2_DMA_Config(void)
//u8 SPI3_DMA_Config(void)
////����DMA���� �����ж�
//u8 SPI1_DMA_TX(u8 *buffer,u16 len)
//u8 SPI1_DMA_RX(u8 *buffer,u16 len)
//u8 SPI2_DMA_TX(u8 *buffer,u16 len)
//u8 SPI2_DMA_RX(u8 *buffer,u16 len)

////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI1_DMA_NVIC_Config(u8 cmd)
////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI2_DMA_NVIC_Config(u8 cmd)
////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI3_DMA_NVIC_Config(u8 cmd)

//ADC1
//void DMA1_Channel1_IRQHandler()  
////SPI1_RX/U3_TX
//void DMA1_Channel2_IRQHandler()  
////SPI1_TX/U3_RX
//void DMA1_Channel3_IRQHandler()  
//SPI2_RX/U1_TX
//void DMA1_Channel4_IRQHandler() 
//void DMA1_Channel5_IRQHandler() 
//void DMA1_Channel6_IRQHandler() 
//U2_TX
//void DMA1_Channel7_IRQHandler() 
//SPI3_RX
//void DMA2_Channel1_IRQHandler() 
////SPI3_TX
//void DMA2_Channel2_IRQHandler() 
//U4_RX
//void DMA2_Channel3_IRQHandler() 
//cannle4 
//cannle5 U4_TX
//void DMA2_Channel4_5_IRQHandler() 



