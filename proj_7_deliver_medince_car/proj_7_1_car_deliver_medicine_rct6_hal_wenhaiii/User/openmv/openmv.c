/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "openmv.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "usart.h"
#include "bsp_led.h"
#include "adc.h"
#include "bsp_sys.h"

int GetOpenmvDataCount  = 0;
uint8_t  Num=0, LoR =0, Finded_flag = 0, FindTask = 0;     //()
u8 LastNum;
u8 NumCount = 0;

u8 sendBuf[4];

uint8_t uart3_rxbuff;

u8 RoomNum, TargetNum;
u8 TASK=1;    //���TASK���Դ����openmv����ֵopenmv�ϵ�FindTask������openmvģ��ƥ��Ĳ�ͬģʽ


char TargetRoom = 0;  //A, B, C, D, E, F, G, H;    //��˸��ַ���Ӧ�ŵ�ͼʵ�ʷ��䣬���������3��8�������ӦC-H

u8  FindStartFlag;
u16 FindTimeCount;

void Openmv_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//����
		static uint16_t RxBuffer1[10]={0};   //һ������յ�һ�����õ�����
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

		if(RxState==0&&com_data==0x2C)  //0x2c֡ͷ
		{
			
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;   //һ������£�RxBuffer1[0] ��š�<bbhhhhhb��,Ȼ��RxBuffer1[1]�ŵľ���0x0c
		}

		else if(RxState==1&&com_data==0x12)  //0x12֡ͷ
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}
		
		else if(RxState==2)
		{
			 
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1��������,�������ݽ���
			{
				RxState=3;
				RxFlag1=1;
				
				 //���������,���е���RxCounter1 == 7��  7-5 = 2    openmv���͹�����һ�����ݰ���8��
				Num =          RxBuffer1[RxCounter1-5]; 
				LoR =          RxBuffer1[RxCounter1-4];     //-1���� 1���ң�0��ʾ��û��ʶ���κ�����
				Finded_flag =  RxBuffer1[RxCounter1-3];
				FindTask =      RxBuffer1[RxCounter1-2];
				
				//RxCounter1-1��֡β
				
				//greenLED_Toggle;    //�������Ƿ�������ݵ�,��ƽ��תһ����ɹ�����һ�����ݣ��������һ����˼
		  	GetOpenmvDataCount++;      
				//������1���ڳɹ�������ٸ����ݰ��� ��Ҫ��1s�ӵ���ʱ�������֡��Խ��Խ׼ȷ����λ���Ļ�ƫ��ʹ���
				//�����һ�½�����룬��openmv�����֡��ֱ�Ӵ�����
				
			}
		}

		else if(RxState==3)		//����Ƿ���ܵ�������־
		{
				if(RxBuffer1[RxCounter1-1] == 0x5B)
				{
							
							RxFlag1 = 0;
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //���մ���
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<10;i++)
							{
									RxBuffer1[i]=0x00;      //�����������������
							}
				}
		} 

		else   //�����쳣
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<10;i++)
				{
						RxBuffer1[i]=0x00;      //�����������������
				}
		}
}





void SendDataToOpenmv(void)
{
	u8 i;
	//���Ϸ��͸�openmv �����ݵĴ��� (֡ͷ�� ģ��ƥ��ģʽѡ���־λ��ģʽ2����Ҫƥ������֣�֡β)   //����Ҫ�ܸߵķ���Ƶ��
		
		
		for(i = 0; i <= 4; i++)   //��TASK��TargetNum���һ���Է��͸�openmv
		{
			sprintf((char *)sendBuf, "*%d%d&", TASK, TargetNum);
				HAL_UART_Transmit(&huart3, sendBuf, sizeof(sendBuf), 1000);
			  //YellowSignal_Toggle;
		}
		//greenLED_off;
}


//�ȴ�openmvʶ��ָ������,������Ŀ�귿�䡣 ֻ���ڸ�λ��
void SetTargetRoom(void)
{
		//��ѰĿ�겡���ŵĻ��������ݣ���ת����ʼ����   //�������һ�δ�openmv���������ּ�ΪĿ�귿��
		/*һ��ʼʶ��Ŀ�귿���*/  
		if(Finded_flag == 1)
		{
			/**�˲�**/ //���Ե�û��Ҫ
//			if(NumCount == 0)   
//			{
//				LastNum = Num;
//			}
//			
//			else if(NumCount >= 2)  //������������һ���ģ����ж�Ϊʶ��
//			{
//				if(LastNum == Num)
//				{
//					RoomNum = Num;    //�б�Ҫ�ö�һ��������Ҫ�˲����б�Ҫ,����ֱ����Num����
//					LastNum = 0;
//					NumCount = 0;
//				}
//				
//			}
//			NumCount++;
			
			 RoomNum = Num;
			 TASK = 2;    //openmv�Ǳ�û��ȫ��ͼƬ����ȥǰ����Ҫ��TASK��ֵΪ2
			
		}
		
		
		else if(Finded_flag == 0)
		{
			RoomNum = 0;   
			LastNum = 0;
			NumCount = 0;
		}
	
		
	 if(RoomNum ==  1) 
	 {
		 TargetRoom = 'A';
		 
		 
	 }
	 else if(RoomNum == 2)
	 {
		 TargetRoom = 'B';
		 
			
	 }
	 else if(RoomNum >= 3)  //����else if(3 <= Num <= 8)
	 {
		 TargetRoom = 'E';  //��һ����ò�һ��
		 
	 }
	 
   switch(RoomNum)
		{
			case 1:
				TargetNum = 1;
			break;
			
			case 2:
				TargetNum = 2;
			break;
			
			case 3:
				TargetNum = 3;
			break;
			
			case 4:
				TargetNum = 4;
			break;
			
			case 5:
				TargetNum = 5;
			break;
			
			case 6:
				TargetNum = 6;
			break;

			case 7:
				TargetNum = 7;
			break;
			
			case 8:
				TargetNum = 8;
			break;	 		
		}
	 //ʶ�𵽵�������3-8�� Ĭ���ȸ�  TargetRoom = RoomH
	 //����openmvʶ�𵽵�����,����ҩ�ĺ����������Ŀ��ֵ��ʵʱ����
}
