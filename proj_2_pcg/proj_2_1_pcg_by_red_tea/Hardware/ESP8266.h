#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "sys.h"
#include "myQueue.h"


/////////////////////////////////////////////////////////////////////////////////////////////
#define _ESP8266_DEBUG_		1 				//���ڵ��Կ���
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
//ģʽѡ��ֻ�ܶ�ѡһ
#define WIFI_USE_STA		1	//STAģʽ ����Ҫ�����ȵ㣩
#define WIFI_USE_AP			0	//APģʽ ��PC���������豸���ȵ㣬��Ҫ���ƣ�
/////////////////////////////////////////////////////////////////////////////////////////////
////�豸����ѡ��ֻ�ܶ�ѡһ�������ñ��豸��
//#define ECG1CANNLE_RESP		1	//�����ĵ�+����
//#define ECG3CANNLE				0	//�����ĵ�
/////////////////////////////////////////////////////////////////////////////////////////////
//WIFI���ݽ���
#define ADS129x_WIFI_REC_EN		0	//WiFi����ʹ��
#define WIFI_REC_LENGTH				1460	//���ջ�����
/////////////////////////////////////////////////////////////////////////////////////////////
//������λ��  V7
//#define WIFI_REAL_DATA_LENGTH	9		//��Ч���ݳ���9 �������ñ��豸��
//#define WIFI_ALL_DATA_LENGTH	15	//һ֡���ݳ���15 �������ñ��豸��
#define WIFI_SEND_LENGTH	1443	//���ͻ����� һ֡���ݳ���39һ֡`�β��� 39*37=1444
/////////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ
#define WIFI_QUEUE_LENGTH	888	//��������������������Ч���ݳ���24*37=888
#define WIFI_QUEUE_SIZE	10	//���д�С
/////////////////////////////////////////////////////////////////////////////////////////////
//�������ݸ�ʽ
typedef struct {
		u16 datalength; 
		u8 *sendbuf;						//���ͻ�����
		u8 *recbuf;							//���ջ�����
		u8 sendlen;
		u8 reclen;
	
		u8 IP1;
		u8 IP2;
		u8 IP3;
		u8 IP4;
		u8 MAC[18];
	
		//QueueInfo *WIFI_Queue; //���нṹ
}_WIFI_Info;
extern  _WIFI_Info *WIFI_Info ; //�ϴ����ݽṹ
////////////////////////////////////////////////////////////////////////////////
//����DEBUG
#if _ESP8266_DEBUG_ == 1 
		#define ESP8266_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define ESP8266_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////
//TCP������Ҫ�ֶ����� WIFI���� ����  IP��ַ �˿�
#define AT          		"AT\r\n"	
#define CWMODE_STA   		"AT+CWMODE_DEF=1\r\n"	//����STAģʽ
#define CWMODE_AP    		"AT+CWMODE_DEF=2\r\n"	//����APģʽ
#define CWMODE_STA_AP   "AT+CWMODE_DEF=3\r\n"	//����STA+APģʽ

#define ESP_BAUD     		460800	//��ʱ������
//#define ESP_USARTDEF     "AT+UART_DEF=460800,8,1,0,0\r\n"	//�޸Ĳ�����	������ ���浽flash
#define ESP_USARTDEF     "AT+UART_DEF=460800,8,1,0,1\r\n"	//�޸Ĳ�����	ʹ��RTS���� ���浽flash

#define RST         "AT+RST\r\n"		//��λ
#define RESTORE     "AT+RESTORE\r\n" //�ָ���������

//WIFI���ָ��
#define CWAUTOCONN	"AT+CWAUTOCONN=1\r\n" //�ϵ��Զ�����AP
#define CSLAP     "AT+CWLAP\r\n"	//ɨ����Χ��AP
#define CSQAP     "AT+CWQAP\r\n"	//�Ͽ���AP������

//#define M8266_CWJAP ����AP ����������
#define CWSAP     "AT+CWSAP_DEF=\"ESP8266AP\",\"12345678\",5,0\r\n"	//��������AP�Ĳ��� SSID ������ ��������STA���Ϊ1
#define CWLIF     "AT+CWLIF\r\n"	//��ѯ��ǰ����AP��STA��Ϣ

#define CWDHCP     "AT+CWDHCP=0,1\r\n"	//�ر�DHCP���������޷����þ�̬IP
//#define CIPSTA     "AT+CIPSTA=\"192.168.5.100\",\"192.168.5.1\",\"255.255.255.0\"\r\n" //����STA��̬IP���� CWDHCP ָ���Ӱ��
#define CIPAP      "AT+CIPAP_DEF=\"192.168.5.1\"\r\n"	//����AP��̬IP���� CWDHCP ָ���Ӱ��


//TCP/TP���ָ��
#define CIFSR       "AT+CIFSR\r\n"			//��ѯ����IP��ַ��ֻ������AP����ܲ�ѯ������OK ����ERR


#define CIPMUX      "AT+CIPMUX=0\r\n"		//���õ�����ģʽ������͸��������Ĭ�ϵ�����
//#define CIPMODE   	 "AT+CIPMODE=1\r\n"	//����͸��ģʽ��ֻ������TCP������ģʽ,������
//#define CIPSEND     "AT+CIPSEND\r\n"		//�������ݣ����뿪��͸���������ӣ����з���>
//#define CIPSERVER    "AT+CIPSERVER=1\r\n"		//����TCP��������������ģʽ�²��ܿ���

////////////////////////////////////////////////////////////////////////////////
//TCP������Ҫ�ֶ����� WIFI���� ����  IP��ַ �˿�
extern	u8 M8266_CWJAP[60];		//����AP ssid: xxx  ���룺xxx
extern	u8 M8266_SAVETRANSLINK_STA[60];//����������Ϣ
extern  u8 ESP8266_SET_FLAG;

s16 ESP8266_SendCmd(u8* cmd, u8* result, u16 timeOut);

void ESP8266_IO_Init(void);
void ESP8266_Goto_Cmd(u8 cmd);
void ESP8266_out_Cmd(void);
void ESP8266_Init(void);

void ESP8266_Config_STA(void);
void ESP8266_Config_AP(void);

void ESP8266_SET(void); //�������
void Set_WIFI(void);//��������

u8 Wifi_Sendbuf_Init(void);

u8 WIFI_Get_IP_MAC(u8* buf1,u8* buf2);
u8 WIFI_SAVE_IP(u8* buf,_WIFI_Info *WIFI_Info) ;	 				    
#endif
