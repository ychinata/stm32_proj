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


u8 UART1_DMA_Finish;
u8 UART2_DMA_Finish;
u8 UART3_DMA_Finish;
u8 UART4_DMA_Finish;

u8 SPI1_DMA_Finish;
u8 SPI2_DMA_Finish;
u8 SPI3_DMA_Finish;

u8 ADC_DMA_Finish;

 
/////////////////////////////////////////////////////////////////////////////////////////////
//DMA�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�ڴ��ַ
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
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

//void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		RCC->AHBENR |= 1<<0;				//����DMA1ʱ��		
//		delay_ms(10);
//	
//		//DMA_CHx -> CNDTR = len;	//���ݴ��������65535
//		DMA_CHx -> CPAR = (u32)cpar;    //�趨����Ĵ�����ַ ���͵�ַ
//		DMA_CHx -> CMAR = (u32)cmar;    //�趨���ݴ洢����ַ ���յ�ַ
//		
//		//CCR	�Ĵ��� 0-14Ϊ����λ		
//		DMA_CHx -> CCR  = 0;  							//��λ������Ĵ���
//		
//		//�����ε��жϣ����жϺ����в�Ҫ�ж����־λ����Ȼ�ᴥ���������ԭ��
//		DMA_CHx -> CCR  	|= 0<<1;         //��������ж�				1���� 0��ֹ
//		DMA_CHx -> CCR  	|= 0<<2;         //�봫���ж� 				1���� 0��ֹ
//		DMA_CHx -> CCR		|= 0<<3;         //����������ж�		1���� 0��ֹ	  ��дһ�������ĵ�ַ����,�������DMA������� 
//	
//		//�趨���ݴ��䷽��
//		DMA_CHx -> CCR  |= 1<<4;           //�趨���ݴ��䷽��   0�������贫�����ڴ� 1�����ڴ洫��������
//		DMA_CHx -> CCR  |= 0<<5;           //0����ִ��ѭ������ 1��ִ��ѭ������           
//		//�趨����ʹ洢����ַ����
//		DMA_CHx -> CCR  |= 0<<6;           //0�������ַ������ 1�������ַ����
//		DMA_CHx -> CCR  |= 1<<7;           //0���ڴ��ַ������ 1���ڴ��ַ����    
//		//�趨�������ݿ��
//		DMA_CHx -> CCR  |= 0<<8;           //�������ݿ��,��[9:8]��λ����
//		DMA_CHx -> CCR  |= 0<<9;           //00��8λ 01��16λ 10��32λ 11������  
//		//�趨�洢���ݿ��
//		DMA_CHx -> CCR  |= 0<<10;          //�洢�����ݿ��,��[11:10]��λ����
//		DMA_CHx -> CCR  |= 0<<11;          //00��8λ 01��16λ 10��32λ 11������  			
//		//�趨Ϊ�е����ȼ�
//		DMA_CHx -> CCR  |= 0<<12;          //ͨ�����ȼ�,��[13:12]��λ����
//		DMA_CHx -> CCR  |= 0<<13;          //00���� 01���� 10���� 11����� 
//		//�洢��ʽ
//		DMA_CHx -> CCR  |= 0<<14;          //0�����ڴ浽�ڴ棻 1���ڴ洫�䵽�ڴ档     
//} 

////����һ��DMA����
//void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
//{
//		DMA_Cmd(DMA_CHx, DISABLE );  //�رյ�ǰ��DMA���䣨������û�д�����ɣ�
//		DMA_SetCurrDataCounter(DMA_CHx,len);//Ҫ���͵����ݳ���
//		DMA_Cmd(DMA_CHx, ENABLE);  //��ʼDMA����  	  
//}

//����һ��DMA����
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
    DMA_CHx -> CCR &= ~(1<<0);	//�رյ�ǰ��DMA���䣨������û�д�����ɣ�
    DMA_CHx -> CNDTR = len;			//�������ݴ�����
    DMA_CHx -> CCR |= 1<<0;			//��ʼDMA����  
}
 


//��ʼ��DMA�ж� cmd 1 ʹ��
void UART1_DMA_TX_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    if(cmd==1)
    {
        DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
    else
    {
        DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
}
void UART2_DMA_TX_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    if(cmd==1)
    {
            DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
            //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
    else
    {
            DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
            //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
}
//void UART3_DMA_TX_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//	
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE); //��������ж�
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,DISABLE); //��������ж�
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
//		}
//}
//void UART4_DMA_TX_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA2_Channel4_5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//	
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE); //��������ж�
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
//		}
//		else
//		{
//				DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,DISABLE); //��������ж�
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
//		}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��ADCDMA
//��·ADC
//void ADC_DMA_Config(u16 *buffer ,u16 len)
//{	
//		DMA_InitTypeDef DMA_InitStructure;
//	
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��ʱ��
//	
//    DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�ò������Զ���DMA�������ַ
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴���������Ϊ��Դ
//    DMA_InitStructure.DMA_BufferSize = len;//����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ��
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ�����ڴ浽�ڴ洫��
//    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
//		
//		DMA_Cmd(DMA1_Channel1, ENABLE);
//		//DMA_Cmd(DMA1_Channel1, DISABLE);
//}

//��·ADC
void ADC_DMA_Config(u16 *buffer ,u16 len)
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
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//ʹ��DMAͨ�����ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
			
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

////ͬ��ADC1 ADC2
//void ADC_DMA_Config(u32 *buffer ,u16 len)
//{	
//		DMA_InitTypeDef DMA_InitStructure;
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��ʱ��

//    DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�ò������Զ���DMA�������ַ
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴�����Ϊ��Դ
//    DMA_InitStructure.DMA_BufferSize = len;//����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ��
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//���ݿ��Ϊ32λ
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//���ݿ��Ϊ32λ
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//ʹ��DMAͨ�����ڴ浽�ڴ洫��
//    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
//			
//		DMA_Cmd(DMA1_Channel1, ENABLE);
//}


void ADC_DMA_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ClearITPendingBit(DMA_IT_TC); //��������ж�����λ
    if(cmd==1)
    {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
    else
    {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE); //��������ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //�뷢���ж�
        //DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //���ʹ����ж�
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//u8 SPI1_DMA_Config(void)
//{
//		DMA_InitTypeDef DMA_InitStructure;

//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
//		delay_ms(10);

//		//DMAͨ��2����SPI->RAM�����ݴ��� SPI1 RX
//		DMA_DeInit(DMA1_Channel2);	
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI1->DR;
//		//DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ//���浥������
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
//		DMA_InitStructure.DMA_BufferSize=512;	//���浥������
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh; 
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel2,&DMA_InitStructure); //��DMAͨ��2���г�ʼ��

//		//DMAͨ��3����RAM->SPI�����ݴ���		SPI TX
//		DMA_DeInit(DMA1_Channel3);
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI1->DR;
//		//DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ//���浥������
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
//		DMA_InitStructure.DMA_BufferSize=512;	//���浥������
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh; 
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel3,&DMA_InitStructure); //��DMAͨ��3���г�ʼ��

//		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE); //ʹ��SPI��DMA��������
//		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE); //ʹ��SPI��DMA��������

//		return 0;
//}
//u8 SPI2_DMA_Config(void)
//{
//	 DMA_InitTypeDef DMA_InitStructure;

//	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
//		
//	 //DMAͨ��4����SPI->RAM�����ݴ���
//	 DMA_DeInit(DMA1_Channel4);	
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI2->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ ���浥������
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
//	 DMA_InitStructure.DMA_BufferSize=512;	//���浥������
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA1_Channel4,&DMA_InitStructure); //��DMAͨ��4���г�ʼ��
//	 	 
//	//DMAͨ��5����RAM->SPI�����ݴ��� SPI TX
//	 DMA_DeInit(DMA1_Channel5);
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI2->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ ���浥������
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
//	 DMA_InitStructure.DMA_BufferSize=512;	//���浥������
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA1_Channel5,&DMA_InitStructure); //��DMAͨ��5���г�ʼ��
//	 
//	 SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE); //ʹ��SPI��DMA��������
//	 SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE); //ʹ��SPI��DMA��������

//	 return 0;
//}

//u8 SPI3_DMA_Config(void)
//{
//	 DMA_InitTypeDef DMA_InitStructure;
//	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
//	
//	 //DMAͨ��5����RAM->SPI�����ݴ��� SPI TX
//	 DMA_DeInit(DMA2_Channel2);
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI3->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
//	 DMA_InitStructure.DMA_BufferSize=512;
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA2_Channel2,&DMA_InitStructure);
//		
//	 //DMAͨ��4����SPI->RAM�����ݴ��� RX
//	 DMA_DeInit(DMA2_Channel1);	
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI3->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //��������ǰװ��ʵ��RAM��ַ
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
//	 DMA_InitStructure.DMA_BufferSize=512;
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA2_Channel1,&DMA_InitStructure); //��DMAͨ��4���г�ʼ��
//		 
//	 SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE); //ʹ��SPI��DMA��������
//	 SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Rx,ENABLE); //ʹ��SPI��DMA��������

//	 return 0;
//}


////����DMA���� �����ж�
//u8 SPI1_DMA_TX(u8 *buffer,u16 len)
//{
//		DMA1_Channel3->CNDTR=len; //����Ҫ��������ݳ���
//		DMA1_Channel3->CMAR=(uint32_t)buffer; //����RAM��������ַ
//		
//		DMA_Cmd(DMA1_Channel3,ENABLE); //����DMA���� RAM->SPI
//	
//		while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)); //�ȴ�DMAͨ��3�������
//		DMA_ClearFlag(DMA1_FLAG_TC3); //���ͨ��3�������״̬���
//		DMA_Cmd(DMA1_Channel3,DISABLE); //ʹDMAͨ��3ֹͣ����
//	  return 0;
//}
//u8 SPI1_DMA_temp=0xff;
////����DMA���� 
//u8 SPI1_DMA_RX(u8 *buffer,u16 len)
//{   	
//		DMA1_Channel2->CNDTR=len; //���ô�������ݳ���
//		DMA1_Channel2->CMAR=(uint32_t)buffer; //�����ڴ滺������ַ
//		
//		/*SPI��Ϊ�����������ݽ���ʱ����Ҫ��������ʱ�ӣ���˴˴�������DMAͨ��3�����*/
//		DMA1_Channel3->CNDTR=len; 
//		DMA1_Channel3->CMAR=(uint32_t)&SPI1_DMA_temp; //temp=0xff
//		DMA1_Channel3->CCR&=~DMA_MemoryInc_Enable; //�ڴ��ַ������
//		
//		DMA_Cmd(DMA1_Channel2,ENABLE); //������RX
//		DMA_Cmd(DMA1_Channel3,ENABLE); //������TX
//	
//		while(!DMA_GetFlagStatus(DMA1_FLAG_TC2)); //�ȴ�DMAͨ��2�����������
//		DMA_ClearFlag(DMA1_FLAG_TC2); 
//		DMA_ClearFlag(DMA1_FLAG_TC3); //���DMAͨ��2��3�Ĵ�����ɱ�־
//		DMA_Cmd(DMA1_Channel2,DISABLE);
//		DMA_Cmd(DMA1_Channel3,DISABLE); //ʹDMAͨ��2��3ֹͣ����
//		DMA1_Channel3->CCR|=DMA_MemoryInc_Enable; //��DMAͨ��3�ָ�Ϊ�ڴ��ַ������ʽ
//	  return 0;
//}



////����DMA����
//u8 SPI2_DMA_TX(u8 *buffer,u16 len)
//{
//		DMA1_Channel5->CNDTR=len; //����Ҫ��������ݳ���
//		DMA1_Channel5->CMAR=(uint32_t)buffer; //����RAM��������ַ
//		
//		DMA_Cmd(DMA1_Channel5,ENABLE); //����DMA���� RAM->SPI
////		while(!DMA_GetFlagStatus(DMA1_FLAG_TC5)); //�ȴ�DMAͨ��5�������
////		DMA_ClearFlag(DMA1_FLAG_TC5); //���ͨ��5�������״̬���
////		DMA_Cmd(DMA1_Channel5,DISABLE); //ʹDMAͨ��5ֹͣ����
//	  return 0;
//}
//u8 SPI2_DMA_temp=0xff;
////����DMA����
//u8 SPI2_DMA_RX(u8 *buffer,u16 len)
//{	
//		DMA1_Channel4->CNDTR=len; 						//���ô�������ݳ���
//		DMA1_Channel4->CMAR=(uint32_t)buffer; //�����ڴ滺������ַ
//		
//		/*SPI��Ϊ�����������ݽ���ʱ����Ҫ��������ʱ�ӣ���˴˴�������DMAͨ��5�����*/
//		DMA1_Channel5->CNDTR=len; 
//		DMA1_Channel5->CMAR=(uint32_t)&SPI2_DMA_temp; //temp=0xff
//		DMA1_Channel5->CCR&=~DMA_MemoryInc_Enable; //�ڴ��ַ������
//		
//		DMA_Cmd(DMA1_Channel4,ENABLE); //������RX
//		DMA_Cmd(DMA1_Channel5,ENABLE); //������TX
//	
////		while(!DMA_GetFlagStatus(DMA1_FLAG_TC4)); //�ȴ�DMAͨ��4�����������
////		DMA_ClearFlag(DMA1_FLAG_TC4); 
////		DMA_ClearFlag(DMA1_FLAG_TC5); //���DMAͨ��4��5�Ĵ�����ɱ�־
////		DMA_Cmd(DMA1_Channel4,DISABLE);
////		DMA_Cmd(DMA1_Channel5,DISABLE); //ʹDMAͨ��4��5ֹͣ����
////		
////		DMA1_Channel5->CCR|=DMA_MemoryInc_Enable; //��DMAͨ��5�ָ�Ϊ�ڴ��ַ������ʽ
//	  return 0;
//}



////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI1_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel3_IRQn; //TX �ж�
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE); //��������ж�
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,DISABLE); //��������ж�
//		}
//}
////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI2_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		DMA_ClearITPendingBit(DMA1_IT_TC5);//���DMA�жϱ�־λ
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel5_IRQn; //��������ж�
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE); //��������ж�
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE); //��������ж�
//		}	
//}
////��ʼ��DMA�ж� cmd 1 ʹ��
//void SPI3_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA2_Channel2_IRQn; //TX �ж�
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ� 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA2_Channel2,DMA_IT_TC,ENABLE); //��������ж�
//		}
//		else
//		{
//				DMA_ITConfig(DMA2_Channel2,DMA_IT_TC,DISABLE); //��������ж�
//		}
//}

////////////////////////////////////////////////////////////////////////////////
//ADC1
u8 cache[2];
void DMA1_Channel1_IRQHandler()  
{  
    if(DMA_GetITStatus(DMA1_IT_TC1))                   //�ж�DMA��������ж�  
    {  
        ADC_DMA_Finish = 1;                         		 //�ò�����ɱ�־λ  
		/////////////////////////////////////////////////////////////////////////////	
        cache[0]=ADCConvertedValue[0]>>8;
        cache[1]=ADCConvertedValue[0];
    

        queue_data_push(UART_Info->UART_Queue,cache,2,20);//��������������
		/////////////////////////////////////////////////////////////////////////////	
       	DMA_ClearITPendingBit(DMA1_IT_TC1);            //���DMA�жϱ�־λ
    } 		
}
////////////////////////////////////////////////////////////////////////////////
////SPI1_RX/U3_TX
//void DMA1_Channel2_IRQHandler()  
//{     
//		if(DMA_GetITStatus(DMA1_IT_TC2))                   //�ж�DMA��������ж�  
//		{
//        UART3_DMA_Finish = 1;                          //�ò�����ɱ�־λ  
//				DMA_ClearITPendingBit(DMA1_IT_TC2);           //���DMA�жϱ�־λ
//				DMA_Cmd(DMA1_Channel2, DISABLE );  //�رյ�ǰ��DMA����
//		}	
//}
////////////////////////////////////////////////////////////////////////////////
////SPI1_TX/U3_RX
//void DMA1_Channel3_IRQHandler()  
//{  
//    if(DMA_GetITStatus(DMA1_IT_TC3))                   //���ʹ�������ж�  
//    {  
//				DMA_Cmd(DMA1_Channel3, DISABLE );  							 //�رյ�ǰ��DMA����

//				SPI1_DMA_Finish=0;			//DMA���� 
//			  DMA_ClearITPendingBit(DMA1_IT_TC3);            //���DMA�жϱ�־λ
//    }	
//}
////////////////////////////////////////////////////////////////////////////////
//SPI2_RX/U1_TX
void DMA1_Channel4_IRQHandler() 
{  
    if(DMA_GetITStatus(DMA1_IT_TC4))     //��������ж�  
    {
        UART1_DMA_Finish = 0;              //�ò�����ɱ�־λ  
        DMA_Cmd(DMA1_Channel4, DISABLE );  //�رյ�ǰ��DMA����
        DMA_ClearFlag(DMA1_FLAG_TC4);   	 //���DMA��ɱ�־
        DMA_ClearITPendingBit(DMA1_IT_TC4);//���DMA�жϱ�־λ
    }	
}
////////////////////////////////////////////////////////////////////////////////
////SPI2_TX/U1_RX
//void DMA1_Channel5_IRQHandler() 
//{  

//	
//}
////////////////////////////////////////////////////////////////////////////////
////U2_RX
//void DMA1_Channel6_IRQHandler() 
//{  
//		if(DMA_GetITStatus(DMA1_IT_TC6))
//		{
//				DMA_ClearITPendingBit(DMA1_IT_TC6);//���DMA�жϱ�־λ
//		}	
//}
////////////////////////////////////////////////////////////////////////////////
//U2_TX
void DMA1_Channel7_IRQHandler() 
{  
    if(DMA_GetITStatus(DMA1_IT_TC7))	//��������ж� 
    {
        UART2_DMA_Finish = 0;              //�ò�����ɱ�־λ  
        DMA_Cmd(DMA1_Channel7, DISABLE );  //�رյ�ǰ��DMA����
        DMA_ClearFlag(DMA1_FLAG_TC7);   	 //���DMA��ɱ�־
        DMA_ClearITPendingBit(DMA1_IT_TC7);//���DMA�жϱ�־λ
    }	
}
////////////////////////////////////////////////////////////////////////////////
//SPI3_RX
//void DMA2_Channel1_IRQHandler() 
//{
//		if(DMA_GetITStatus(DMA2_IT_TC1))
//		{
//				DMA_ClearITPendingBit(DMA2_IT_TC1);//���DMA�жϱ�־λ
//		}
//}
////////////////////////////////////////////////////////////////////////////////
////SPI3_TX
//void DMA2_Channel2_IRQHandler() 
//{
//		if(DMA_GetITStatus(DMA2_IT_TC2))                  //�ж�DMA��������ж�  
//		{
//				SPI3_DMA_Finish = 0;                          //�ò�����ɱ�־λ  
//				DMA_Cmd(DMA2_Channel2, DISABLE );  						//�رյ�ǰ��DMA����
//				DMA_ClearITPendingBit(DMA2_IT_TC2);           //���DMA�жϱ�־λ
//		}	
//}
////////////////////////////////////////////////////////////////////////////////
//U4_RX
//void DMA2_Channel3_IRQHandler() 
//{
//		if(DMA_GetITStatus(DMA2_IT_TC3))
//		{
//				DMA_ClearITPendingBit(DMA2_IT_TC3);//���DMA�жϱ�־λ
//		}
//}
////////////////////////////////////////////////////////////////////////////////
//cannle4 
//cannle5 U4_TX
//void DMA2_Channel4_5_IRQHandler() 
//{
//		if(DMA_GetITStatus(DMA2_IT_TC5))                   //�ж�DMA��������ж�  
//		{
//        UART4_DMA_Finish = 0;                          //�ò�����ɱ�־λ  
//				DMA_Cmd(DMA2_Channel5, DISABLE );  						 //�رյ�ǰ��DMA����
//				DMA_ClearFlag(DMA2_FLAG_TC5);   	 						 //���DMA��ɱ�־
//				DMA_ClearITPendingBit(DMA2_IT_TC5);            //���DMA�жϱ�־λ
//		}
//}
////////////////////////////////////////////////////////////////////////////////
