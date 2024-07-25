#include "stm32f10x.h"
#include "Delay.h"

uint8_t Key_Number;
uint8_t Key_State;

uint8_t Key_Count;

uint8_t Key1_State[2]={1,1};
uint8_t Key2_State[2]={1,1};

#define Key1_Read()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define Key2_Read()	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_3|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetKeyNum(void)
{
	uint8_t KeyNum=0;
	if(Key1_Read()==0)
	{
		Delay_ms(20);
		while(Key1_Read()==0);
		Delay_ms(20);
		KeyNum=1;
	}
	if(Key2_Read()==0)
	{
		Delay_ms(20);
		while(Key2_Read()==0);
		Delay_ms(20);
		KeyNum=2;
	}
	return KeyNum;
}

uint8_t Key_GetKeyNumber(void)
{
	uint8_t KeyNumberTemp;
	KeyNumberTemp=Key_Number;
	if(KeyNumberTemp)
	{
		Key_Number=0;
		return KeyNumberTemp;
	}
	return 0;
}

void Key_Loop(void)
{
	Key1_State[1]=Key1_State[0];
	Key1_State[0]=Key1_Read();
	Key2_State[1]=Key2_State[0];
	Key2_State[0]=Key2_Read();
	
	if(Key2_State[0]==1)
	{
		if(Key1_State[1]==0 && Key1_State[0]==0)
		{
			Key_Count++;
			if(Key_Count==25)
			{
				Key_Number=3;
			}
			else if(Key_Count>35)
			{
				Key_Count=35;
				Key_Number=5;
			}
		}
		else if(Key1_State[1]==0 && Key1_State[0]==1)
		{
			if(Key_Count<25)
			{
				Key_Number=1;
			}
			Key_Count=0;
		}
	}
	if(Key1_State[0]==1)
	{
		if(Key2_State[1]==0 && Key2_State[0]==0)
		{
			Key_Count++;
			if(Key_Count==25)
			{
				Key_Number=4;
			}
			else if(Key_Count>35)
			{
				Key_Count=35;
				Key_Number=6;
			}
		}
		else if(Key2_State[1]==0 && Key2_State[0]==1)
		{
			if(Key_Count<25)
			{
				Key_Number=2;
			}
			Key_Count=0;
		}
	}
}
