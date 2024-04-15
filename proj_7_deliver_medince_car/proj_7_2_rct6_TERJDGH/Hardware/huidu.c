#include "sys.h"


void huidu_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t get_num_huidu(void)
{
	uint8_t huidu_num = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
	{
		huidu_num = 4;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		huidu_num = 5;
	}
	return huidu_num;
}
