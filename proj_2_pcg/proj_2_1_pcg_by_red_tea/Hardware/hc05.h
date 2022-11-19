#ifndef __HC05_H
#define __HC05_H
#include "sys.h"




#define HC05_UART_DEBUG		1 				//串口调试开关
#define HC05_AT			PAout(4)	//AT控制引脚
#define HC05_EN			PBout(0)	//电源控制引脚
#define HC05_ON  		HC05_EN=0
#define HC05_OFF		HC05_EN=1


#define HC05_UART   USART2
#define HC05_uart_init(x)   uart2_init(x)
#define HC05_RX_LEN		USART2_RX_LEN
#define HC05_RX_BUF		USART2_RX_BUF
#define HC05_CLEAR()  USART2_Clear()

////////////////////////////////////////////////////////////////////////////////
//串口DEBUG
#if HC05_UART_DEBUG == 1 
		#define HC05_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define HC05_DEBUG(...)		;
#endif
////////////////////////////////////////////////////////////////////////////////
#define HC05_AT_TEST  "AT\r\n"													//AT测试
#define HC05_AT_NAME  "AT+NAME=HC05\r\n"					//设置设备名称
//#define HC05_AT_RLOE  "AT+ROLE=1\r\n"											//设置为主机
#define HC05_AT_RLOE  "AT+ROLE=0\r\n"									//设置为从机 从机可以被手机和电脑搜索到
#define HC05_AT_RMAAD "AT+RMAAD\r\n"										//清除主机记录的从机地址
//#define HC05_AT_PSWD  "AT+PSWD=1234\r\n"							//蓝牙密码 原装芯片
#define HC05_AT_PSWD  "AT+PSWD=\"1234\"\r\n"						//蓝牙密码 兼容芯片
#define HC05_AT_UART  "AT+UART=460800,0,0\r\n"				//设置波特率
#define HC05_AT_CMODE "AT+CMODE=1\r\n"									//设置连接方式
#define HC05_AT_RESET "AT+RESET\r\n"										//复位


extern u8 HC05_SET_FLAG;//蓝牙设置标志


void HCO5_GPIO_Init(void);//上电引脚初始化
u8 HCO5_AT_Confg(void);			//设置蓝牙
void HC05_SET(void);//蓝牙设置检测
u8 Blue_Uart_Send(u8 *p); //串口设置函数


#endif


