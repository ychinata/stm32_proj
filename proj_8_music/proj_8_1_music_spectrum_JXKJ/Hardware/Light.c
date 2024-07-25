#include "stm32f10x.h"
#include "ADC2.h"

uint16_t Light_ValueBuf[50];
uint8_t Light_pValueBuf;
uint16_t Light_Value;

void Light_Loop(void);

void Light_Init(void)
{
	ADC2_Init();
	for(uint8_t i=0;i<50;i++)
	{
		Light_Loop();
	}
}	

uint16_t Light_GetValue(void)
{
	return Light_Value;
}

void Light_Loop(void)
{
	uint8_t i;
	uint32_t Sum=0;
	Light_ValueBuf[Light_pValueBuf]=ADC2_GetValue();
	Light_pValueBuf++;
	Light_pValueBuf%=50;
	for(i=0;i<50;i++)
	{
		Sum+=Light_ValueBuf[i];
	}
	Light_Value=Sum/50;
}
