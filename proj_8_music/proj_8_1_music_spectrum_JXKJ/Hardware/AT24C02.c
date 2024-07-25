#include "stm32f10x.h"
#include "MyI2C.h"
#include "Display.h"
#include "Delay.h"

#define AT24C02_ADDRESS		0xA0

void AT24C02_Init(void)
{
	MyI2C_Init();
}

void AT24C02_WriteByte(uint8_t WordAddress,uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	Delay_ms(5);
}

uint8_t AT24C02_ReadByte(uint8_t WordAddress)
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress);
	MyI2C_ReceiveAck();
	MyI2C_Start();
	MyI2C_SendByte(AT24C02_ADDRESS|0x01);
	MyI2C_ReceiveAck();
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	MyI2C_Stop();
	return Data;
}
