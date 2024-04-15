#include "lightsensor.h"

/**
  * @brief  ���⴫������ʼ��
  * @param  ��
  * @retval ��
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
  * @brief  ��������ʼ��
  * @param  ��
  * @retval ��
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
  * @brief  ��������
  * @param  ��
  * @retval ��
  */
void Beep_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}
/**
  * @brief  �������ر�
  * @param  ��
  * @retval ��
  */
void Beep_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
/**
  * @brief  �ȴ�ҩƷװ��
  * @param  ��
  * @retval 0�ȴ���1װҩ��2ȡҩ
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

