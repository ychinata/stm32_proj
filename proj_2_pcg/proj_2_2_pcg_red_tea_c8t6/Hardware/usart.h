#ifndef __USART_H
#define __USART_H
#include "sys.h" 
#include "myQueue.h"



//////////////////////////////////////////////////////////////////////////
#define UART1_DEBUG 1 //串口使能
#define USART_REC_LEN  		255 //定义最大接收字节数

#define UART_REC_LENGTH		256	//接收缓冲区
//////////////////////////////////////////////////////////////////////////
#if UART1_DEBUG == 1 
	#define Main_printf(...)  printf(__VA_ARGS__) //串口
#else
	#define Main_printf(...) 	//串口
#endif
//////////////////////////////////////////////////////////////////////////
////wifi
//1帧100个采样?
//#define UART_QUEUE_LENGTH	200	//（队列容量）100次采样 2*100=200  
//#define UART_QUEUE_SIZE	30	//队列大小
//#define UART_SEND_LENGTH	800	//发送缓冲区  8*100
//蓝牙
//1帧20个采样?
#define UART_QUEUE_LENGTH	40	//（队列容量）20次采样 2*20=40  
#define UART_QUEUE_SIZE		100	//队列大小
#define UART_SEND_LENGTH	160	//发送缓冲区  8*20

//__align(4) typedef struct {
typedef struct {
    u16 datalength; 
    u8 *sendbuf;			//发送缓冲区
    //u8 *recbuf;			//接收缓冲区
    u32 datanumber; 
    u8 Queue_pop_flag;		//队列数据取出标志		1BYTE,1为可取出,0为不可取出
    QueueInfo *UART_Queue;	//队列缓冲区指针
} UartInfoStru;

extern UartInfoStru *g_UART_Info ; //上传数据结构
//////////////////////////////////////////////////////////////////////////		
extern u8 USART_RX_BUF[USART_REC_LEN] ;   //接收缓冲                         
extern u16 USART_RX_STA;         		//接收状态标记	

//extern u8 USART1_RX_BUF[USART_REC_LEN] ;   //接收缓冲                         
//extern u16 USART1_RX_LEN;
//extern u16 USART1_TIME;

extern u8 g_Usart2RxBuf[USART_REC_LEN] ;   //接收缓冲   
extern u16 USART2_RX_LEN;         		//接收状态标记	
extern u16 USART2_TIME;
extern u16 USART2_RX_EVENT;

//extern u8 USART3_RX_BUF[USART_REC_LEN] ;   //接收缓冲   
//extern u16 USART3_RX_LEN;         		//接收状态标记	
//extern u16 USART3_TIME;


void UART1_Init(u32 bound);
void UART2_Init(u32 bound);
//void uart3_init(u32 bound);


//void u2_printf(char* fmt,...); 
//void u3_printf(char* fmt,...);  
//void u4_printf(char* fmt,...);  


void USART1_Clear(void);
void USART2_Clear(void);
//void USART3_Clear(void);


void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len);  

void UART1_Send_STR(u8* cmd);
void UART2_Send_STR(u8* cmd);
void UART3_Send_STR(u8* cmd);

s16 Str_search(u8* buf,u8 len,u8* result);//判断指定字符串是否在数组中
s16 Char_search(u8* buf,u8 len,u8 result);


#endif

