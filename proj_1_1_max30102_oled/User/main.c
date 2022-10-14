#include "algorithm.h"
#include "blood.h"
#include "delay.h"
#include "exti.h"
#include "Font.h"
#include "gui.h"
#include "iic.h"
#include "lcd.h"
#include "MAX30102.h"
#include "OLED_IIC.h"
#include "stm32f10x.h"
#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "18b20.h"

// ��������
void InitAllDriver(void);

// @kmakise
// https://www.bilibili.com/video/BV1AJ411Y7yY
// https://github.com/kmakise/Oximeter
int main(void)
{	
	int i = 0;

	InitAllDriver();
	
	OLED_Printf_EN(0, 0, "   *Oximeter*");
	Gui_StrCenter(0,91,BLUE,BLUE,"2022-09-30",16,1);
	// ΪʲôҪִ��128��? ִ����TFT����������
//	for (i = 0; i < 128; i++) {
//		while (MAX30102_INTPin_Read() == 0) {
//			// ��ȡFIFO
//			MAX30102_Read_Fifo();
//		}
//	}
//	
	while(1) {
		//GUI_MainTest();
		BLOOD_Loop();		
		OLED_Printf_EN(2, 0, "heart:%3d/min  ", 60);
	}
}

/************************************************
Func: main�������е�ģ���ʼ��
Date: 2022.9.30
Author: h00421956
History:
************************************************/
void InitAllDriver(void) {
	// ģ���ʼ��-BNG
	
	// ϵͳ��ʼ��
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();	     //��ʱ��ʼ��	
	
	// ����init
	USART_CONFIG();
	printf("���Ǵ��ڵ�������\r\n");	
	// oled init
	IIC_GPIO_INIT();	
	OLED_Init();
	OLED_Fill_Picture(0x00);

	//Ds18b20_Init();
	//DS18B20_Read_Temperature();
	
	// Ѫ��ģ��init
	MAX30102_GPIO();	
	MAX30102_Reset();	
	MAX30102_Config();
	
	// tft-lcd��ʼ��
	LCD_Init();	   //Һ������ʼ��			
	
	// ģ���ʼ��-END

}

