#ifndef __HC05_H
#define __HC05_H
#include "sys.h"




#define HC05_UART_DEBUG		1 				//���ڵ��Կ���
#define HC05_AT			PAout(4)	//AT��������
#define HC05_EN			PBout(0)	//��Դ��������
#define HC05_ON  		HC05_EN=0
#define HC05_OFF		HC05_EN=1


#define HC05_UART   USART2
#define HC05_uart_init(x)   uart2_init(x)
#define HC05_RX_LEN		USART2_RX_LEN
#define HC05_RX_BUF		USART2_RX_BUF
#define HC05_CLEAR()  USART2_Clear()

////////////////////////////////////////////////////////////////////////////////
//����DEBUG
#if HC05_UART_DEBUG == 1 
		#define HC05_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define HC05_DEBUG(...)		;
#endif
////////////////////////////////////////////////////////////////////////////////
#define HC05_AT_TEST  "AT\r\n"													//AT����
#define HC05_AT_NAME  "AT+NAME=HC05\r\n"					//�����豸����
//#define HC05_AT_RLOE  "AT+ROLE=1\r\n"											//����Ϊ����
#define HC05_AT_RLOE  "AT+ROLE=0\r\n"									//����Ϊ�ӻ� �ӻ����Ա��ֻ��͵���������
#define HC05_AT_RMAAD "AT+RMAAD\r\n"										//���������¼�Ĵӻ���ַ
//#define HC05_AT_PSWD  "AT+PSWD=1234\r\n"							//�������� ԭװоƬ
#define HC05_AT_PSWD  "AT+PSWD=\"1234\"\r\n"						//�������� ����оƬ
#define HC05_AT_UART  "AT+UART=460800,0,0\r\n"				//���ò�����
#define HC05_AT_CMODE "AT+CMODE=1\r\n"									//�������ӷ�ʽ
#define HC05_AT_RESET "AT+RESET\r\n"										//��λ


extern u8 HC05_SET_FLAG;//�������ñ�־


void HCO5_GPIO_Init(void);//�ϵ����ų�ʼ��
u8 HCO5_AT_Confg(void);			//��������
void HC05_SET(void);//�������ü��
u8 Blue_Uart_Send(u8 *p); //�������ú���


#endif


