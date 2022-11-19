#include "key.h"
#include "delay.h"
#include "led.h"






//��������Դ���Ƴ�ʼ��
void KEY_IO_Init(void) //IO��ʼ��
{ 
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��
	
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}




volatile u8 KeyMask=0;//�������
volatile u8 KeyDat=0;
u8 KEY_TYPE=0;
void Key_Scan(void)
{
		if ( KEY1 ==0)//KEY����
		{		
				if(KeyDat & 0x01)//��鰴�����±�־λ
				{
						if(TIM1->CNT >= KEYDOWNMINTIME)//��ʱ����1s��������Ч��
						{
								if(TIM1->CNT >= KEYDOWNLONGTIME)//��ʱ����2s����������
								{		
										KeyMask=KEYLONGDOWN;
										LED2=LED_ON;//������Ч								
								}								
								else//�̰����¼�	
								{								
										KeyMask=KEYSHORTDOWN; 	
										LED2=LED_OFF;//������Ч								
								}
						}	
				}
				else//��������˲��
				{			
						TIM_Cmd(TIM1, ENABLE); 
						TIM_Cmd(TIM3, DISABLE); 

						TIM1->CNT=0;//��ն�ʱ��������	
						LED2=LED_ON;//��������˲��
						KeyDat |= 0x01;	//�״ΰ������
						KeyMask=KEYUNKNOW ;//˲�䰴��Ϊ��Ч״̬				
				}
		}	
		else//�ް��°��£����߰�������
		{
				if(KeyDat & 0x01)	//�������º���
				{	
						KeyDat &= ~0x01;//�尴����־
						TIM_Cmd(TIM1, DISABLE);
						TIM_Cmd(TIM3, ENABLE); 

						LED2=LED_OFF;
						if(KeyMask==KEYSHORTDOWN )//�̰����¼�
						{				
								KeyMask=KEYUNKNOW;//�����		
								KEY_TYPE		= KEYSHORTDOWN; 				
						}					
						else if(KeyMask==KEYLONGDOWN)//�������¼�
						{					
								KeyMask=KEYUNKNOW;//�����
								KEY_TYPE		= KEYLONGDOWN; 				
						}
				}		
		}
}				    


