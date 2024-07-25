#include "stm32f10x.h"
#include "Flash.h"
#include "Display.h"
/*
uint16_t Store_Data[16]=
{
	0x1234,		//测试数据
	0,			//Clock_ColorSelect
	0,			//Brightness
	0
};

void Store_Init(void)
{
	uint16_t TestData;
	TestData=Flash_Read(0x0800FC00);
	if(TestData!=0x1234)
	{
		Flash_ErasePage(0x0800FC00);
		Flash_WritePage(0x0800FC00,Store_Data,16);
	}
	else
	{
		Flash_ReadPage(0x0800FC00,Store_Data,16);
	}
}

void Store_Update(void)
{
	Display_Enable(0);
	Flash_ErasePage(0x0800FC00);
	Flash_WritePage(0x0800FC00,Store_Data,16);
	Display_Enable(1);
}
*/
