#ifndef __USART_H
#define __USART_H
#include "sys.h" 
#include "myQueue.h"



//////////////////////////////////////////////////////////////////////////
#define UART1_DEBUG 1 //����ʹ��
#define USART_REC_LEN  		255 //�����������ֽ���

#define UART_REC_LENGTH		256	//���ջ�����
//////////////////////////////////////////////////////////////////////////
#if UART1_DEBUG == 1 
	#define Main_printf(...)  printf(__VA_ARGS__) //����
#else
	#define Main_printf(...) 	//����
#endif
//////////////////////////////////////////////////////////////////////////
////wifi
//#define UART_QUEUE_LENGTH	200	//������������100�β��� 2*100=200  
//#define UART_QUEUE_SIZE	30	//���д�С
//#define UART_SEND_LENGTH	800	//���ͻ�����  8*100
//����
#define UART_QUEUE_LENGTH	40	//������������20�β��� 2*20=40  
#define UART_QUEUE_SIZE		100	//���д�С
#define UART_SEND_LENGTH	160	//���ͻ�����  8*100

__align(4) typedef struct {
    u16 datalength; 
    u8 *sendbuf;			//���ͻ�����
    //u8 *recbuf;			//���ջ�����
    u32 datanumber; 
    u8 Queue_pop_flag;		//��������ȡ����־		1BYTE	
    QueueInfo *UART_Queue;	//���л�����ָ��
}_UART_Info;

extern  _UART_Info *UART_Info ; //�ϴ����ݽṹ
//////////////////////////////////////////////////////////////////////////		
extern u8 USART_RX_BUF[USART_REC_LEN] ;   //���ջ���                         
extern u16 USART_RX_STA;         		//����״̬���	

//extern u8 USART1_RX_BUF[USART_REC_LEN] ;   //���ջ���                         
//extern u16 USART1_RX_LEN;
//extern u16 USART1_TIME;

extern u8 g_Usart2RxBuf[USART_REC_LEN] ;   //���ջ���   
extern u16 USART2_RX_LEN;         		//����״̬���	
extern u16 USART2_TIME;
extern u16 USART2_RX_EVENT;

//extern u8 USART3_RX_BUF[USART_REC_LEN] ;   //���ջ���   
//extern u16 USART3_RX_LEN;         		//����״̬���	
//extern u16 USART3_TIME;


void uart1_init(u32 bound);
void uart2_init(u32 bound);
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

s16 Str_search(u8* buf,u8 len,u8* result);//�ж�ָ���ַ����Ƿ���������
s16 Char_search(u8* buf,u8 len,u8 result);


#endif
