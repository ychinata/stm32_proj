#include "lightsensor.h"

/**
  * @brief  红外传感器初始化
  * @param  无
  * @retval 无
  */
void light_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruture.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruture);
}
/**
  * @brief  蜂鸣器初始化
  * @param  无
  * @retval 无
  */
void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruture.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruture.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruture);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
/**
  * @brief  蜂鸣器打开
  * @param  无
  * @retval 无
  */
void Beep_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}
/**
  * @brief  蜂鸣器关闭
  * @param  无
  * @retval 无
  */
void Beep_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
/**
  * @brief  等待药品装载
  * @param  无
  * @retval 0等待，1装药，2取药
  */
uint8_t wait_pha_ok(void)
{
	static uint8_t load_flag;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1)
	{
		load_flag = 1;
		return 1;
	}
	else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		if(load_flag == 1)
		{
			return 2;
		}
	}
	return 0;
}

