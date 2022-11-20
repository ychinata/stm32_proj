#include "usart.h"	 
#include "stdarg.h"
#include "string.h"
#include "stdio.h"	

////////////////////////////////////////////////////////////////////////// 
_UART_Info *UART_Info ; //���ݽṹ



void uart1_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9   
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10    
	 //Usart1 NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

 
//����1������
u8 USART_RX_BUF[USART_REC_LEN]; 
u16 USART_RX_STA=0;       //����״̬���	  

//����1�жϷ������
void USART1_IRQHandler(void)                	//����1�жϷ������
{		
	u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)
					USART_RX_STA=0;//���մ���,���¿�ʼ
				else 
					USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)
						USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
    } 
} 

////����1������
//u8 USART1_RX_BUF[USART_REC_LEN] ;           
//u8 USART1_TX_BUF[USART_REC_LEN] ; 
//u16 USART1_RX_LEN;
//u16 USART1_TIME;
//u8 USART1_RX_EVENT;
////****************************************************************************** 
////����1�жϳ���       
////******************************************************************************
//void USART1_IRQHandler(void)  
//{
//		u8 Res;
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//		{
//				USART1_TIME=0;

//				Res = USART_ReceiveData(USART1);	//��ȡ���յ�������
//				USART1_RX_BUF[USART1_RX_LEN] = Res;
//				USART1_RX_LEN++;
//				if(USART1_RX_LEN == USART_REC_LEN)
//					USART1_RX_LEN=0;
//		}   		 
//}
	


//******************************************************************************              
//name:             USART2_Init             
//introduce:        USART2��ʼ��        
//****************************************************************************** 
void uart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO
	USART_InitTypeDef USART_InitStructure;   //���ڽṹ�嶨��
	NVIC_InitTypeDef NVIC_InitStructure;  //�жϽṹ�嶨��

	//����GPIOA��USART2 ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO , ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//CTS��ģ�������ˣ�����ģ��֪ͨMCU��ģ���Ƿ�׼���� ��Ч��ƽΪ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����GPIO��ģʽ��IO�� TX     
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          //�������PA2                                                                                          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ����������
	//RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;          //��������PA3                                                                              
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);       			 //��ʼ���������
		
	//NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //�����ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           //��USART2��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //��Ӧ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//��������//����ͨ�Ų�����		
	USART_InitStructure.USART_BaudRate = bound;                    ////������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   ////���ݳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        ////1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;           ////��Ч��
	//USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None; //����Ӳ��������ʧ��
	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_CTS; //����Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  //���÷���ʹ�ܣ�����ʹ�� 	
	USART_Init(USART2, &USART_InitStructure);      			//USART_Init��ʼ��                                                                                      
		
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 			//����2�����ж� 
  	USART_Cmd(USART2, ENABLE);                     			//ʹ��USART2                                                                                                  
} 


//����2������
u8 g_Usart2RxBuf[USART_REC_LEN] ;           
u8 g_Usart2TxBuf[USART_REC_LEN] ; 
u16 USART2_RX_LEN;
u16 USART2_TIME;
u16 USART2_RX_EVENT;
//****************************************************************************** 
//����2�жϳ���       
//******************************************************************************
void USART2_IRQHandler(void)  
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART2_TIME=0;
	
		Res = USART_ReceiveData(USART2);	//��ȡ���յ�������	
		g_Usart2RxBuf[USART2_RX_LEN] = Res;
		USART2_RX_LEN++;
		if(USART2_RX_LEN == USART_REC_LEN)
			USART2_RX_LEN=0;
	}   		 
} 



//void uart3_init(u32 bound)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;   //����һ���ṹ�������������ʼ��GPIO
//  	USART_InitTypeDef USART_InitStructure;   //���ڽṹ�嶨��
//		NVIC_InitTypeDef NVIC_InitStructure;  //�жϽṹ�嶨��
// 
//  	//����GPIOA��USART3 ʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE);  
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//  	
//		//����GPIO��ģʽ��IO�� TX     
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;          //�������                                                                                        
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
//  	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ����������
//		//RX
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;          //��������                                                                         
//  	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;//��������
//  	GPIO_Init(GPIOB,&GPIO_InitStructure);       			 //��ʼ���������
//					
//		//��������//����ͨ�Ų�����		
//		USART_InitStructure.USART_BaudRate = bound;                    ////������
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b;   ////���ݳ�8λ
//		USART_InitStructure.USART_StopBits = USART_StopBits_1;        ////1λֹͣλ
//		USART_InitStructure.USART_Parity = USART_Parity_No;           ////��Ч��
//  	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None; //����Ӳ��������ʧ��
//  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  //���÷���ʹ�ܣ�����ʹ��	
//		USART_Init(USART3, &USART_InitStructure);      			//USART_Init��ʼ��                                                                                      
//			
//		//NVIC ����
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //�����ж����ȼ�����
//  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;           //��USART2��ȫ���ж�
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�Ϊ1
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //��Ӧ���ȼ�Ϊ2
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ʹ��
//  	NVIC_Init(&NVIC_InitStructure);
//  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 			//���ڽ����ж� 
//		
//  	USART_Cmd(USART3, ENABLE);                     			//ʹ��USART     
//} 



////����3������
//u8 USART3_RX_BUF[USART_REC_LEN] ;           
//u8 USART3_TX_BUF[USART_REC_LEN] ; 
//u16 USART3_RX_LEN;
//u16 USART3_TIME;
////****************************************************************************** 
////����3�жϳ���       
////******************************************************************************
//void USART3_IRQHandler(void)  
//{
//		u8 Res;
//		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//		{
//				USART3_TIME=0;

//				Res = USART_ReceiveData(USART3);	//��ȡ���յ�������	
//				USART3_RX_BUF[USART3_RX_LEN] = Res;
//				USART3_RX_LEN++;
//				if(USART3_RX_LEN == USART_REC_LEN)
//					USART3_RX_LEN=0;
//		}   		 
//} 







//���ڻ�������
void USART2_Clear(void)
{
	memset(g_Usart2RxBuf, 0, sizeof(g_Usart2RxBuf));
    USART2_RX_LEN = 0;
}



//USARTx_Send
//���ڷ����ַ���
void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len)  
{  
    u8 i = 0;  
    for(i = 0; i < Len; i++)  
    {  
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);     
        USART_SendData(USARTx, Data[i]);                                                     
    }  
} 



////����2,printf ����
////ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
//void u2_printf(char* fmt,...)  
//{ 
//	u16 i,j; 
//	va_list ap; 
//	va_start(ap,fmt);
//	vsprintf((char*)g_Usart2TxBuf,fmt,ap);
//	va_end(ap);
//	i=strlen((const char*)g_Usart2TxBuf);		//�˴η������ݵĳ���
//	for(j=0;j<i;j++)							//ѭ����������
//	{
//	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
//		USART_SendData(USART2,g_Usart2TxBuf[j]); 
//	} 
//}
////����3,printf ����
////ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
//void u3_printf(char* fmt,...)  
//{ 
//	u16 i,j; 
//	va_list ap; 
//	va_start(ap,fmt);
//	vsprintf((char*)USART3_TX_BUF,fmt,ap);
//	va_end(ap);
//	i=strlen((const char*)USART3_TX_BUF);		//�˴η������ݵĳ���
//	for(j=0;j<i;j++)							//ѭ����������
//	{
//	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
//		USART_SendData(USART3,USART3_TX_BUF[j]); 
//	} 
//}



//�ж�ָ���ַ����Ƿ���������
//����-1 û��ָ���Ĵ�
//0-125 �ַ����������е�λ��
s16 Str_search(u8* buf,u8 len,u8* result)
{
	u8 *res;
	u8 num;
	
	//strstr(str1,str2)����str2��str1���״γ��ֵĵ�ַ
	res =  strstr((const u8 *)buf, (const u8 *)result); 
	if(res != NULL)	//�ж��Ƿ���Ԥ�ڵĽ��
	{
			num=res-buf;
			if(num<len)
			{
				//Main_printf("�õ�Ԥ�ڽ��,�����е� %d ����ʼ\r\n",res-buf);
				return res-buf;
			}
	}
	//Main_printf("û��Ԥ�ڽ��\r\n");
	return -1;
}

//�ж�ָ���ַ��Ƿ���������
//����-1 û��ָ���Ĵ�
//0-125 �ַ����������е�λ��
s16 Char_search(u8* buf,u8 len,u8 result)
{
	u8 *res;
	u8 num;
	
	res =  strchr((const u8 *)buf, result); 	
	if(res != NULL)	//�ж��Ƿ���Ԥ�ڵĽ��
	{
			num=res-buf;
			if(num<len)
			{
				//Main_printf("�õ�Ԥ�ڽ��,�����е� %d ����ʼ\r\n",res-buf);
				return res-buf;
			}
	}
	//Main_printf("û��Ԥ�ڽ��\r\n");
	return -1;
}


//////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 

//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//		USART1->DR = (u8) ch;      
//		return ch;
//}

/*ʹ��microLib�ķ���*/
/*��ϸ���ֲ��ʱ���� TC �� TXE ��־λ�ڸ�λ��ʱ����1 ������һ��whileѭ������û���õġ�
�����������״ε�һ���ַ���û�б�������ͱ�������ַ����ǵ������ʵ�ʿ����Ķ�ʧ����
��һ���ȡSR�Ĵ������ڶ���дDR�Ĵ��� �պ������TC��־λ ����һ��whileѭ�����������ˡ�*/ 
int fputc(int ch, FILE *f)
{
		/* TC TXE ��־λ�ڸ�λ��ʱ����1 */
		/*��һ���ȡSR�Ĵ������ڶ���дDR�Ĵ���*/
		/* ����������������״η��͵ĵ�һ���ַ��ᶪʧ */
		USART1->SR;
		USART_SendData(USART1, (uint8_t) ch);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}