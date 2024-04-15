/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "bsp_BlueTooth.h"
#include <stdio.h>
#include "usart.h"
#include "string.h"
#include "stm32f1xx.h"

u8 uart2_rxbuff;

u8 OneTargetRoom, OneLoadFlag, OneArriveFlag;   //����һ��״̬�ı�־λ
u16 BlueRxBuffer[7]={0};

//Ҫ��Э�鱣֤ͨ�ſɿ��Ļ�������Ҳ���õ����ڻص�����
//һ����ZET6����������������������RCT6�����������ӻ�
void BlueTooth_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t BlueRxCounter=0;//����
	  
		static uint8_t BlueRxState = 0;	
		//static uint8_t BlueRxFlag = 0;    //�����ɹ����ձ�־λ
	 //�����ɹ����ձ�־λ��ʵ���Բ�Ҫ����OneTargertNum��OneLoadFlagֱ�Ӳ�����һ�ε�Ҳûɶ������һ�㲻����ֿ�����������ܿ��ˢ��������
	 //��û��Ҫ����߽�OneTargertNum��OneLoadFlagʹ��һ��֮�����BlueRxFlagһͬ����

		if(BlueRxState==0&&com_data==0x52)  //0x52֡ͷ
		{
			
			BlueRxState=1;
			BlueRxBuffer[BlueRxCounter++]=com_data;  
		}

		else if(BlueRxState==1&&com_data==0x21)  //0x21֡ͷ
		{
			BlueRxState=2;
			BlueRxBuffer[BlueRxCounter++]=com_data;
		}
		
		else if(BlueRxState==2)
		{
			 
			BlueRxBuffer[BlueRxCounter++]=com_data;
			if(BlueRxCounter>=7||com_data == 0xf2)    
			{
				BlueRxState=3;
				
				//BlueRxFlag1=1;    //�ɹ�����һ�����ݣ������������ж��Ƿ������������ 
				
//				OneTargertRoom =  BlueRxBuffer1[BlueRxCounter-5];     //[BlueRxCounter-5]��R  Ҳ��Ҳ��82��Ҳ��ʮ�����Ƶ�52    ��ȷ��֡ͷ1 
//				OneLoadFlag =  BlueRxBuffer1[BlueRxCounter-4];  //[BlueRxCounter-4]  ������ʮ���Ƶ�33��Ҳ��ʮ�����Ƶ�21      ��ȷ��֡ͷ2    
				
				OneTargetRoom =  BlueRxBuffer[BlueRxCounter-3];        
				OneLoadFlag =  BlueRxBuffer[BlueRxCounter-2];          
				
				
				//���������������RxCounter1-1�ŵ���֡β

			}
		}

		else if(BlueRxState==3)		//����Ƿ���ܵ�������־
		{
				//if(BlueRxBuffer[BlueRxState-1] == 0xf2)    //����д������������������BlueRxState1-1������BlueRxCounter1-1��������ȫ�����Զ�������
			
			  if(BlueRxBuffer[BlueRxCounter-1] == 0xf2)    
				{
							
							//BlueRxFlag = 0;
							BlueRxState = 0;
							BlueRxCounter = 0;
						
				}
				else   //���մ���
				{
							BlueRxState  = 0;
							BlueRxCounter =0;
							for(i=0;i<7;i++)
							{
									BlueRxBuffer[i]=0x00;      //�����������������
							}
				}
		} 

		else   //�����쳣
		{
				BlueRxState = 0;
				BlueRxCounter = 0;
				for(i=0;i<7;i++)
				{
						BlueRxBuffer[i]=0x00;      //�����������������
				}
		}
}


