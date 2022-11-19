#include "hc05.h"
#include "delay.h"
#include "usart.h"	 
#include "led.h" 
#include "key.h"


//蓝牙引脚初始化
void HCO5_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		//AT 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		//EN 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		HC05_OFF;//关蓝牙
		HC05_AT=0;	//取消AT指令
}




u8 HC05_SET_FLAG=0;

//蓝牙设置检测
void HC05_SET(void)
{
		u16 ret=0;
		if (KEY1 == 0)//按键按下
		{
				LED_2=LED_ON;
				while( KEY1 == 0)
				{
						delay_ms(10);
						ret++;
						if ( ret > 300 )//3s
						{
								LED_2=LED_OFF;
								HC05_SET_FLAG=1;//设置蓝牙
								while( KEY1 == 0);
										
								return ;//退出函数
						}
				}
				LED_2=LED_OFF;
		}
}



//发送指令，并判断指定字符串是否缓冲区中
//返回	-1 			没有指定的串
//			0-125		字符串在数组中的位置
s16 HC05_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	  HC05_CLEAR();//清空缓冲区
    USARTx_Send(HC05_UART,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		HC05_DEBUG("HC05 rsp:%s \r\n",HC05_RX_BUF);		
		res = Str_search(HC05_RX_BUF,HC05_RX_LEN,result);//查询字符串
		return res;
}



u8 HCO5_AT_Confg(void)
{		
		u8 res=1;
	
		//设置AT模式		
		HC05_AT=1;	//拉高AT引脚
		delay_s(1);
		HC05_ON;//开蓝牙
		delay_s(1);
		HC05_AT=0;
		delay_s(1);	
	
		HC05_uart_init(38400);
		USART_ITConfig(HC05_UART, USART_IT_RXNE, ENABLE);//开启串口接受中断		

		HC05_DEBUG("AT测试 ");	
		if(HC05_SendCmd(HC05_AT_TEST, "OK", 500) < 0)//通讯测试
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");

		HC05_DEBUG("设置蓝牙名 ");
		if(HC05_SendCmd(HC05_AT_NAME, "OK", 500) < 0)//名称
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");

		HC05_DEBUG("主从设置 ");
		if(HC05_SendCmd(HC05_AT_RLOE, "OK", 500) < 0)//主从模式
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");
		
		HC05_DEBUG("清除记录的地址 ");
		if(HC05_SendCmd(HC05_AT_RMAAD, "OK", 500) < 0)//清除从机地址
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");
		
		HC05_DEBUG("设置波特率 ");
		if(HC05_SendCmd(HC05_AT_UART, "OK", 500) < 0)	//设置波特率
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");

		HC05_DEBUG("设置连接方式 ");
		if(HC05_SendCmd(HC05_AT_CMODE, "OK", 500) < 0)//连接方式
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");

		HC05_DEBUG("设置密码 ");
		if(HC05_SendCmd(HC05_AT_PSWD, "OK", 500) < 0)	//设置密码
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("成功\r\n");
	
		HC05_DEBUG("复位 ");
		if(HC05_SendCmd(HC05_AT_RESET, "OK", 500) < 0)	//复位			
				goto end;
		
		res=0;//设置成功
end:
		USART_ITConfig(HC05_UART, USART_IT_RXNE, DISABLE);//关闭串口接受中断	
		LED_2=LED_OFF;
		HC05_OFF;//关蓝牙电源
		
		if(res == 0)
				HC05_DEBUG("蓝牙设置成功\r\n");
		else
				HC05_DEBUG("蓝牙设置失败\r\n");
		HC05_OFF;//关蓝牙电源
		
		return res;
}




