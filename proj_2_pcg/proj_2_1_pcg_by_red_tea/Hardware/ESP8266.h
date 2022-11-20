#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "sys.h"
#include "myQueue.h"


/////////////////////////////////////////////////////////////////////////////////////////////
#define _ESP8266_DEBUG_		1 				//串口调试开关
/////////////////////////////////////////////////////////////////////////////////////////////
#define ESP8266_UART   		USART2
#define ESP8266_uart_init(x)   uart2_init(x)
#define ESP8266_RX_BUF		g_Usart2RxBuf
#define ESP8266_RX_LEN		USART2_RX_LEN
#define ESP8266_CLEAR()   USART2_Clear()
/////////////////////////////////////////////////////////////////////////////////////////////
#define ESP8266_EN	 	PAout(7) 
#define ESP8266_RST	 	PAout(4) 
//#define ESP8266_POWER	 	PBout(6) 

#define ESP8266_PWOERON  	ESP8266_EN=1
#define ESP8266_PWOEROFF	ESP8266_EN=0
//#define ESP8266_PWOERON  	ESP8266_POWER=0
//#define ESP8266_PWOEROFF	ESP8266_POWER=1
/////////////////////////////////////////////////////////////////////////////////////////////
//模式选择，只能二选一
#define WIFI_USE_STA		1	//STA模式 （需要连接热点）
#define WIFI_USE_AP			0	//AP模式 （PC主动连接设备的热点，需要完善）
/////////////////////////////////////////////////////////////////////////////////////////////
////设备功能选择，只能二选一（不适用本设备）
//#define ECG1CANNLE_RESP		1	//单导心电+呼吸
//#define ECG3CANNLE				0	//三导心电
/////////////////////////////////////////////////////////////////////////////////////////////
//WIFI数据接收
#define ADS129x_WIFI_REC_EN		0	//WiFi接收使能
#define WIFI_REC_LENGTH				1460	//接收缓冲区
/////////////////////////////////////////////////////////////////////////////////////////////
//匿名上位机  V7
//#define WIFI_REAL_DATA_LENGTH	9		//有效数据长度9 （不适用本设备）
//#define WIFI_ALL_DATA_LENGTH	15	//一帧数据长度15 （不适用本设备）
#define WIFI_SEND_LENGTH	1443	//发送缓冲区 一帧数据长度39一帧`次采样 39*37=1444
/////////////////////////////////////////////////////////////////////////////////////////////
//队列信息
#define WIFI_QUEUE_LENGTH	888	//（队列容量）缓冲区有效数据长度24*37=888
#define WIFI_QUEUE_SIZE	10	//队列大小
/////////////////////////////////////////////////////////////////////////////////////////////
//发送数据格式
typedef struct {
		u16 datalength; 
		u8 *sendbuf;						//发送缓冲区
		u8 *recbuf;							//接收缓冲区
		u8 sendlen;
		u8 reclen;
	
		u8 IP1;
		u8 IP2;
		u8 IP3;
		u8 IP4;
		u8 MAC[18];
	
		//QueueInfo *WIFI_Queue; //队列结构
}_WIFI_Info;
extern  _WIFI_Info *WIFI_Info ; //上传数据结构
////////////////////////////////////////////////////////////////////////////////
//串口DEBUG
#if _ESP8266_DEBUG_ == 1 
		#define ESP8266_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define ESP8266_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////
//TCP传输需要手动设置 WIFI名称 密码  IP地址 端口
#define AT          		"AT\r\n"	
#define CWMODE_STA   		"AT+CWMODE_DEF=1\r\n"	//设置STA模式
#define CWMODE_AP    		"AT+CWMODE_DEF=2\r\n"	//设置AP模式
#define CWMODE_STA_AP   "AT+CWMODE_DEF=3\r\n"	//设置STA+AP模式

#define ESP_BAUD     		460800	//临时波特率
//#define ESP_USARTDEF     "AT+UART_DEF=460800,8,1,0,0\r\n"	//修改波特率	无流控 保存到flash
#define ESP_USARTDEF     "AT+UART_DEF=460800,8,1,0,1\r\n"	//修改波特率	使能RTS流控 保存到flash

#define RST         "AT+RST\r\n"		//复位
#define RESTORE     "AT+RESTORE\r\n" //恢复出厂设置

//WIFI相关指令
#define CWAUTOCONN	"AT+CWAUTOCONN=1\r\n" //上电自动连接AP
#define CSLAP     "AT+CWLAP\r\n"	//扫描周围的AP
#define CSQAP     "AT+CWQAP\r\n"	//断开与AP的连接

//#define M8266_CWJAP 连接AP 存入数组中
#define CWSAP     "AT+CWSAP_DEF=\"ESP8266AP\",\"12345678\",5,0\r\n"	//配置自身AP的参数 SSID 密码无 运行连入STA最大为1
#define CWLIF     "AT+CWLIF\r\n"	//查询当前连接AP的STA信息

#define CWDHCP     "AT+CWDHCP=0,1\r\n"	//关闭DHCP，开启后无法设置静态IP
//#define CIPSTA     "AT+CIPSTA=\"192.168.5.100\",\"192.168.5.1\",\"255.255.255.0\"\r\n" //设置STA静态IP，与 CWDHCP 指令互相影响
#define CIPAP      "AT+CIPAP_DEF=\"192.168.5.1\"\r\n"	//设置AP静态IP，与 CWDHCP 指令互相影响


//TCP/TP相关指令
#define CIFSR       "AT+CIFSR\r\n"			//查询本地IP地址，只有连上AP后才能查询，返回OK 或者ERR


#define CIPMUX      "AT+CIPMUX=0\r\n"		//设置单链接模式，用于透传，开机默认单链接
//#define CIPMODE   	 "AT+CIPMODE=1\r\n"	//设置透传模式，只适用于TCP单链接模式,不保存
//#define CIPSEND     "AT+CIPSEND\r\n"		//发送数据，必须开启透传及单链接，换行返回>
//#define CIPSERVER    "AT+CIPSERVER=1\r\n"		//建立TCP服务器，多连接模式下才能开启

////////////////////////////////////////////////////////////////////////////////
//TCP传输需要手动设置 WIFI名称 密码  IP地址 端口
extern	u8 M8266_CWJAP[60];		//连接AP ssid: xxx  密码：xxx
extern	u8 M8266_SAVETRANSLINK_STA[60];//保存配网信息
extern  u8 ESP8266_SET_FLAG;

s16 ESP8266_SendCmd(u8* cmd, u8* result, u16 timeOut);

void ESP8266_IO_Init(void);
void ESP8266_Goto_Cmd(u8 cmd);
void ESP8266_out_Cmd(void);
void ESP8266_Init(void);

void ESP8266_Config_STA(void);
void ESP8266_Config_AP(void);

void ESP8266_SET(void); //配网检测
void Set_WIFI(void);//配网设置

u8 Wifi_Sendbuf_Init(void);

u8 WIFI_Get_IP_MAC(u8* buf1,u8* buf2);
u8 WIFI_SAVE_IP(u8* buf,_WIFI_Info *WIFI_Info) ;	 				    
#endif
