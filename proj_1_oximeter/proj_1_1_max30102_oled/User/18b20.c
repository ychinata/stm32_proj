#include "18b20.h"
#include "usart.h"
#include "OLED_IIC.h"

/************************************************
Func:
Date: 2022.
Author: h00421956
History: 
1.20220908.rename Ds18b20_GPIO_OUT->DS18B20_GPIO_OutCfg
************************************************/
void DS18B20_GPIO_OutCfg(void)
{	
	RCC_APB2PeriphClockCmd(Ds18b20_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin		=	Ds18b20_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(Ds18b20_PORT,&GPIO_InitStruct);
}

/************************************************
Func:
Date: 2022.
Author: h00421956
History: 
1.20220908.rename Ds18b20_GPIO_IN->DS18B20_GPIO_InCfg
************************************************/
void DS18B20_GPIO_InCfg(void)
{	
	RCC_APB2PeriphClockCmd(Ds18b20_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin		=	Ds18b20_Pin;
	GPIO_Init(Ds18b20_PORT,&GPIO_InitStruct);
}

uint8_t Ds18b20_Init(void)
{
	uint8_t ack=1;
	DS18B20_GPIO_OutCfg();
	DQ_0();
	Delay_us(500);
	DQ_1();
	Delay_us(60);
	DS18B20_GPIO_InCfg();
	ack=DQ_Read();
	Delay_us(180);
	DQ_1();
	return ack;
}

void Ds18b20_Write_Byte(uint8_t byte)
{
	uint8_t i;
	DS18B20_GPIO_OutCfg();
	for(i=0 ; i<8 ; i++) {
		if (byte&0x01) {
				DQ_0();// Write 1
				Delay_us(2);                            
				DQ_1();
				Delay_us(60);             
		} else {
				DQ_0();// Write 0
				Delay_us(60);             
				DQ_1();
				Delay_us(2);                          
		}
		byte=byte>>1;
	}
}

uint8_t DS18B20_Read_Write()
{
	uint8_t i;
	uint8_t byte;        		 //byteΪҪ���յ�������
	for(i=0 ; i<8 ; i++) {
		DS18B20_GPIO_OutCfg();//SET PG11 OUTPUT
		DQ_0(); 
		Delay_us(2);
		DQ_1();
		DS18B20_GPIO_InCfg();//SET PG11 INPUT
		Delay_us(12);
		byte >>= 1;    
		if(DQ_Read())
			byte |= 0x80;
		Delay_us(50);           
					 
	}
	return byte;
}

/************************ ��ȡ�¶� ************************/
int16_t DS18B20_Read_Temperature(void)  	//�����¶�ת������ȡ�¶�
{
	u8 tl=0,th=0;
	if(Ds18b20_Init())
		return 0x7fff;
	Ds18b20_Write_Byte(0xCC);
	Ds18b20_Write_Byte(0x44);//����DS18B20�����¶�ת��

	if(Ds18b20_Init())
		return 0x7fff;	
	Ds18b20_Write_Byte(0xCC);//����64λROM��ַ��ֱ����DS18B20���¶�ת�����������һ���ӻ�����
	Ds18b20_Write_Byte(0xBE);//��DS18B20�ڲ�RAM��9�ֽڵ��¶�����
	tl=DS18B20_Read_Write();//����8λ
	th=DS18B20_Read_Write();//����8λ
	return (th<<8)+tl;
}

/************************ �����¶� ************************/
void DS18B20_Deal_Temp(void)
{
	float Temp=0;
	Temp = DS18B20_Read_Temperature();
	Temp = Temp*0.0625; 
	printf("ָ���¶�%0.2f",Temp);
	OLED_Printf_EN(0,0,"Temp:%0.2f`C",Temp);
	Delay_ms(200);
}

