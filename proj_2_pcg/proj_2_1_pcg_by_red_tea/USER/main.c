#include "sys.h"      
#include "delay.h"       
#include "usart.h"      	
#include "led.h"       
#include "Timer.h"
#include "key.h"
#include "dma.h"
#include "adc.h"

#include "string.h"
#include "myQueue.h"
#include "malloc.h"

#include "hc05.h"
//#include "ESP8266.h"



//////////////////////////////////////////////////////////////////////
/*
ATָ�� ������ָ�������޸Ĳ�����
	AT+2000  
	AT+1000
	AT+0500
*/
#define RATE_2000  50
#define RATE_1000  100
#define RATE_500   200

//////////////////////////////////////////////////////////////////////
void SystemInit8 (void);
void SoftReset(void);//�����λ
void Send_BlueTooth(void);
void Send_WIFI(void);
void USART2_rec_EVENT(void);

u8 NImingV7_Sendbuf_Init(void);
//////////////////////////////////////////////////////////////////////

//main
int main(void)
{	
		work_state=SEND_BULE;
////////////////////////////////////////////////////////////////////////////////
//ϵͳ��ʼ�� 72M
		SystemInit();	
		delay_init();	
		delay_ms(100);
		mem_init(SRAMIN);	//�ڴ����	
		LED_IO_Init();		
		KEY_IO_Init();
		HCO5_GPIO_Init();
		//ESP8266_IO_Init();
		uart1_init(460800);//���ڳ�ʼ��
		Main_printf("����\r\n");	
		Main_printf("STM32F103C8 V5.1.2 ��ͨ�������ɼ�  ����Ӳ�� V2.1.5 2022-07-26\r\n");	
////////////////////////////////////////////////////////////////////////////////
		LED1=LED_ON;	LED2=LED_ON;
		delay_ms(1000);		
		LED1=LED_OFF;LED2=LED_OFF;
////////////////////////////////////////////////////////////////////////////////
//		//��ʼ��ϵͳʱ��	 8M		
//		__disable_irq();//�����ж�
//		SystemInit8();
//		SystemCoreClockUpdate();//��ϵͳʱ�ӷ����仯ʱ,�ô˺�������ʱ��
//		__enable_irq();//�����ж�			
//		delay_init();	//��ʱ������ʼ��
//		delay_ms(100);	
//		uart1_init(460800);//���ڼ�DMA��ʼ��
//		Main_printf("��Ƶ��8M\r\n");	
////////////////////////////////////////////////////////////////////////////////			
		TIM1_Init(30000,7200);//������⣨Ԥ�����ܣ�	
		TIM2_Init(10,7200);		//�������ݽ���
		TIM3_Init(10000,7200);//ϵͳָʾ��	
		TIM4_Init(RATE_2000,720);//����������
////////////////////////////////////////////////////////////////////////////////
		//ADC��ʼ��
		Main_printf("DMA ADC ��ʼ��\r\n");
		adc1_init();
		ADC_DMA_Config(ADCConvertedValue,3);
		Main_printf("DMA ADC ��ʼ�����\r\n");		
///////////////////////////////////////////////////////////////////////////
		UART_Info = (_UART_Info*)mymalloc(SRAMIN,sizeof(_UART_Info)); //���нṹ
		UART_Info->UART_Queue =  queue_init(UART_QUEUE_SIZE,UART_QUEUE_LENGTH) ;//ѭ�����г�ʼ��
		UART_Info->sendbuf = (u8*)mymalloc(SRAMIN,UART_SEND_LENGTH);	//���ͻ����� 
		if(UART_Info== NULL|| UART_Info->UART_Queue == NULL ||UART_Info->sendbuf == NULL )
		{
				Main_printf("���ڻ���,�ڴ�����ʧ��\r\n");
		}
		else
		{
				Main_printf("���ڻ���,�ڴ�����ɹ�\r\n");
		}
		Main_printf("�ڴ浱ǰռ�� %d\r\n",mem_perused(SRAMIN));	
///////////////////////////////////////////////////////////////////////////				
		while(1)
		{
				Send_BlueTooth();
				//Send_WIFI();
		}
}	
	



void Send_WIFI(void)
{
//		u16 i,j,z,res;
//		u8 addcheck=0,sumcheck=0;	
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Main_printf("WiFi����\r\n");
//		Main_printf("�ڴ浱ǰռ�� %d\r\n",mem_perused(SRAMIN));	
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Main_printf("WIFI������⣬��������3S��ʼ����\r\n");
//		TIM3_Init(3000,7200);	//WIFI���ü��
//		TIM3_Timing=0;
//		ESP8266_SET_FLAG=0;
//		while(ESP8266_SET_FLAG==0 && TIM3_Timing<10)	//�ȴ�3s
//				ESP8266_SET();	//��ⰴ��
//		if(ESP8266_SET_FLAG)	//����
//		{
//				Set_WIFI();	//WIFIAT����
//		}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Main_printf("��ʼ��WIFI\r\n");
//		Main_printf("��WIFI��DMA�ж�\r\n");
//		TIM3_Init(5000,7200);	//ϵͳָʾ��
//		ESP8266_Init();	//��ʼ��
//		DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)UART_Info->sendbuf);
//		UART2_DMA_TX_NVIC_Config(1);	//�������� DMA�ж�
//		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//DMA
//		UART2_DMA_Finish=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		NImingV7_Sendbuf_Init();	//����������λ����Э��	
/////////////////////////////////////////////////////////////////////////////
//		LED_1=LED_OFF;	LED_2=LED_OFF;
//		TIM3_Init(10000,7200);	//WIFI���ü��
//		Main_printf("��������\r\n");
//		delay_s(1);
//		KEY_TYPE=0;
//		LowPower_flag=0;
//		TIM3_Timing=0;
//		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );//�������
//		ADC_DMA_NVIC_Config(1);//����DMA ADC�ж�			
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		while(1)
//		{	
//				Key_Scan();//�������
//				if(KEY_TYPE)
//				{
//						KEY_TYPE=0;
//						break;
//				}
//////////////////////////////////////////////////////////////////////////////////////
//				if(TIM3_Timing ==10)	//�������
//				{
//						TIM3_Timing=0;
//						BAT_VOL = (u16)((float)ADCConvertedValue[1] / ADCConvertedValue[2]*1200)*2;
//						Main_printf("BAT_VOL %d mV ",BAT_VOL);
//				
////						if(LowPower_flag==0)
////						{
////								if(BAT_VOL<3200)//�͵�������
////								{
////										LowPower_flag=1;
////									#if LOWPOWER ==1//�͹���
////										TIM3_Init(2000,1600);
////									#else
////										TIM3_Init(2000,7200);
////									#endif
////								}
////						}
////						else if(LowPower_flag==1)
////						{
////								if(BAT_VOL<2900)//�͵�������
////								{
////										LowPower_flag=2;
////									#if LOWPOWER ==1//�͹���
////										TIM3_Init(1000,1600);
////									#else
////										TIM3_Init(1000,7200);
////									#endif
////								}
////						}
////						if(LowPower_flag==2)
////						{
////								Main_printf("�͵����Զ��ػ�\r\n");
////								break;
////						}
//				}
//////////////////////////////////////////////////////////////////////////////////////				
//				if(USART2_RX_EVENT)//�������ݽ���
//				{
//						if(UART2_DMA_Finish==0) //����δ��ռ��
//						{
//								USART2_RX_EVENT=0;

//								Main_printf("�������ݽ���:");
//								for(i=0;i<USART2_RX_LEN;i++)
//								{
//										Main_printf("%x ",USART2_RX_BUF[i]);
//								}
//								Main_printf("\r\n");
//						////////////////////////////////////////////////////////////////////////////////////	
//								USART2_rec_EVENT();
//						////////////////////////////////////////////////////////////////////////////////////		
//								USART2_Clear();
//								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//����2�����ж� 
//						}
//				}
//////////////////////////////////////////////////////////////////////////////////////
//				if(UART_Info->Queue_pop_flag==1)	//��������ȡ����־
//				{
//						if(UART2_DMA_Finish==0) //DMA����
//						{
//								UART2_DMA_Finish=1;
//								UART_Info->Queue_pop_flag=0;
//								DMA_Enable(DMA1_Channel7,UART_SEND_LENGTH);//��������
//						}
////						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
////						{
////								UART_Info->UART_Queue->Queue_Full_flag=0;
////								Main_printf("UART queue full\r\n");
////						}
//				}
//				else	if(serch_queue_data(UART_Info->UART_Queue))//����������
//				{
//						for(i=0;i<100;i++)//һ֡100�β���������
//						{
//								//֡1���ݳ���Ϊ8�ֽ�		4��֡ͷ��+2����Ч���ݣ�+2��У�飩=8	
//								UART_Info->sendbuf[0 +i*8]=0xAA;
//								UART_Info->sendbuf[1 +i*8]=0xFF;
//								UART_Info->sendbuf[2 +i*8]=0xF1;	
//								UART_Info->sendbuf[3 +i*8]=2; //��Ч���ݳ��� 8*4=32 8������
//								//MIC
//								UART_Info->sendbuf[4+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+1+i*2); //��λ��ǰ
//								UART_Info->sendbuf[5+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+0+i*2);
//								//У��λ
//								sumcheck = 0;addcheck=0;
//								for(j=0;j< 6 ;j++)
//								{
//											sumcheck += UART_Info->sendbuf[j+i*8];	
//											addcheck += sumcheck;					
//								}	
//								UART_Info->sendbuf[6+i*8] = sumcheck;	//У��		
//								UART_Info->sendbuf[7+i*8] = addcheck;	//У��		
//						}
//						UART_Info->Queue_pop_flag=1;
//						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//ȡ���ݣ���ͷ�����������ݣ���β����
//				}
//////////////////////////////////////////////////////////////////////////////////////
//		}	
//		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//�������										
//		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE );//��ֹ����		
//		queue_clear(UART_Info->UART_Queue);
//		UART2_DMA_TX_NVIC_Config(0);
//		UART2_DMA_Finish=0;
//		ESP8266_PWOEROFF;
}


void Send_BlueTooth(void)
{
		u16 i,j,z,res;
		u8 addcheck=0,sumcheck=0;	
//////////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("��������\r\n");
		Main_printf("�ڴ浱ǰռ�� %d\r\n",mem_perused(SRAMIN));	
//////////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("�������ü��\r\n");
		TIM3_Init(3000,7200);	//WIFI���ü��
		TIM3_Timing=0;
		HC05_SET_FLAG=0;
		while(HC05_SET_FLAG==0 && TIM3_Timing<10)	//�ȴ�3s
				HC05_SET();//�����������	
		if(HC05_SET_FLAG)	//����
		{
				res=HCO5_AT_Confg();//��ʼ����		
				if(res)		
					Main_printf("��������ʧ��\r\n");							
				else									
					Main_printf("�������óɹ�\r\n");			
				Main_printf("ϵͳ����\r\n");			
				TIM_Cmd(TIM3, ENABLE); //����˸
				delay_s(3);
				TIM_Cmd(TIM3, DISABLE); 	
				SoftReset();				
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("�����������ڼ�DMA�ж�\r\n");
		HC05_ON;//������		
		HC05_uart_init(460800);//���ڳ�ʼ�� HC05����	
		DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)UART_Info->sendbuf);
		UART2_DMA_TX_NVIC_Config(1);	//�������� DMA�ж�
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//DMA
		UART2_DMA_Finish=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////
		NImingV7_Sendbuf_Init();	//����������λ����Э��	
///////////////////////////////////////////////////////////////////////////
		LED_1=LED_OFF;	LED_2=LED_OFF;
		TIM3_Init(10000,7200);	//WIFI���ü��
		Main_printf("��������\r\n");
		delay_s(1);
		KEY_TYPE=0;
		LowPower_flag=0;
		TIM3_Timing=0;
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );//�������
		ADC_DMA_NVIC_Config(1);//����DMA ADC�ж�			
//////////////////////////////////////////////////////////////////////////////////////////////////////
		while(1)
		{	
				Key_Scan();//�������
				if(KEY_TYPE)
				{
						KEY_TYPE=0;
						break;
				}
////////////////////////////////////////////////////////////////////////////////////
				if(TIM3_Timing ==10)	//�������
				{
						TIM3_Timing=0;
						BAT_VOL = (u16)((float)ADCConvertedValue[1] / ADCConvertedValue[2]*1200)*2;
						Main_printf("BAT_VOL %d mV ",BAT_VOL);			
						if(LowPower_flag==0)
						{
								if(BAT_VOL<3200)//�͵�������
								{
										LowPower_flag=1;
									#if LOWPOWER ==1//�͹���
										TIM3_Init(2000,1600);
									#else
										TIM3_Init(2000,7200);
									#endif
								}
						}
						else if(LowPower_flag==1)
						{
								if(BAT_VOL<2900)//�͵�������
								{
										LowPower_flag=2;
									#if LOWPOWER ==1//�͹���
										TIM3_Init(1000,1600);
									#else
										TIM3_Init(1000,7200);
									#endif
								}
						}
//						if(LowPower_flag==2)
//						{
//								Main_printf("�͵����Զ��ػ�\r\n");
//								break;
//						}
				}
////////////////////////////////////////////////////////////////////////////////////				
				if(USART2_RX_EVENT)//�������ݽ���
				{
						if(UART2_DMA_Finish==0) //����δ��ռ��
						{
								USART2_RX_EVENT=0;

								Main_printf("�������ݽ���:");
								for(i=0;i<USART2_RX_LEN;i++)
								{
										Main_printf("%x ",USART2_RX_BUF[i]);
								}
								Main_printf("\r\n");
						////////////////////////////////////////////////////////////////////////////////////	
								USART2_rec_EVENT();
						////////////////////////////////////////////////////////////////////////////////////		
								USART2_Clear();
								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//����2�����ж� 
						}
				}
////////////////////////////////////////////////////////////////////////////////////
				if(UART_Info->Queue_pop_flag==1)	//��������ȡ����־
				{
						if(UART2_DMA_Finish==0) //DMA����
						{
								UART2_DMA_Finish=1;
								UART_Info->Queue_pop_flag=0;
								DMA_Enable(DMA1_Channel7,UART_SEND_LENGTH);//��������
						}
						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
						{
								UART_Info->UART_Queue->Queue_Full_flag=0;
								Main_printf("f ");
						}
				}
				else	if(serch_queue_data(UART_Info->UART_Queue))//����������
				{
						for(i=0;i<20;i++)//һ֡100�β���������
						{
								//֡1���ݳ���Ϊ8�ֽ�		4��֡ͷ��+2����Ч���ݣ�+2��У�飩=8	
								UART_Info->sendbuf[0 +i*8]=0xAA;
								UART_Info->sendbuf[1 +i*8]=0xFF;
								UART_Info->sendbuf[2 +i*8]=0xF1;	
								UART_Info->sendbuf[3 +i*8]=2; //��Ч���ݳ��� 8*4=32 8������
								//MIC
								UART_Info->sendbuf[4+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+1+i*2); //��λ��ǰ
								UART_Info->sendbuf[5+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+0+i*2);
								//У��λ
								sumcheck = 0;addcheck=0;
								for(j=0;j< 6 ;j++)
								{
											sumcheck += UART_Info->sendbuf[j+i*8];	
											addcheck += sumcheck;					
								}	
								UART_Info->sendbuf[6+i*8] = sumcheck;	//У��		
								UART_Info->sendbuf[7+i*8] = addcheck;	//У��		
						}
						UART_Info->Queue_pop_flag=1;
						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//ȡ���ݣ���ͷ�����������ݣ���β����
				}
////////////////////////////////////////////////////////////////////////////////////
		}	
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//�������										
		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE );//��ֹ����		
		queue_clear(UART_Info->UART_Queue);
		UART2_DMA_TX_NVIC_Config(0);
		UART2_DMA_Finish=0;
}





////���ݽ��մ���
void USART2_rec_EVENT(void) 
{
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+2000")==0)//��ѯ�ַ���
		{
				Main_printf("�޸Ĳ�����2000\r\n");	
				TIM4_Init(RATE_2000,720);		//2K���� 50
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//�������	
		}
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+1000")==0)//��ѯ�ַ���
		{
				Main_printf("�޸Ĳ�����1000\r\n");
				TIM4_Init(RATE_1000,720);		//1K���� 100
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//�������	
		}
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+0500")==0)//��ѯ�ַ���
		{
				Main_printf("�޸Ĳ�����500\r\n");	
				TIM4_Init(RATE_500,720);		//2K���� 50
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//�������	
		}
}


//�����������ݸ�ʽ	
//32�β���1֡
u8 NImingV7_Sendbuf_Init(void)
{
		u16 i;
		
		u8 datalength = 8;//һ�β���Ҫ���͵����ݳ���

		memset(UART_Info->sendbuf,0,UART_SEND_LENGTH);//����
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//������λ��Э��
		for(i=0;i<100;i++) //һ��N�β��� 32 *8 =256�ֽ�
		{		
					//֡ͷ
					UART_Info->sendbuf[0 + datalength*i] = 0xAA;
					UART_Info->sendbuf[1 + datalength*i] = 0xFF;
					UART_Info->sendbuf[2 + datalength*i] = 0xF1;
					UART_Info->sendbuf[3 + datalength*i] = 2;//��Ч���ݳ���
		}	
		return  0;		
}



//�޸�HCLKʱ��Ϊ8M
void SystemInit8 (void)
{
	  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	
		RCC->CR |= (uint32_t)0x00000001;		//��HSI(�ڲ�����ʱ��8M)
		RCC->CFGR &= (uint32_t)0xF0FF0000;	//RCC_CFGR�Ĵ�����ʼ��									
		RCC->CR &= (uint32_t)0xFEF6FFFF;		//��RCC_CR�Ĵ���HSEON,CSSON,PLLONλ��0
		RCC->CR &= (uint32_t)0xFFFBFFFF;		//��RCC_CR�Ĵ���HSEBYPλ��0
		RCC->CFGR &= (uint32_t)0xFF80FFFF;	//��RCC_CFGR�Ĵ���PLLSRC, PLLXTPRE, 	
		RCC->CIR = 0x009F0000;		//�ر����е��жϺͶ�Ӧ��λ(��ʼ���ж�) 
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);	//ʹ��HSE(�ⲿ����ʱ��)
		do//�ȴ� HSE �����ȶ���������ʱ����
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		//�ж�HSE�Ƿ����  
		if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;
		else			HSEStatus = (uint32_t)0x00;  	
		if (HSEStatus == (uint32_t)0x01)// HSE �����ɹ�����������´���
		{
				// SYSCLK �������������ʱ��ı�������			
				FLASH->ACR |= FLASH_ACR_PRFTBE;//ʹ�� FLASH Ԥ��ȡ������
				FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);//Flash 0�ȴ�״̬		
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;	// 0�� 0 < SYSCLK <= 24M// 1�� 24< SYSCLK <= 48M			// 2�� 48< SYSCLK <= 72M 				    

							
				RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;    /* HCLK = SYSCLK */      
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;    /* PCLK2 = HCLK */ 
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;    /* PCLK1 = HCLK */
				
				//ѡ��HSE��Ϊϵͳʱ��Դ
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
				RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    
				//�ȴ�HSE��Ϊϵͳʱ��Դ
				while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
				{
				}		
		}
		else		
			NVIC_SystemReset(); // ��λ
}


void SoftReset(void)//�����λ
{
		//__disable_irq()ֻ�ǽ�ֹCPUȥ��Ӧ�жϣ�û��������ȥ�����жϵĴ���
		//__enable_irq()�����жϺ�������Ӧ���жϱ�־û����գ�������ᴥ���ж�
    __set_FAULTMASK(1); // �ر������ж�
    NVIC_SystemReset(); // ��λ
}
/**********************************************************************
����������ļ������ݵ����壺
Code����ʾ������ռ�� FLASH �Ĵ�С��FLASH��
RO-data���� Read Only-data�� ��ʾ������ĳ������� const ���ͣ�FLASH��
RW-data���� Read Write-data�� ��ʾ�ѱ���ʼ����ȫ�ֱ�����SRAM��
ZI-data���� Zero Init-data�� ��ʾδ����ʼ����ȫ�ֱ���(SRAM)
***********************************************************************/
