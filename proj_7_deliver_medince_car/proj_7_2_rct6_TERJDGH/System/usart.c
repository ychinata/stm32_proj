#include "sys.h"

unsigned char k210_num = 0;
int16_t Serial_RxNUM;
int16_t k210_Rx; 

void USART2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//设置Tx引脚为复用推挽
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//设置Rx引脚为浮空上拉
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=115200;//设置波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//设置RX和TX
	USART_InitStruct.USART_Parity=USART_Parity_No;//设置无校验位
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8位数据长度
	
	USART_Init(USART2,&USART_InitStruct);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
	
	USART_Cmd(USART2,ENABLE);
	
}


/**
  * @brief  获取k210的数字识别数据
  * @param  无
  * @retval 无
  */
void Get_k210_NUM(uint8_t num)
{
	static uint8_t first = 1;
	if(first == 1)
	{
		k210_num = num;
		first = 0;
	}
	else 
		return;
	
	return;
}


/**
  * @brief  串口中断
  * @param  无
  * @retval 无
  */
void USART2_IRQHandler(void)
{
	static uint8_t RxState = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		
		if (RxState == 0)
		{
			if (RxData == 0x3A)
			{
				RxState = 1;
			}
		}
		else if (RxState == 1)
		{
			Serial_RxNUM = RxData;
				RxState = 2;
		}
		else if (RxState == 2)
		{
			if (RxData == 0x5C)
			{
				if(Serial_RxNUM > 0 && Serial_RxNUM < 9)
				{
					Get_k210_NUM(Serial_RxNUM);
					k210_Rx = Serial_RxNUM;
					RxState = 0;
				}
				
			}
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

