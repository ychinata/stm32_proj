#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "serial.h"  

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //开启USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //配置为复用推挽输出 （TX）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //配置为上拉输入  （Rx）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;             //配置波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //配置流控 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;         //配置发送（TX）或接收模式 （Rx）
	USART_InitStructure.USART_Parity = USART_Parity_No;      //配置校验位 No无校验 Odd奇校验 Even偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //选择停止位  0.5 1 1.5
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //选择字长 8位或9位
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启中断 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         //分组2
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//开启
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 		//优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);          //开启USART
}

void Serial_SendByte(uint8_t Byte) 
{
	USART_SendData(USART1, Byte); //传输一个字节数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //等待TXE置1
}


//发送一个数组 1；数组名 2；数组长度
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

//发送一个字符串 1；输入要写入的字符串
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}
//发送一组数字 1；数字 2；数字长度
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

//打印函数 
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) //读取USART_IT_RXNE是否被置1
	{
		Serial_RxData = USART_ReceiveData(USART1);      //读取模块
		Serial_RxFlag = 1;															//读取完成置标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);  //清除标志位
	}
}

void Send_data(USART_TypeDef * USARTx,uint8_t *s)
{
 while(*s!='\0')
 { 
  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC )==RESET); 
  USART_SendData(USARTx,*s);
  s++;
 }

}
