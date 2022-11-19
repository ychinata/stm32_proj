#include "ESP8266.h"
#include "usart.h"	
#include <string.h>
#include "delay.h"       
#include "led.h"       
#include "key.h"
#include "Timer.h"

#include "myQueue.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/04/30
//版本：V1.3
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


_WIFI_Info *WIFI_Info ; //WIFI数据结构
u8 ESP8266_SET_FLAG=0;


//TCP传输需要手动设置 WIFI名称 密码  IP地址 端口
u8 M8266_CWJAP[60]="AT+CWJAP=\"OPPORENO\",\"12345678\"\r\n";		//连接AP ssid: xxx  密码：xxx
u8 M8266_SAVETRANSLINK_STA[60] = "AT+SAVETRANSLINK=1,\"192.168.0.166\",8083,\"TCP\"\r\n";//保存配网信息	
u8 M8266_SAVETRANSLINK_AP[60] = "AT+SAVETRANSLINK=1,\"192.168.5.2\",10500,\"TCP\"\r\n" ;//保存配网信息
	

//IO初始化
void ESP8266_IO_Init(void)
{
 
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

		//RST
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			
		//EN
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);			

		ESP8266_RST=1; //不复位
		//ESP8266_EN=0;	//断电
		ESP8266_PWOEROFF; //断电
}



////匿名发送数据格式	
////AA AA F1 21(4*8+1=33) ... test CHECK	//4+32+1+1=38字节  8通道数据
////38次采样1帧 队列长度1444字节
//u8 Wifi_Sendbuf_Init(void)
//{
//		u16 i;
//	
//		WIFI_Info->datalength = (ADS129X_USE_CANNLE+1)*4 +3;//一次采样要发送的数据长度 39
//		memset(WIFI_Info->sendbuf,0,WIFI_SEND_LENGTH);//清零
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//		//匿名上位机协议
//		for(i=0;i<ADS129x_info.Ads129x_Write_Num;i++) //一包N次采样
//		{		
//					//帧头
//					WIFI_Info->sendbuf[0 + WIFI_Info->datalength*i] = 0xAA;
//					WIFI_Info->sendbuf[1 + WIFI_Info->datalength*i] = 0xFF;
//					WIFI_Info->sendbuf[2 + WIFI_Info->datalength*i] = 0xF1;
//					WIFI_Info->sendbuf[3 + WIFI_Info->datalength*i] = ADS129X_USE_CANNLE*4 +1;	//有效数据长度 32 +1 = 33
//		}	
//			
//		return 1;
//}






//发送指令，并判断指定字符串是否缓冲区中
//返回-1 没有指定的串
//0-125 字符串在数组中的位置
s16 ESP8266_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	
	  ESP8266_CLEAR();//清空缓冲区
    USARTx_Send(ESP8266_UART,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		//ESP8266_DEBUG("ESP8266 rsp:%s \r\n",ESP8266_RX_BUF);		
		res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,result);//查询字符串
		return res;
}








//设置检测
void ESP8266_SET(void)
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
								ESP8266_SET_FLAG=1;//设置蓝牙
								while( KEY1 == 0);
										
								return ;//退出函数
						}
				}
				LED_2=LED_OFF;
		}
}



void ESP8266_Config_AP(void)
{
		ESP8266_Goto_Cmd(1);//进入AT指令模式
	
//		ESP8266_DEBUG("恢复出厂设置\r\n");
//		ESP8266_SendCmd("AT+RESTORE\r\n", "OK", 500);////恢复出厂设置
//		ESP8266_Goto_Cmd(1);//进入AT指令模式
		
		ESP8266_DEBUG("设置工作模式：AP \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWMODE_AP, "OK", 500) >= 0)//模块工作模式 设置AP模式
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("建立热点： ESP8266AP 无密码\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWSAP, "OK", 500) >= 0)//配置 ESP8266 softAP [WiFi名称][密码][信道号][加密方式](保存)
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("设置 AP 的IP 192.168.5.1\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPAP, "OK", 500) >= 0)//设置 ESP8266 AP 的IP 地址，[IP地址][网关][子网掩码] (保存)
						break;
				else
						LED2=!LED2;				
		}
			
		ESP8266_DEBUG("设置工作模式：单链接 \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPMUX, "OK", 500) >= 0)//设置单链接模式，用于透传，开机默认单链接
						break;
				else
						LED2=!LED2;				
		}
	
		
		ESP8266_DEBUG("\r\n建立TCP连接 \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_SAVETRANSLINK_AP, "OK", 500) >= 0)//建立TCP连接
						break;
				else
				{
						LED2=!LED2;		
						ESP8266_DEBUG("失败");
				}					
		}
				
		ESP8266_DEBUG("\r\n设置波特率透传 \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(ESP_USARTDEF, "OK", 500) >= 0)//设置波特率 及流控
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_uart_init(ESP_BAUD); //初始化	
		ESP8266_out_Cmd();//退出指令模式
		
		LED1=LED_OFF;	
		LED2=LED_OFF;			
		ESP8266_DEBUG("设置成功\r\n");
}

void ESP8266_Config_STA(void)
{
		ESP8266_Goto_Cmd(1);//进入AT指令模式	
	
//		ESP8266_DEBUG("恢复出厂设置\r\n");
//		ESP8266_SendCmd("AT+RESTORE\r\n", "OK", 500);////恢复出厂设置
//		ESP8266_Goto_Cmd(1);//进入AT指令模式
		
		ESP8266_DEBUG("设置工作模式：STA\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CWMODE_STA, "OK", 500) >= 0)//模块工作模式 设置STA模式
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_DEBUG("设置工作模式：单链接 \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIPMUX, "OK", 500) >= 0)//设置单链接模式，用于透传，开机默认单链接
						break;
				else
						LED2=!LED2;				
		}
		ESP8266_DEBUG("\r\n等待连接WIFI\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_CWJAP, "OK", 1500) >= 0)//连接AP WIFI热点SSID和密码
						break;
				else
				{
						LED2=!LED2;	
						ESP8266_DEBUG("等待 ");
				}
		}
		
		ESP8266_DEBUG("\r\n查看IP地址\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(CIFSR, "OK", 500) >= 0)//查询连接
						break;
				else
				{
						LED2=!LED2;	
						ESP8266_DEBUG("等待 ");
				}					
		}
		
		ESP8266_DEBUG("\r\n建立TCP连接\r\n");
		while(1)
		{
				if(ESP8266_SendCmd(M8266_SAVETRANSLINK_STA, "OK", 500) >= 0)//建立TCP连接
						break;
				else
				{
						LED2=!LED2;		
						ESP8266_DEBUG("失败");
				}					
		}
				
		ESP8266_DEBUG("\r\n设置波特率透传 \r\n");
		while(1)
		{
				if(ESP8266_SendCmd(ESP_USARTDEF, "OK", 500) >= 0)//设置波特率 及流控
						break;
				else
						LED2=!LED2;				
		}
		
		ESP8266_uart_init(ESP_BAUD); //初始化	
		ESP8266_out_Cmd();//退出指令模式
		
		LED1=LED_OFF;	
		LED2=LED_OFF;			
		ESP8266_DEBUG("设置成功\r\n");
}

//进入AT指令模式
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

		ESP8266_DEBUG("AT指令测试\r\n");
		ESP8266_DEBUG("默认波特率 %d\r\n",ESP_BAUD);
		ESP8266_uart_init(ESP_BAUD);
		ESP8266_SendCmd("+++", "OK", 1200);//退出透传
		if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //模块有效性检查
		{
				ESP8266_DEBUG("进入AT指令模式\r\n");
				return;
		}
		else
		{
				ESP8266_DEBUG("进入AT指令模式失败，重新配置WIFI\r\n");
		}
		while(1)
		{				
				ESP8266_DEBUG("当前波特率 115200\r\n");
				ESP8266_uart_init(115200);
				ESP8266_SendCmd("+++", "OK", 1200);//退出透传
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //模块有效性检查
				{
						break;
				}
				else
						LED2=!LED2;		
				
				ESP8266_DEBUG("当前波特率 460800\r\n");
				ESP8266_uart_init(460800);
				ESP8266_SendCmd("+++", "OK", 1200);//退出透传
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //模块有效性检查
				{
						break;
				}
				else
						LED2=!LED2;	
				
				ESP8266_DEBUG("当前波特率 921600\r\n");
				ESP8266_uart_init(921600);
				ESP8266_SendCmd("+++", "OK", 1200);//退出透传
				if(ESP8266_SendCmd(AT, "OK", 500) >= 0) //模块有效性检查
				{
						break;
				}
				else
						LED2=!LED2;				
		}
}


//进入透传模式
void ESP8266_out_Cmd(void)
{
		ESP8266_DEBUG("退出AT指令模式\r\n");
		ESP8266_SendCmd(RST, "OK", 500) ;//复位
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
	
		
		ESP8266_DEBUG("WIFI初始化，AT指令测试\r\n");
		ESP8266_DEBUG("默认波特率 %d\r\n",ESP_BAUD);
		ESP8266_uart_init(ESP_BAUD);
		ESP8266_SendCmd("+++", "OK", 1200);//退出透传
		if(ESP8266_SendCmd(AT, "OK", 500) < 0) //模块有效性检查
		{
				ESP8266_DEBUG("无响应，按照默认参数设置\r\n");
			
			#if WIFI_USE_STA ==1
				ESP8266_Config_STA();//设置WIFI
			#elif WIFI_USE_AP ==1
				ESP8266_Config_AP();
			#endif
		}		
		if(ESP8266_SendCmd("AT+CWMODE?\r\n", "OK", 500) >= 0) //查询当前模式
		{
			
				if(ESP8266_RX_BUF[21] =='1') //STA模式
				{
						ESP8266_DEBUG("当前WIFI STA模式\r\n");
				#if WIFI_USE_AP ==1
						ESP8266_Config_AP();
				#endif
				}
				else if(ESP8266_RX_BUF[21] =='2') //AP模式
				{
						ESP8266_DEBUG("当前WIFI AP模式\r\n");
				#if WIFI_USE_STA ==1
						ESP8266_Config_STA();//设置WIFI
				#endif
				}
				else
				{
						ESP8266_DEBUG("当前WIFI 其他模式\r\n");
				#if WIFI_USE_STA ==1
						ESP8266_Config_STA();//设置WIFI
				#elif WIFI_USE_AP ==1
						ESP8266_Config_AP();
				#endif
				}
 		}
		ESP8266_out_Cmd();
		ESP8266_DEBUG("WIFI初始化成功\r\n");
		//USART_ITConfig(ESP8266_UART, USART_IT_RXNE, DISABLE);//串口接收中断	
		return;
}




//串口配网,STA模式专用
void Set_WIFI(void)
{
		u8 u_recbuf[200];
		u16 u_reclen;
	
	
	#if WIFI_USE_STA ==1

		Main_printf("开启AT配网\r\n");
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//串口接受中断

		LED1=LED_OFF;
		LED2=LED_OFF;	
	
		Main_printf("网络及IP设置\r\n\r\n");	
		Main_printf("AT配网，有效内容不要超60字节,例:\r\n\r\n");
		Main_printf("设置名称密码 : AT+CWJAP_DEF=\"OPPORENO\",\"12345678\"\r\n");
		Main_printf("设置TCP连接  : AT+SAVETRANSLINK=1,\"192.168.0.166\",8083,\"TCP\"\r\n");
		Main_printf("退出设置     : AT+OVER\r\n\r\n");
	
		while(1)
		{	
				if(USART_RX_STA & 0x8000)//接收到数据
				{
						//搬运数据
						u_reclen=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
						memcpy(u_recbuf,USART_RX_BUF,u_reclen);//复制内容
						USART_RX_STA=0;
						if(u_recbuf[0]=='A' && u_recbuf[1]=='T' && u_recbuf[2]=='+')//AT指令
						{
								if(	u_recbuf[3]=='C' && u_recbuf[4]=='W' && u_recbuf[5]=='J'  && u_recbuf[6]=='A' 
									&& u_recbuf[7]=='P' &&  u_recbuf[8]=='_' && u_recbuf[9]=='D' && u_recbuf[10]=='E'
									&& u_recbuf[11]=='F' && u_recbuf[12]=='=') //设置名称
								{
										Main_printf("设置名称密码指令:");
										memset(M8266_CWJAP, 0, 60);
										memcpy(M8266_CWJAP,u_recbuf,u_reclen);//复制内容
										M8266_CWJAP[u_reclen] = 0x0d;
										M8266_CWJAP[u_reclen+1] = 0x0a;
										ESP8266_DEBUG("%s\r\n",M8266_CWJAP);
								}

								else	if(	u_recbuf[3]=='S' && u_recbuf[4]=='A' && u_recbuf[5]=='V'  && u_recbuf[6]=='E' 
									&& u_recbuf[7]=='T' &&  u_recbuf[8]=='R' && u_recbuf[9]=='A' && u_recbuf[10]=='N'
									&& u_recbuf[11]=='S' &&  u_recbuf[12]=='L' && u_recbuf[13]=='I' && u_recbuf[14]=='N'
									&& u_recbuf[15]=='K' && u_recbuf[16]=='=') //设置名称
								{
										Main_printf("设置IP端口指令:");
										memset(M8266_SAVETRANSLINK_STA, 0, 60);
										memcpy(M8266_SAVETRANSLINK_STA,u_recbuf,u_reclen);//复制内容
										M8266_SAVETRANSLINK_STA[u_reclen] = 0x0d;
										M8266_SAVETRANSLINK_STA[u_reclen+1] = 0x0a;
										ESP8266_DEBUG("%s\r\n",M8266_SAVETRANSLINK_STA);
								}
								else if(u_recbuf[3]=='O' && u_recbuf[4]=='V' && u_recbuf[5]=='E'  && u_recbuf[6]=='R') //设置完成
								{
										Main_printf("请等待设置完成\r\n");
										break;
								}
								else 
								{
										Main_printf("错误的指令\r\n");
								}
						}
						else 
						{
								Main_printf("错误的指令\r\n");
						}
				}
				else
				{
						delay_ms(10);   
				}		
		}
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭串口接受中断
		//要设置的参数
		Main_printf("要设置名称密码:	%s",M8266_CWJAP);
		Main_printf("要设置TCP连接:	%s\r\n",M8266_SAVETRANSLINK_STA);		
		ESP8266_Config_STA(); //设置开机自动透传
	#else
		ESP8266_Config_AP();	//设置开机自动透传
	#endif
		LED1=LED_OFF;
		LED2=LED_OFF;	
}




//////////////////////////////////////////////////////////////////////
//其他的测试程序
//////////////////////////////////////////////////////////////////////

/******************************************************************************
      函数说明：指数运算
      入口数据：m底数，n指数
      返回值：  无
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
//获取模块的MAC 和 IP
u8 WIFI_Get_IP_MAC(u8* buf1,u8* buf2)
{
		u8 res,i;
		u8 start,length;
		
		//ESP8266_DEBUG("查询本地IP\r\n");
		if(ESP8266_SendCmd("AT+CIFSR\r\n", "OK", 1000) >= 0)//查询本地IP地址，需要连接AP后才可以查询
		{
//////////////////////////////////////////////////////////////////////////////////////////////////
				//查询设备IP
				res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"+CIFSR:STAIP,\"");//查询字符串
				if(res>=0)
				{
						start = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"\"");//查询第一个引号位置字符串
						if(start>=0)
						{
//								ESP8266_DEBUG("起始引号位置%d\r\n",start); //24
								length = Str_search(ESP8266_RX_BUF+start+1,ESP8266_RX_LEN,"\"");//查询字符串
								if(length>=0)
								{
//										ESP8266_DEBUG("后续引号位置%d\r\n",length);	//7
//										ESP8266_DEBUG("本地IP\r\n");
//										for(i=0;i<length;i++)
//										{
//												printf("%c",ESP8266_RX_BUF[start+1+i]);
//										}
//										ESP8266_DEBUG("\r\n");
										memcpy(buf1,ESP8266_RX_BUF+start+1,length);//copy至缓存区
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
				//查询设备MAC
				res = Str_search(ESP8266_RX_BUF,ESP8266_RX_LEN,"+CIFSR:STAMAC,\"");//查询字符串
				if(res>=0)
				{
						start = Str_search(ESP8266_RX_BUF+res,ESP8266_RX_LEN,"\"");//查询第一个引号位置字符串
						if(start>=0)
						{
//								ESP8266_DEBUG("起始引号位置%d\r\n",start); //24
								length = Str_search(ESP8266_RX_BUF+res+start+1,ESP8266_RX_LEN,"\"");//查询字符串
								if(length>=0)
								{
//										ESP8266_DEBUG("后续引号位置%d\r\n",length);	//7
//										ESP8266_DEBUG("MAC\r\n");
//										for(i=0;i<length;i++)
//										{
//												printf("%c",ESP8266_RX_BUF[start+1+i]);
//										}
//										ESP8266_DEBUG("\r\n");
											memcpy(buf2,ESP8266_RX_BUF+res+start+1,length);//copy至缓存区
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



//本地IP写入WIFI结构
u8 WIFI_SAVE_IP(u8* buf,_WIFI_Info *WIFI_Info) 
{
		s16 res=0;
		u8 num=0;
		u8 i,j;
	
		u8 lenth;//数据长度
		u8 ip1=0,ip2=0,ip3=0,ip4=0; //数值
		u8 ip1_add,ip2_add,ip3_add,ip4_add;//起始地址
		u8 len_ip1,len_ip2,len_ip3,len_ip4;//长度

		u8* REMOTE_ADDR2= buf;//远程IP
		
		lenth = strlen(REMOTE_ADDR2); //得到字符串长度		
		res = Char_search(REMOTE_ADDR2+0,30,'.');	//检测第1个‘.’ 出现的位置		
		len_ip1 = res; 	//ip1长度
		ip1_add=0;			//ip1 起始位置
		ip2_add = res+1; 
		res = Char_search(REMOTE_ADDR2+ip2_add,30,'.'); //检测第2个‘.’ 出现的位置		
		len_ip2 = res+ip2_add -len_ip1 -1 ;		
		ip3_add = res+ip2_add+1;		
		res = Char_search(REMOTE_ADDR2+ip3_add,30,'.'); //检测第3个‘.’ 出现的位置		
		len_ip3 = res+ip3_add -len_ip1-len_ip2 -2;
		ip4_add = res+ip3_add+1;				
		len_ip4 = lenth-len_ip1-len_ip2-len_ip3-3;
		
		//ascii 转 数值
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
			
		Main_printf("获取ip %d.%d.%d.%d\r\n",ip1,ip2,ip3,ip4);

		WIFI_Info->IP1=ip1;
		WIFI_Info->IP2=ip2;
		WIFI_Info->IP3=ip3;
		WIFI_Info->IP4=ip4;
}
