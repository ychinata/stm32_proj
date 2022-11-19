#include "hc05.h"
#include "delay.h"
#include "usart.h"	 
#include "led.h" 
#include "key.h"


//�������ų�ʼ��
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
	
		HC05_OFF;//������
		HC05_AT=0;	//ȡ��ATָ��
}




u8 HC05_SET_FLAG=0;

//�������ü��
void HC05_SET(void)
{
		u16 ret=0;
		if (KEY1 == 0)//��������
		{
				LED_2=LED_ON;
				while( KEY1 == 0)
				{
						delay_ms(10);
						ret++;
						if ( ret > 300 )//3s
						{
								LED_2=LED_OFF;
								HC05_SET_FLAG=1;//��������
								while( KEY1 == 0);
										
								return ;//�˳�����
						}
				}
				LED_2=LED_OFF;
		}
}



//����ָ����ж�ָ���ַ����Ƿ񻺳�����
//����	-1 			û��ָ���Ĵ�
//			0-125		�ַ����������е�λ��
s16 HC05_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	  HC05_CLEAR();//��ջ�����
    USARTx_Send(HC05_UART,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		HC05_DEBUG("HC05 rsp:%s \r\n",HC05_RX_BUF);		
		res = Str_search(HC05_RX_BUF,HC05_RX_LEN,result);//��ѯ�ַ���
		return res;
}



u8 HCO5_AT_Confg(void)
{		
		u8 res=1;
	
		//����ATģʽ		
		HC05_AT=1;	//����AT����
		delay_s(1);
		HC05_ON;//������
		delay_s(1);
		HC05_AT=0;
		delay_s(1);	
	
		HC05_uart_init(38400);
		USART_ITConfig(HC05_UART, USART_IT_RXNE, ENABLE);//�������ڽ����ж�		

		HC05_DEBUG("AT���� ");	
		if(HC05_SendCmd(HC05_AT_TEST, "OK", 500) < 0)//ͨѶ����
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");

		HC05_DEBUG("���������� ");
		if(HC05_SendCmd(HC05_AT_NAME, "OK", 500) < 0)//����
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");

		HC05_DEBUG("�������� ");
		if(HC05_SendCmd(HC05_AT_RLOE, "OK", 500) < 0)//����ģʽ
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");
		
		HC05_DEBUG("�����¼�ĵ�ַ ");
		if(HC05_SendCmd(HC05_AT_RMAAD, "OK", 500) < 0)//����ӻ���ַ
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");
		
		HC05_DEBUG("���ò����� ");
		if(HC05_SendCmd(HC05_AT_UART, "OK", 500) < 0)	//���ò�����
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");

		HC05_DEBUG("�������ӷ�ʽ ");
		if(HC05_SendCmd(HC05_AT_CMODE, "OK", 500) < 0)//���ӷ�ʽ
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");

		HC05_DEBUG("�������� ");
		if(HC05_SendCmd(HC05_AT_PSWD, "OK", 500) < 0)	//��������
				goto end;
		LED_2 =!LED_2;
		HC05_DEBUG("�ɹ�\r\n");
	
		HC05_DEBUG("��λ ");
		if(HC05_SendCmd(HC05_AT_RESET, "OK", 500) < 0)	//��λ			
				goto end;
		
		res=0;//���óɹ�
end:
		USART_ITConfig(HC05_UART, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�	
		LED_2=LED_OFF;
		HC05_OFF;//��������Դ
		
		if(res == 0)
				HC05_DEBUG("�������óɹ�\r\n");
		else
				HC05_DEBUG("��������ʧ��\r\n");
		HC05_OFF;//��������Դ
		
		return res;
}




