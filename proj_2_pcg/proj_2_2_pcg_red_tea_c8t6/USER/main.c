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
#include "oled.h"

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

void SoftReset(void);//软件复位
void Send_BlueTooth(void);
void Send_WIFI(void);
void USART2_REC_EVENT(void);
u8 NImingV7_Sendbuf_Init(void);
//////////////////////////////////////////////////////////////////////

// 心音采集主程序
// 2023.1.30
int main(void)
{	
    g_WorkState = SEND_BULE;
	//系统初始化 72M
    SystemInit();	
    delay_init();	
    delay_ms(100);
    mem_init(SRAMIN);	//内存管理	
    LED_IO_Init();		
    KEY_IO_Init();
    HCO5_GPIO_Init();
    UART1_Init(38400);//串口初始化,原版460800
    OLED_Init();
    // debug
	OLED_ShowString(1, 1, "PCG Demo:");	// 在1行1列显示
	OLED_ShowString(2, 1, "AD0:");
    OLED_ShowString(3, 1, "debug setp:");
    Main_printf("开机\r\n");
    Main_printf("STM32F103C8T6单通道心音采集V0.1 2023-1-30\r\n");	

    LED1 = LED_ON;	
    LED2 = LED_ON;
    delay_ms(1000);		
    LED1 = LED_OFF;
    LED2 = LED_OFF;
		
    TIM1_Init(30000,7200);		//按键检测（预留功能）	
    TIM2_Init(10,7200);			//串口数据解析(修改采样率)
    TIM3_Init(10000,7200);		//系统指示灯	
    TIM4_Init(RATE_2000,720);	//采样率设置

	//ADC初始化
    Main_printf("DMA ADC 初始化\r\n");
    ADC1_Init();
    DMA_ADC_Config(g_ADCConvertedValue, 3);
    Main_printf("DMA ADC 初始化完成\r\n");
    //debug
    OLED_ShowString(3, 12, "2");

	// 串口数据申请内存
    g_UART_Info = (UartInfoStru*)mymalloc(SRAMIN, sizeof(UartInfoStru)); //队列结构
    g_UART_Info->UART_Queue =  QUEUE_Init(UART_QUEUE_SIZE, UART_QUEUE_LENGTH) ;//循环队列初始化
    g_UART_Info->sendbuf = (u8*)mymalloc(SRAMIN, UART_SEND_LENGTH);	//发送缓冲区 
    if(g_UART_Info == NULL || g_UART_Info->UART_Queue == NULL || g_UART_Info->sendbuf == NULL ) {
        Main_printf("串口缓存,内存申请失败\r\n");
    } else {
        Main_printf("串口缓存,内存申请成功\r\n");
    }
    Main_printf("内存当前占用 %d\r\n", mem_perused(SRAMIN));
    //debug
    OLED_ShowString(3, 12, "3");    

	// 启动循环任务
    while(1) {				
        Send_BlueTooth();
    }
}	
	
// 蓝牙发送串口数据
void Send_BlueTooth(void)
{
    u16 i,j,res;
    u8 addcheck=0;
    u8 sumcheck=0;	
	
    Main_printf("蓝牙发送\r\n");
    Main_printf("内存当前占用 %d\r\n",mem_perused(SRAMIN));	
    Main_printf("蓝牙设置检测\r\n");
    TIM3_Init(3000,7200);	//WIFI设置检测
    g_TIM3_Timing = 0;
    g_Hc05SetFlag = 0;
    //debug
    OLED_ShowString(3, 12, "4");    
    while(g_Hc05SetFlag==0 && g_TIM3_Timing<10)	//等待3s
        HC05_SET();//检测蓝牙按键	
    //debug
    OLED_ShowString(3, 12, "5");    
    if (g_Hc05SetFlag) {	// 如果用户进行配网
        res=HCO5_AT_Confg();// 开始设置		
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
    //debug
    OLED_ShowString(3, 12, "6");
    
    Main_printf("开启蓝牙串口及DMA中断\r\n");
    HC05_ON;//开蓝牙		
    HC05_uart_init(38400);//串口初始化 HC05串口,原版460800
    // UART-DMA配置
    DMA_UART_Config(DMA1_Channel7, (u32)&USART2->DR, (u32)g_UART_Info->sendbuf);
    DMA_UART2_TX_NVIC_Config(ENABLE);	//开启串口 DMA中断
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//DMA
    g_Uart2DmaFinishFlag = 0;
    NImingV7_Sendbuf_Init();	//适配匿名上位机的协议	
    
    LED_1 = LED_OFF;	
    LED_2 = LED_OFF;
    TIM3_Init(10000,7200);	//WIFI设置检测
    Main_printf("发送数据\r\n");
    delay_s(1);
    g_KeyType = 0;
    g_LowPowerFlag = 0;
    g_TIM3_Timing = 0;
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//允许更新
    DMA_ADC_NVIC_Config(ENABLE);//开启DMA ADC中断		
    //debug
    OLED_ShowString(3, 12, "7");    

    while(1) {	
        Key_Scan();//按键检测
        if(g_KeyType) {
            g_KeyType = 0;
            break;
        }

		//debug
		OLED_ShowString(3, 12, "8");

		//debug
		OLED_ShowNum(2, 5, g_ADCConvertedValue[0], 4);
	    delay_ms(100);		

        // 电池电量监测
        if(g_TIM3_Timing == 10) {	
            g_TIM3_Timing = 0;
            g_BatVol = (u16)((float)g_ADCConvertedValue[1] / g_ADCConvertedValue[2]*1200)*2;
            Main_printf("Bat Voltage %d mV\r\n",g_BatVol);			
            if (g_LowPowerFlag==0){
                if (g_BatVol<3200) {//低电量报警
                    g_LowPowerFlag= 1;
                    #if LOWPOWER == 1//低功耗
                    TIM3_Init(2000,1600);
                    #else
                    TIM3_Init(2000,7200);
                    #endif
                }
            } else if(g_LowPowerFlag==1) {
                if (g_BatVol<2900) {//低电量待机
                        g_LowPowerFlag=2;
                    #if LOWPOWER == 1//低功耗
                        TIM3_Init(1000,1600);
                    #else
                        TIM3_Init(1000,7200);
                    #endif
                }
            }
        }

		//debug
		OLED_ShowString(3, 12, "9");

        // 串口数据接收(修改ADC采样率)
        if (USART2_RX_EVENT) { 				
            if (g_Uart2DmaFinishFlag == 0) { //串口未被占用						
                USART2_RX_EVENT=0;

                Main_printf("串口数据接收:");
                for(i=0; i < USART2_RX_LEN; i++) {                
                    Main_printf("%x ", g_Usart2RxBuf[i]);
                }
                Main_printf("\r\n");
                USART2_REC_EVENT();
                USART2_Clear();
                USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//串口2接收中断 
            }
        }

		//debug
		OLED_ShowString(3, 12, "A");

        // 队列数据取出标志
        if (g_UART_Info->Queue_pop_flag == 1) {	
            if (g_Uart2DmaFinishFlag == 0) { 					//DMA空闲
                g_Uart2DmaFinishFlag = 1;
                g_UART_Info->Queue_pop_flag = 0;
                DMA_Enable(DMA1_Channel7, UART_SEND_LENGTH);	//发送数据
            } else if (g_UART_Info->UART_Queue->Queue_Full_flag == 1) {
                g_UART_Info->UART_Queue->Queue_Full_flag = 0;
                Main_printf("串口数据发送:\r\n");
            }
        } else if (QUEUE_SearchData(g_UART_Info->UART_Queue)) {	//若队列有数据
        	// 一帧20次采样的数据,填充发送缓冲区
            for (i = 0; i < 20; i++) {
                //帧1数据长度为8字节		4（帧头）+2（有效数据）+2（校验）=8	
                // 1.填充四字节帧头
                g_UART_Info->sendbuf[0 + i*8] = 0xAA;
                g_UART_Info->sendbuf[1 + i*8] = 0xFF;
                g_UART_Info->sendbuf[2 + i*8] = 0xF1;	
                g_UART_Info->sendbuf[3 + i*8] = 2; //有效数据长度 8*4=32 8个导联?注释待更新?2023.1.30
                // 2.填充两字节MIC有效数据
                // 搬移：队列缓冲区数据->发送缓冲区数据,每次搬2个U8
                g_UART_Info->sendbuf[4 + i*8] = *(*(g_UART_Info->UART_Queue->databuf \
                	+ g_UART_Info->UART_Queue->front)+1+i*2); //低位在前
                g_UART_Info->sendbuf[5 + i*8]=*(*(g_UART_Info->UART_Queue->databuf \
					+ g_UART_Info->UART_Queue->front)+0+i*2);
                // 3.填充两字节校验位
                sumcheck = 0;
                addcheck = 0;
                for (j = 0; j < 6 ;j++) {	// 对前6位进行校验
                    sumcheck += g_UART_Info->sendbuf[j + i*8];	
                    addcheck += sumcheck;					
                }
                g_UART_Info->sendbuf[6 + i*8] = sumcheck;	//校验		
                g_UART_Info->sendbuf[7 + i*8] = addcheck;	//校验		
            }
            g_UART_Info->Queue_pop_flag = 1;
            //取数据，队头自增，存数据，队尾自增
            g_UART_Info->UART_Queue->front = (g_UART_Info->UART_Queue->front+1) % g_UART_Info->UART_Queue->capacity;
            Main_printf("串口数据一帧充填buf完毕:\r\n");
        }
    }	
    //debug
    OLED_ShowString(3, 12, "B");	
	
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//允许更新
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);	//禁止更新
    QUEUE_Clear(g_UART_Info->UART_Queue);
    DMA_UART2_TX_NVIC_Config(DISABLE);
    g_Uart2DmaFinishFlag = 0;
}

////数据接收处理，用于修改ADC采样率
void USART2_REC_EVENT(void) 
{
	//查询字符串
    if(Str_search(g_Usart2RxBuf,USART2_RX_LEN,"AT+2000")==0) {
        Main_printf("修改采样率2000\r\n");	
        TIM4_Init(RATE_2000,720);		//2K采样 50
        TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新	
    }
	//查询字符串
    if(Str_search(g_Usart2RxBuf,USART2_RX_LEN,"AT+1000")==0) {    
        Main_printf("修改采样率1000\r\n");
        TIM4_Init(RATE_1000,720);		//1K采样 100
        TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新	
    }
	//查询字符串
    if(Str_search(g_Usart2RxBuf,USART2_RX_LEN,"AT+0500")==0) {    
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
    memset(g_UART_Info->sendbuf,0,UART_SEND_LENGTH);//清零

    //匿名上位机协议
    //一包N次采样 32 *8 =256字节
    for (i=0;i<100;i++) {		
        //帧头
        g_UART_Info->sendbuf[0 + datalength*i] = 0xAA;
        g_UART_Info->sendbuf[1 + datalength*i] = 0xFF;
        g_UART_Info->sendbuf[2 + datalength*i] = 0xF1;
        g_UART_Info->sendbuf[3 + datalength*i] = 2;//有效数据长度
    }	
    return  0;		
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

