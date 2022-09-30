#include "stm32f10x.h"
#include "exti.h"
#include "systick.h"
#include "usart.h"
#include "iic.h"
#include "Font.h"
#include "OLED_IIC.h"
#include "18b20.h"
#include "MAX30102.h"
#include "algorithm.h"
#include "blood.h"

// @kmakise
// https://www.bilibili.com/video/BV1AJ411Y7yY
// https://github.com/kmakise/Oximeter

int main(void)
{	
	int i = 0;

	// ģ���ʼ��-BNG
	USART_CONFIG();
	printf("���Ǵ��ڵ�������\r\n");	
	IIC_GPIO_INIT();	
	OLED_Init();
	OLED_Fill_Picture(0x00);

	//Ds18b20_Init();
	//DS18B20_Read_Temperature();
	
	MAX30102_GPIO();	
	MAX30102_Reset();	
	MAX30102_Config();
	// ģ���ʼ��-END
	OLED_Printf_EN(0, 0, "   *Oximeter*");
	
	for (i = 0; i < 128; i++) {
		while (MAX30102_INTPin_Read() == 0) {
			// ��ȡFIFO
			MAX30102_Read_Fifo();
		}
	}

	
	while(1) {
		// DS18B20_Deal_Temp();
		BLOOD_Loop();
		//OLED_Printf_EN(2, 0, "heart:%3d/min  ", 60);
	}
}

