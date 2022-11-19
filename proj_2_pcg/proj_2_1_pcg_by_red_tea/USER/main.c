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
AT指令 下面条指令用于修改采样率
	AT+2000  
	AT+1000
	AT+0500
*/
#define RATE_2000  50
#define RATE_1000  100
#define RATE_500   200

//////////////////////////////////////////////////////////////////////
void SystemInit8 (void);
void SoftReset(void);//软件复位
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
//系统初始化 72M
		SystemInit();	
		delay_init();	
		delay_ms(100);
		mem_init(SRAMIN);	//内存管理	
		LED_IO_Init();		
		KEY_IO_Init();
		HCO5_GPIO_Init();
		//ESP8266_IO_Init();
		uart1_init(460800);//串口初始化
		Main_printf("开机\r\n");	
		Main_printf("STM32F103C8 V5.1.2 单通道心音采集  配套硬件 V2.1.5 2022-07-26\r\n");	
////////////////////////////////////////////////////////////////////////////////
		LED1=LED_ON;	LED2=LED_ON;
		delay_ms(1000);		
		LED1=LED_OFF;LED2=LED_OFF;
////////////////////////////////////////////////////////////////////////////////
//		//初始化系统时钟	 8M		
//		__disable_irq();//关总中断
//		SystemInit8();
//		SystemCoreClockUpdate();//当系统时钟发生变化时,用此函数更新时钟
//		__enable_irq();//开总中断			
//		delay_init();	//延时函数初始化
//		delay_ms(100);	
//		uart1_init(460800);//串口及DMA初始化
//		Main_printf("降频到8M\r\n");	
////////////////////////////////////////////////////////////////////////////////			
		TIM1_Init(30000,7200);//按键检测（预留功能）	
		TIM2_Init(10,7200);		//串口数据解析
		TIM3_Init(10000,7200);//系统指示灯	
		TIM4_Init(RATE_2000,720);//采样率设置
////////////////////////////////////////////////////////////////////////////////
		//ADC初始化
		Main_printf("DMA ADC 初始化\r\n");
		adc1_init();
		ADC_DMA_Config(ADCConvertedValue,3);
		Main_printf("DMA ADC 初始化完成\r\n");		
///////////////////////////////////////////////////////////////////////////
		UART_Info = (_UART_Info*)mymalloc(SRAMIN,sizeof(_UART_Info)); //队列结构
		UART_Info->UART_Queue =  queue_init(UART_QUEUE_SIZE,UART_QUEUE_LENGTH) ;//循环队列初始化
		UART_Info->sendbuf = (u8*)mymalloc(SRAMIN,UART_SEND_LENGTH);	//发送缓冲区 
		if(UART_Info== NULL|| UART_Info->UART_Queue == NULL ||UART_Info->sendbuf == NULL )
		{
				Main_printf("串口缓存,内存申请失败\r\n");
		}
		else
		{
				Main_printf("串口缓存,内存申请成功\r\n");
		}
		Main_printf("内存当前占用 %d\r\n",mem_perused(SRAMIN));	
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
//		Main_printf("WiFi发送\r\n");
//		Main_printf("内存当前占用 %d\r\n",mem_perused(SRAMIN));	
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Main_printf("WIFI配网检测，长按按键3S开始配网\r\n");
//		TIM3_Init(3000,7200);	//WIFI设置检测
//		TIM3_Timing=0;
//		ESP8266_SET_FLAG=0;
//		while(ESP8266_SET_FLAG==0 && TIM3_Timing<10)	//等待3s
//				ESP8266_SET();	//检测按键
//		if(ESP8266_SET_FLAG)	//配网
//		{
//				Set_WIFI();	//WIFIAT配网
//		}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Main_printf("初始化WIFI\r\n");
//		Main_printf("开WIFI及DMA中断\r\n");
//		TIM3_Init(5000,7200);	//系统指示灯
//		ESP8266_Init();	//初始化
//		DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)UART_Info->sendbuf);
//		UART2_DMA_TX_NVIC_Config(1);	//开启串口 DMA中断
//		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//DMA
//		UART2_DMA_Finish=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		NImingV7_Sendbuf_Init();	//适配匿名上位机的协议	
/////////////////////////////////////////////////////////////////////////////
//		LED_1=LED_OFF;	LED_2=LED_OFF;
//		TIM3_Init(10000,7200);	//WIFI设置检测
//		Main_printf("发送数据\r\n");
//		delay_s(1);
//		KEY_TYPE=0;
//		LowPower_flag=0;
//		TIM3_Timing=0;
//		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );//允许更新
//		ADC_DMA_NVIC_Config(1);//开启DMA ADC中断			
////////////////////////////////////////////////////////////////////////////////////////////////////////
//		while(1)
//		{	
//				Key_Scan();//按键检测
//				if(KEY_TYPE)
//				{
//						KEY_TYPE=0;
//						break;
//				}
//////////////////////////////////////////////////////////////////////////////////////
//				if(TIM3_Timing ==10)	//电量监测
//				{
//						TIM3_Timing=0;
//						BAT_VOL = (u16)((float)ADCConvertedValue[1] / ADCConvertedValue[2]*1200)*2;
//						Main_printf("BAT_VOL %d mV ",BAT_VOL);
//				
////						if(LowPower_flag==0)
////						{
////								if(BAT_VOL<3200)//低电量报警
////								{
////										LowPower_flag=1;
////									#if LOWPOWER ==1//低功耗
////										TIM3_Init(2000,1600);
////									#else
////										TIM3_Init(2000,7200);
////									#endif
////								}
////						}
////						else if(LowPower_flag==1)
////						{
////								if(BAT_VOL<2900)//低电量待机
////								{
////										LowPower_flag=2;
////									#if LOWPOWER ==1//低功耗
////										TIM3_Init(1000,1600);
////									#else
////										TIM3_Init(1000,7200);
////									#endif
////								}
////						}
////						if(LowPower_flag==2)
////						{
////								Main_printf("低电量自动关机\r\n");
////								break;
////						}
//				}
//////////////////////////////////////////////////////////////////////////////////////				
//				if(USART2_RX_EVENT)//串口数据接收
//				{
//						if(UART2_DMA_Finish==0) //串口未被占用
//						{
//								USART2_RX_EVENT=0;

//								Main_printf("串口数据接收:");
//								for(i=0;i<USART2_RX_LEN;i++)
//								{
//										Main_printf("%x ",USART2_RX_BUF[i]);
//								}
//								Main_printf("\r\n");
//						////////////////////////////////////////////////////////////////////////////////////	
//								USART2_rec_EVENT();
//						////////////////////////////////////////////////////////////////////////////////////		
//								USART2_Clear();
//								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//串口2接收中断 
//						}
//				}
//////////////////////////////////////////////////////////////////////////////////////
//				if(UART_Info->Queue_pop_flag==1)	//队列数据取出标志
//				{
//						if(UART2_DMA_Finish==0) //DMA空闲
//						{
//								UART2_DMA_Finish=1;
//								UART_Info->Queue_pop_flag=0;
//								DMA_Enable(DMA1_Channel7,UART_SEND_LENGTH);//发送数据
//						}
////						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
////						{
////								UART_Info->UART_Queue->Queue_Full_flag=0;
////								Main_printf("UART queue full\r\n");
////						}
//				}
//				else	if(serch_queue_data(UART_Info->UART_Queue))//队列有数据
//				{
//						for(i=0;i<100;i++)//一帧100次采样的数据
//						{
//								//帧1数据长度为8字节		4（帧头）+2（有效数据）+2（校验）=8	
//								UART_Info->sendbuf[0 +i*8]=0xAA;
//								UART_Info->sendbuf[1 +i*8]=0xFF;
//								UART_Info->sendbuf[2 +i*8]=0xF1;	
//								UART_Info->sendbuf[3 +i*8]=2; //有效数据长度 8*4=32 8个导联
//								//MIC
//								UART_Info->sendbuf[4+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+1+i*2); //低位在前
//								UART_Info->sendbuf[5+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+0+i*2);
//								//校验位
//								sumcheck = 0;addcheck=0;
//								for(j=0;j< 6 ;j++)
//								{
//											sumcheck += UART_Info->sendbuf[j+i*8];	
//											addcheck += sumcheck;					
//								}	
//								UART_Info->sendbuf[6+i*8] = sumcheck;	//校验		
//								UART_Info->sendbuf[7+i*8] = addcheck;	//校验		
//						}
//						UART_Info->Queue_pop_flag=1;
//						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//取数据，队头自增，存数据，队尾自增
//				}
//////////////////////////////////////////////////////////////////////////////////////
//		}	
//		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//允许更新										
//		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE );//禁止更新		
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
		Main_printf("蓝牙发送\r\n");
		Main_printf("内存当前占用 %d\r\n",mem_perused(SRAMIN));	
//////////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("蓝牙设置检测\r\n");
		TIM3_Init(3000,7200);	//WIFI设置检测
		TIM3_Timing=0;
		HC05_SET_FLAG=0;
		while(HC05_SET_FLAG==0 && TIM3_Timing<10)	//等待3s
				HC05_SET();//检测蓝牙按键	
		if(HC05_SET_FLAG)	//配网
		{
				res=HCO5_AT_Confg();//开始设置		
				if(res)		
					Main_printf("蓝牙设置失败\r\n");							
				else									
					Main_printf("蓝牙设置成功\r\n");			
				Main_printf("系统重启\r\n");			
				TIM_Cmd(TIM3, ENABLE); //灯闪烁
				delay_s(3);
				TIM_Cmd(TIM3, DISABLE); 	
				SoftReset();				
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("开启蓝牙串口及DMA中断\r\n");
		HC05_ON;//开蓝牙		
		HC05_uart_init(460800);//串口初始化 HC05串口	
		DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)UART_Info->sendbuf);
		UART2_DMA_TX_NVIC_Config(1);	//开启串口 DMA中断
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//DMA
		UART2_DMA_Finish=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////
		NImingV7_Sendbuf_Init();	//适配匿名上位机的协议	
///////////////////////////////////////////////////////////////////////////
		LED_1=LED_OFF;	LED_2=LED_OFF;
		TIM3_Init(10000,7200);	//WIFI设置检测
		Main_printf("发送数据\r\n");
		delay_s(1);
		KEY_TYPE=0;
		LowPower_flag=0;
		TIM3_Timing=0;
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );//允许更新
		ADC_DMA_NVIC_Config(1);//开启DMA ADC中断			
//////////////////////////////////////////////////////////////////////////////////////////////////////
		while(1)
		{	
				Key_Scan();//按键检测
				if(KEY_TYPE)
				{
						KEY_TYPE=0;
						break;
				}
////////////////////////////////////////////////////////////////////////////////////
				if(TIM3_Timing ==10)	//电量监测
				{
						TIM3_Timing=0;
						BAT_VOL = (u16)((float)ADCConvertedValue[1] / ADCConvertedValue[2]*1200)*2;
						Main_printf("BAT_VOL %d mV ",BAT_VOL);			
						if(LowPower_flag==0)
						{
								if(BAT_VOL<3200)//低电量报警
								{
										LowPower_flag=1;
									#if LOWPOWER ==1//低功耗
										TIM3_Init(2000,1600);
									#else
										TIM3_Init(2000,7200);
									#endif
								}
						}
						else if(LowPower_flag==1)
						{
								if(BAT_VOL<2900)//低电量待机
								{
										LowPower_flag=2;
									#if LOWPOWER ==1//低功耗
										TIM3_Init(1000,1600);
									#else
										TIM3_Init(1000,7200);
									#endif
								}
						}
//						if(LowPower_flag==2)
//						{
//								Main_printf("低电量自动关机\r\n");
//								break;
//						}
				}
////////////////////////////////////////////////////////////////////////////////////				
				if(USART2_RX_EVENT)//串口数据接收
				{
						if(UART2_DMA_Finish==0) //串口未被占用
						{
								USART2_RX_EVENT=0;

								Main_printf("串口数据接收:");
								for(i=0;i<USART2_RX_LEN;i++)
								{
										Main_printf("%x ",USART2_RX_BUF[i]);
								}
								Main_printf("\r\n");
						////////////////////////////////////////////////////////////////////////////////////	
								USART2_rec_EVENT();
						////////////////////////////////////////////////////////////////////////////////////		
								USART2_Clear();
								USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//串口2接收中断 
						}
				}
////////////////////////////////////////////////////////////////////////////////////
				if(UART_Info->Queue_pop_flag==1)	//队列数据取出标志
				{
						if(UART2_DMA_Finish==0) //DMA空闲
						{
								UART2_DMA_Finish=1;
								UART_Info->Queue_pop_flag=0;
								DMA_Enable(DMA1_Channel7,UART_SEND_LENGTH);//发送数据
						}
						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
						{
								UART_Info->UART_Queue->Queue_Full_flag=0;
								Main_printf("f ");
						}
				}
				else	if(serch_queue_data(UART_Info->UART_Queue))//队列有数据
				{
						for(i=0;i<20;i++)//一帧100次采样的数据
						{
								//帧1数据长度为8字节		4（帧头）+2（有效数据）+2（校验）=8	
								UART_Info->sendbuf[0 +i*8]=0xAA;
								UART_Info->sendbuf[1 +i*8]=0xFF;
								UART_Info->sendbuf[2 +i*8]=0xF1;	
								UART_Info->sendbuf[3 +i*8]=2; //有效数据长度 8*4=32 8个导联
								//MIC
								UART_Info->sendbuf[4+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+1+i*2); //低位在前
								UART_Info->sendbuf[5+i*8]=*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+0+i*2);
								//校验位
								sumcheck = 0;addcheck=0;
								for(j=0;j< 6 ;j++)
								{
											sumcheck += UART_Info->sendbuf[j+i*8];	
											addcheck += sumcheck;					
								}	
								UART_Info->sendbuf[6+i*8] = sumcheck;	//校验		
								UART_Info->sendbuf[7+i*8] = addcheck;	//校验		
						}
						UART_Info->Queue_pop_flag=1;
						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//取数据，队头自增，存数据，队尾自增
				}
////////////////////////////////////////////////////////////////////////////////////
		}	
		TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//允许更新										
		TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE );//禁止更新		
		queue_clear(UART_Info->UART_Queue);
		UART2_DMA_TX_NVIC_Config(0);
		UART2_DMA_Finish=0;
}





////数据接收处理
void USART2_rec_EVENT(void) 
{
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+2000")==0)//查询字符串
		{
				Main_printf("修改采样率2000\r\n");	
				TIM4_Init(RATE_2000,720);		//2K采样 50
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新	
		}
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+1000")==0)//查询字符串
		{
				Main_printf("修改采样率1000\r\n");
				TIM4_Init(RATE_1000,720);		//1K采样 100
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新	
		}
		if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+0500")==0)//查询字符串
		{
				Main_printf("修改采样率500\r\n");	
				TIM4_Init(RATE_500,720);		//2K采样 50
				TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新	
		}
}


//匿名发送数据格式	
//32次采样1帧
u8 NImingV7_Sendbuf_Init(void)
{
		u16 i;
		
		u8 datalength = 8;//一次采样要发送的数据长度

		memset(UART_Info->sendbuf,0,UART_SEND_LENGTH);//清零
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//匿名上位机协议
		for(i=0;i<100;i++) //一包N次采样 32 *8 =256字节
		{		
					//帧头
					UART_Info->sendbuf[0 + datalength*i] = 0xAA;
					UART_Info->sendbuf[1 + datalength*i] = 0xFF;
					UART_Info->sendbuf[2 + datalength*i] = 0xF1;
					UART_Info->sendbuf[3 + datalength*i] = 2;//有效数据长度
		}	
		return  0;		
}



//修改HCLK时钟为8M
void SystemInit8 (void)
{
	  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	
		RCC->CR |= (uint32_t)0x00000001;		//打开HSI(内部高速时钟8M)
		RCC->CFGR &= (uint32_t)0xF0FF0000;	//RCC_CFGR寄存器初始化									
		RCC->CR &= (uint32_t)0xFEF6FFFF;		//将RCC_CR寄存器HSEON,CSSON,PLLON位置0
		RCC->CR &= (uint32_t)0xFFFBFFFF;		//将RCC_CR寄存器HSEBYP位置0
		RCC->CFGR &= (uint32_t)0xFF80FFFF;	//将RCC_CFGR寄存器PLLSRC, PLLXTPRE, 	
		RCC->CIR = 0x009F0000;		//关闭所有的中断和对应的位(初始化中断) 
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);	//使能HSE(外部高速时钟)
		do//等待 HSE 启动稳定，并做超时处理
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		//判断HSE是否就绪  
		if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;
		else			HSEStatus = (uint32_t)0x00;  	
		if (HSEStatus == (uint32_t)0x01)// HSE 启动成功，则继续往下处理
		{
				// SYSCLK 周期与闪存访问时间的比例设置			
				FLASH->ACR |= FLASH_ACR_PRFTBE;//使能 FLASH 预存取缓冲区
				FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);//Flash 0等待状态		
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;	// 0： 0 < SYSCLK <= 24M// 1： 24< SYSCLK <= 48M			// 2： 48< SYSCLK <= 72M 				    

							
				RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;    /* HCLK = SYSCLK */      
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;    /* PCLK2 = HCLK */ 
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;    /* PCLK1 = HCLK */
				
				//选择HSE作为系统时钟源
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
				RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    
				//等待HSE作为系统时钟源
				while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
				{
				}		
		}
		else		
			NVIC_SystemReset(); // 复位
}


void SoftReset(void)//软件复位
{
		//__disable_irq()只是禁止CPU去响应中断，没有真正的去屏蔽中断的触发
		//__enable_irq()开启中断后，由于相应的中断标志没有清空，因而还会触发中断
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}
/**********************************************************************
编译结果里面的几个数据的意义：
Code：表示程序所占用 FLASH 的大小（FLASH）
RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)
***********************************************************************/
