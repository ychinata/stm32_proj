#include "ESP8266.h"
#include "usart.h"	
#include <string.h>
#include "delay.h"       
#include "led.h"       
#include "key.h"
#include "Timer.h"

#include "myQueue.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2022/04/30
//�汾��V1.3
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////


_WIFI_Info *WIFI_Info ; //WIFI���ݽṹ
u8 ESP8266_SET_FLAG=0;


//TCP������Ҫ�ֶ����� WIFI���� ����  IP��ַ �˿�
u8 M8266_CWJAP[60]="AT+CWJAP=\"OPPORENO\",\"12345678\"\r\n";		//����AP ssid: xxx  ���룺xxx
u8 M8266_SAVETRANSLINK_STA[60] = "AT+SAVETRANSLINK=1,\"192.168.0.166\",8083,\"TCP\"\r\n";//����������Ϣ	
u8 M8266_SAVETRANSLINK_AP[60] = "AT+SAVETRANSLINK=1,\"192.168.5.2\",10500,\"TCP\"\r\n" ;//����������Ϣ
	

//IO��ʼ��
void ESP8266_IO_Init(void)
{
 
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

		//RST
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			
		//EN
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			

		ESP8266_RST=1; //����λ
		//ESP8266_EN=0;	//�ϵ�
		ESP8266_PWOEROFF; //�ϵ�
}



////�����������ݸ�ʽ	
////AA AA F1 21(4*8+1=33) ... test CHECK	//4+32+1+1=38�ֽ�  8ͨ������
////38�β���1֡ ���г���1444�ֽ�
//u8 Wifi_Sendbuf_Init(void)
//{
//		u16 i;
//	
//		WIFI_Info->datalength = (ADS129X_USE_CANNLE+1)*4 +3;//һ�β���Ҫ���͵����ݳ��� 39
//		memset(WIFI_Info->sendbuf,0,WIFI_SEND_LENGTH);//����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//		//������λ��Э��
//		for(i=0;i<ADS129x_info.Ads129x_Write_Num;i++) //һ��N�β���
//		{		
//					//֡ͷ
//					WIFI_Info->sendbuf[0 + WIFI_Info->datalength*i] = 0xAA;
//					WIFI_Info->sendbuf[1 + WIFI_Info->datalength*i] = 0xFF;
//					WIFI_Info->sendbuf[2 + WIFI_Info->datalength*i] = 0xF1;
//					WIFI_Info->sendbuf[3 + WIFI_Info->datalength*i] = ADS129X_USE_CANNLE*4 +1;	//��Ч���ݳ��� 32 +1 = 33
//		}	
//			
//		return 1;
//}






//����ָ����ж�ָ���ַ����Ƿ񻺳�����
//����-1 û��ָ���Ĵ�
//0-125 �ַ����������е�λ��
s16 ESP8266_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	
	  ESP8266_CLEAR();//��ջ�����
    USARTx_Send(ESP8266_UART,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		//ESP8266_DEBUG("ESP8266 rsp:%s \r\n",ESP8266_RX_BUF);		
		res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,result);//��ѯ�ַ���
		return res;
}








//���ü��
void ESP8266_SET(void)
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
								ESP8266_SET_FLAG=1;//��������
								while( KEY1 == 0);
										
								return ;//�˳�����
						}
				}
				LED_2=LED_OFF;
		}
}



void ESP8266_Config_AP(void)
{
		ESP8266_Goto_Cmd(1);//����ATָ��ģʽ
	
//		ESP8266_DEBUG("�ָ���������\r\n");
//		ESP8266_SendCmd("AT+RESTORE\r\n", "OK", 500);////�ָ���������
//		ESP8266_Goto_Cmd(1);//����ATָ��ģʽ
		
		ESP8266_DEBUG("���ù���ģʽ��AP \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWMODE_AP, "OK", 500) >= 0)//ģ�鹤��ģʽ ����APģʽ
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("�����ȵ㣺 ESP8266AP ������\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWSAP, "OK", 500) >= 0)//���� ESP8266 softAP [WiFi����][����][�ŵ���][���ܷ�ʽ](����)
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("���� AP ��IP 192.168.5.1\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPAP, "OK", 500) >= 0)//���� ESP8266 AP ��IP ��ַ��[IP��ַ][����][��������] (����)
						break;
				else
						LED2=!LED2;				
		}
			
		ESP8266_DEBUG("���ù���ģʽ�������� \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPMUX, "OK", 500) >= 0)//���õ�����ģʽ������͸��������Ĭ�ϵ�����
						break;
				else
						LED2=!LED2;				
		}
	
		
		ESP8266_DEBUG("\r\n����TCP���� \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_SAVETRANSLINK_AP, "OK", 500) >= 0)//����TCP����
						break;
				else
				{
						LED2=!LED2;		
						ESP8266_DEBUG("ʧ��");
				}					
		}
				
		ESP8266_DEBUG("\r\n���ò�����͸�� \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(ESP_USARTDEF, "OK", 500) >= 0)//���ò����� ������
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_uart_init(ESP_BAUD); //��ʼ��	
		ESP8266_out_Cmd();//�˳�ָ��ģʽ
		
		LED1=LED_OFF;	
		LED2=LED_OFF;			
		ESP8266_DEBUG("���óɹ�\r\n");
}

void ESP8266_Config_STA(void)
{
		ESP8266_Goto_Cmd(1);//����ATָ��ģʽ	
	
//		ESP8266_DEBUG("�ָ���������\r\n");
//		ESP8266_SendCmd("AT+RESTORE\r\n", "OK", 500);////�ָ���������
//		ESP8266_Goto_Cmd(1);//����ATָ��ģʽ
		
		ESP8266_DEBUG("���ù���ģʽ��STA\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWMODE_STA, "OK", 500) >= 0)//ģ�鹤��ģʽ ����STAģʽ
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("���ù���ģʽ�������� \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPMUX, "OK", 500) >= 0)//���õ�����ģʽ������͸��������Ĭ�ϵ�����
						break;
				else
						LED2=!LED2;				
		}
		ESP8266_DEBUG("\r\n�ȴ�����WIFI\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_CWJAP, "OK", 1500) >= 0)//����AP WIFI�ȵ�SSID������
						break;
				else
				{
						LED2=!LED2;	
						ESP8266_DEBUG("�ȴ� ");
				}
		}
		
		ESP8266_DEBUG("\r\n�鿴IP��ַ\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIFSR, "OK", 500) >= 0)//��ѯ����
						break;
				else
				{
						LED2=!LED2;	
						ESP8266_DEBUG("�ȴ� ");
				}					
		}
		
		ESP8266_DEBUG("\r\n����TCP����\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_SAVETRANSLINK_STA, "OK", 500) >= 0)//����TCP����
						break;
				else
				{
						LED2=!LED2;		
						ESP8266_DEBUG("ʧ��");
				}					
		}
				
		ESP8266_DEBUG("\r\n���ò�����͸�� \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(ESP_USARTDEF, "OK", 500) >= 0)//���ò����� ������
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_uart_init(ESP_BAUD); //��ʼ��	
		ESP8266_out_Cmd();//�˳�ָ��ģʽ
		
		LED1=LED_OFF;	
		LED2=LED_OFF;			
		ESP8266_DEBUG("���óɹ�\r\n");
}

//����ATָ��ģʽ
void ESP8266_Goto_Cmd(u8 cmd)
{		
		if (cmd==1)
		{		
			ESP8266_PWOEROFF;
			delay_ms(200);
			ESP8266_PWOERON;
			//delay_s(2);
		
			ESP8266_RST=0;
			delay_ms(200);
			ESP8266_RST=1;
			delay_s(2);		
		}

		ESP8266_DEBUG("ATָ�����\r\n");
		ESP8266_DEBUG("Ĭ�ϲ����� %d\r\n",ESP_BAUD);
		ESP8266_uart_init(ESP_BAUD);
		ESP8266_SendCmd("+++", "OK", 1200);//�˳�͸��
		if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //ģ����Ч�Լ��
		{
				ESP8266_DEBUG("����ATָ��ģʽ\r\n");
				return;
		}
		else
		{
				ESP8266_DEBUG("����ATָ��ģʽʧ�ܣ���������WIFI\r\n");
		}
		while(1)
		{				
				ESP8266_DEBUG("��ǰ������ 115200\r\n");
				ESP8266_uart_init(115200);
				ESP8266_SendCmd("+++", "OK", 1200);//�˳�͸��
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //ģ����Ч�Լ��
				{
						break;
				}
				else
						LED2=!LED2;		
				
				ESP8266_DEBUG("��ǰ������ 460800\r\n");
				ESP8266_uart_init(460800);
				ESP8266_SendCmd("+++", "OK", 1200);//�˳�͸��
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //ģ����Ч�Լ��
				{
						break;
				}
				else
						LED2=!LED2;	
				
				ESP8266_DEBUG("��ǰ������ 921600\r\n");
				ESP8266_uart_init(921600);
				ESP8266_SendCmd("+++", "OK", 1200);//�˳�͸��
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //ģ����Ч�Լ��
				{
						break;
				}
				else
						LED2=!LED2;				
		}
}


//����͸��ģʽ
void ESP8266_out_Cmd(void)
{
		ESP8266_DEBUG("�˳�ATָ��ģʽ\r\n");
		ESP8266_SendCmd(RST, "OK", 500) ;//��λ
}


void ESP8266_Init(void)
{
	
		ESP8266_PWOEROFF;
		delay_ms(200);
		ESP8266_PWOERON;
		//delay_s(2);
		
		ESP8266_RST=0;
		delay_ms(200);
		ESP8266_RST=1;
		delay_s(2);
	
		
		ESP8266_DEBUG("WIFI��ʼ����ATָ�����\r\n");
		ESP8266_DEBUG("Ĭ�ϲ����� %d\r\n",ESP_BAUD);
		ESP8266_uart_init(ESP_BAUD);
		ESP8266_SendCmd("+++", "OK", 1200);//�˳�͸��
		if(ESP8266_SendCmd(AT, "OK", 500) < 0) //ģ����Ч�Լ��
		{
				ESP8266_DEBUG("����Ӧ������Ĭ�ϲ�������\r\n");
			
			#if WIFI_USE_STA ==1
				ESP8266_Config_STA();//����WIFI
			#elif WIFI_USE_AP ==1
				ESP8266_Config_AP();
			#endif
		}		
		if(ESP8266_SendCmd("AT+CWMODE?\r\n", "OK", 500) >= 0) //��ѯ��ǰģʽ
		{
			
				if(ESP8266_RX_BUF[21] =='1') //STAģʽ
				{
						ESP8266_DEBUG("��ǰWIFI STAģʽ\r\n");
				#if WIFI_USE_AP ==1
						ESP8266_Config_AP();
				#endif
				}
				else if(ESP8266_RX_BUF[21] =='2') //APģʽ
				{
						ESP8266_DEBUG("��ǰWIFI APģʽ\r\n");
				#if WIFI_USE_STA ==1
						ESP8266_Config_STA();//����WIFI
				#endif
				}
				else
				{
						ESP8266_DEBUG("��ǰWIFI ����ģʽ\r\n");
				#if WIFI_USE_STA ==1
						ESP8266_Config_STA();//����WIFI
				#elif WIFI_USE_AP ==1
						ESP8266_Config_AP();
				#endif
				}
 		}
		ESP8266_out_Cmd();
		ESP8266_DEBUG("WIFI��ʼ���ɹ�\r\n");
		//USART_ITConfig(ESP8266_UART, USART_IT_RXNE, DISABLE);//���ڽ����ж�	
		return;
}




//��������,STAģʽר��
void Set_WIFI(void)
{
		u8 u_recbuf[200];
		u16 u_reclen;
	
	
	#if WIFI_USE_STA ==1

		Main_printf("����AT����\r\n");
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���ڽ����ж�

		LED1=LED_OFF;
		LED2=LED_OFF;	
	
		Main_printf("���缰IP����\r\n\r\n");	
		Main_printf("AT��������Ч���ݲ�Ҫ��60�ֽ�,��:\r\n\r\n");
		Main_printf("������������ : AT+CWJAP_DEF=\"OPPORENO\",\"12345678\"\r\n");
		Main_printf("����TCP����  : AT+SAVETRANSLINK=1,\"192.168.0.166\",8083,\"TCP\"\r\n");
		Main_printf("�˳�����     : AT+OVER\r\n\r\n");
	
		while(1)
		{	
				if(USART_RX_STA & 0x8000)//���յ�����
				{
						//��������
						u_reclen=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
						memcpy(u_recbuf,USART_RX_BUF,u_reclen);//��������
						USART_RX_STA=0;
						if(u_recbuf[0]=='A' && u_recbuf[1]=='T' && u_recbuf[2]=='+')//ATָ��
						{
								if(	u_recbuf[3]=='C' && u_recbuf[4]=='W' && u_recbuf[5]=='J'  && u_recbuf[6]=='A' 
									&& u_recbuf[7]=='P' &&  u_recbuf[8]=='_' && u_recbuf[9]=='D' && u_recbuf[10]=='E'
									&& u_recbuf[11]=='F' && u_recbuf[12]=='=') //��������
								{
										Main_printf("������������ָ��:");
										memset(M8266_CWJAP, 0, 60);
										memcpy(M8266_CWJAP,u_recbuf,u_reclen);//��������
										M8266_CWJAP[u_reclen] = 0x0d;
										M8266_CWJAP[u_reclen+1] = 0x0a;
										ESP8266_DEBUG("%s\r\n",M8266_CWJAP);
								}

								else	if(	u_recbuf[3]=='S' && u_recbuf[4]=='A' && u_recbuf[5]=='V'  && u_recbuf[6]=='E' 
									&& u_recbuf[7]=='T' &&  u_recbuf[8]=='R' && u_recbuf[9]=='A' && u_recbuf[10]=='N'
									&& u_recbuf[11]=='S' &&  u_recbuf[12]=='L' && u_recbuf[13]=='I' && u_recbuf[14]=='N'
									&& u_recbuf[15]=='K' && u_recbuf[16]=='=') //��������
								{
										Main_printf("����IP�˿�ָ��:");
										memset(M8266_SAVETRANSLINK_STA, 0, 60);
										memcpy(M8266_SAVETRANSLINK_STA,u_recbuf,u_reclen);//��������
										M8266_SAVETRANSLINK_STA[u_reclen] = 0x0d;
										M8266_SAVETRANSLINK_STA[u_reclen+1] = 0x0a;
										ESP8266_DEBUG("%s\r\n",M8266_SAVETRANSLINK_STA);
								}
								else if(u_recbuf[3]=='O' && u_recbuf[4]=='V' && u_recbuf[5]=='E'  && u_recbuf[6]=='R') //�������
								{
										Main_printf("��ȴ��������\r\n");
										break;
								}
								else 
								{
										Main_printf("�����ָ��\r\n");
								}
						}
						else 
						{
								Main_printf("�����ָ��\r\n");
						}
				}
				else
				{
						delay_ms(10);   
				}		
		}
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�
		//Ҫ���õĲ���
		Main_printf("Ҫ������������:	%s",M8266_CWJAP);
		Main_printf("Ҫ����TCP����:	%s\r\n",M8266_SAVETRANSLINK_STA);		
		ESP8266_Config_STA(); //���ÿ����Զ�͸��
	#else
		ESP8266_Config_AP();	//���ÿ����Զ�͸��
	#endif
		LED1=LED_OFF;
		LED2=LED_OFF;	
}




//////////////////////////////////////////////////////////////////////
//�����Ĳ��Գ���
//////////////////////////////////////////////////////////////////////

/******************************************************************************
      ����˵����ָ������
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/*AT+CIFSR
+CIFSR:STAIP,"0.0.0.0"
+CIFSR:STAMAC,"e8:db:84:a2:e7:a9"

OK*/
/*AT+CIFSR
+CIFSR:STAIP,"192.168.43.213"
+CIFSR:STAMAC,"e8:db:84:a2:e7:a9"

OK*/
//��ȡģ���MAC �� IP
u8 WIFI_Get_IP_MAC(u8* buf1,u8* buf2)
{
		u8 res,i;
		u8 start,length;
		
		//ESP8266_DEBUG("��ѯ����IP\r\n");
		if(ESP8266_SendCmd("AT+CIFSR\r\n", "OK", 1000) >= 0)//��ѯ����IP��ַ����Ҫ����AP��ſ��Բ�ѯ
		{
//////////////////////////////////////////////////////////////////////////////////////////////////
				//��ѯ�豸IP
				res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"+CIFSR:STAIP,\"");//��ѯ�ַ���
				if(res>=0)
				{
						start = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"\"");//��ѯ��һ������λ���ַ���
						if(start>=0)
						{
//								ESP8266_DEBUG("��ʼ����λ��%d\r\n",start); //24
								length = Str_search(ESP8266_RX_BUF+start+1,ESP8266_RX_LEN,"\"");//��ѯ�ַ���
								if(length>=0)
								{
//										ESP8266_DEBUG("��������λ��%d\r\n",length);	//7
//										ESP8266_DEBUG("����IP\r\n");
//										for(i=0;i<length;i++)
//										{
//												printf("%c",ESP8266_RX_BUF[start+1+i]);
//										}
//										ESP8266_DEBUG("\r\n");
										memcpy(buf1,ESP8266_RX_BUF+start+1,length);//copy��������
								}
								else
										return 4;
						}
						else
								return 3;
				}	
				else			
						return 2;				
//////////////////////////////////////////////////////////////////////////////////////////////////
				//��ѯ�豸MAC
				res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"+CIFSR:STAMAC,\"");//��ѯ�ַ���
				if(res>=0)
				{
						start = Str_search(ESP8266_RX_BUF+res,ESP8266_RX_LEN,"\"");//��ѯ��һ������λ���ַ���
						if(start>=0)
						{
//								ESP8266_DEBUG("��ʼ����λ��%d\r\n",start); //24
								length = Str_search(ESP8266_RX_BUF+res+start+1,ESP8266_RX_LEN,"\"");//��ѯ�ַ���
								if(length>=0)
								{
//										ESP8266_DEBUG("��������λ��%d\r\n",length);	//7
//										ESP8266_DEBUG("MAC\r\n");
//										for(i=0;i<length;i++)
//										{
//												printf("%c",ESP8266_RX_BUF[start+1+i]);
//										}
//										ESP8266_DEBUG("\r\n");
											memcpy(buf2,ESP8266_RX_BUF+res+start+1,length);//copy��������
											return 0;
								}
								else
										return 7;
						}
						else
								return 6;
				}	
				else			
					return 5;	
		}
		else			
					return 1;
}



//����IPд��WIFI�ṹ
u8 WIFI_SAVE_IP(u8* buf,_WIFI_Info *WIFI_Info) 
{
		s16 res=0;
		u8 num=0;
		u8 i,j;
	
		u8 lenth;//���ݳ���
		u8 ip1=0,ip2=0,ip3=0,ip4=0; //��ֵ
		u8 ip1_add,ip2_add,ip3_add,ip4_add;//��ʼ��ַ
		u8 len_ip1,len_ip2,len_ip3,len_ip4;//����

		u8* REMOTE_ADDR2= buf;//Զ��IP
		
		lenth = strlen(REMOTE_ADDR2); //�õ��ַ�������		
		res = Char_search(REMOTE_ADDR2+0,30,'.');	//����1����.�� ���ֵ�λ��		
		len_ip1 = res; 	//ip1����
		ip1_add=0;			//ip1 ��ʼλ��
		ip2_add = res+1; 
		res = Char_search(REMOTE_ADDR2+ip2_add,30,'.'); //����2����.�� ���ֵ�λ��		
		len_ip2 = res+ip2_add -len_ip1 -1 ;		
		ip3_add = res+ip2_add+1;		
		res = Char_search(REMOTE_ADDR2+ip3_add,30,'.'); //����3����.�� ���ֵ�λ��		
		len_ip3 = res+ip3_add -len_ip1-len_ip2 -2;
		ip4_add = res+ip3_add+1;				
		len_ip4 = lenth-len_ip1-len_ip2-len_ip3-3;
		
		//ascii ת ��ֵ
		j=ip1_add;
		for(i=len_ip1;i>0;i--)
		{
				ip1 +=  (REMOTE_ADDR2[j]-'0') *mypow(10,(i-1)); 
				j++;
		}
		j=ip2_add;
		for(i=len_ip2;i>0;i--)
		{
				ip2 +=  (REMOTE_ADDR2[j]-'0') *mypow(10,(i-1));
				j++;			
		}
		j=ip3_add;
		for(i=len_ip3;i>0;i--)
		{
				ip3 +=  (REMOTE_ADDR2[j]-'0') *mypow(10,(i-1));
				j++;			
		}
		j=ip4_add;
		for(i=len_ip4;i>0;i--)
		{
				ip4 +=  (REMOTE_ADDR2[j]-'0') *mypow(10,(i-1)); 
				j++;
		}
			
		Main_printf("��ȡip %d.%d.%d.%d\r\n",ip1,ip2,ip3,ip4);

		WIFI_Info->IP1=ip1;
		WIFI_Info->IP2=ip2;
		WIFI_Info->IP3=ip3;
		WIFI_Info->IP4=ip4;
}
