/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "menu.h"
#include <stdio.h>
#include "bsp_key.h"
#include "control.h"
#include "oled.h"
#include "encoder.h"
#include "tim.h"
#include "openmv.h"
#include "bsp_GraySensor.h"
#include "bsp_BlueTooth.h"

extern unsigned char BMP1[];   //���õ�ͼƬ��ҳ
//extern unsigned char BMP4[];  //HCF  //��Ҫ���
extern unsigned char BMP3[];  //START

//OLED�˵����ĸ����ݻ������͹��˰�
u8 str_buff1[64];
u8 str_buff2[64];
u8 str_buff3[64];
u8 str_buff4[64];   
u8 str_buff5[64];
u8 str_buff6[64];

u8 Menu_Item = 132;  //�����ʾһ��ʼ����ת�ĸ�����


extern uint16_t ADC_Light[10];

void OLED_Display(u8 num)
{
	switch(num)      //������Ҫ��ʾ������
	{
		case 0:
	    OLED_DrawBMP(0, 0, 128, 8, BMP3);  
		break;
		
		
		//ѡ��һ�������һ��ѡ��
		case 1:
			OLED_ShowString(40, 0, "MENU", 16);
		  OLED_ShowString(20, 3, "MPU6050", 8);
		  OLED_ShowString(20, 4, "TASK", 8);
		  OLED_ShowString(20, 5, "TEST", 8);
		  OLED_ShowString(0, 3, ">>", 8);
		break;
		
		//��һ��ѡ���������
		case 10:
		 OLED_ShowString(0, 0, "MPU6050 :", 16);
		 OLED_ShowString(20, 3, "Gyroscope", 8);
		 OLED_ShowString(20, 4, "Accelerometer", 8);
		 OLED_ShowString(20, 5, "Euler angle", 8);
		 OLED_ShowString(0, 3, ">>", 8);
		 break;
		
		 case 11:
		  OLED_ShowString(0, 0, "MPU6050 :", 16);
		 OLED_ShowString(20, 3, "Gyroscope", 8);
		 OLED_ShowString(20, 4, "Accelerometer", 8);
		 OLED_ShowString(20, 5, "Euler angle", 8);
		  OLED_ShowString(0, 4, ">>", 8);
		 break;
		 
		 case 12:
		  OLED_ShowString(0, 0, "MPU6050 :", 16);
		 OLED_ShowString(20, 3, "Gyroscope", 8);
		 OLED_ShowString(20, 4, "Accelerometer", 8);
		 OLED_ShowString(20, 5, "Euler angle", 8);
		  OLED_ShowString(0, 5, ">>", 8);
		 break;
		 
		 //��һ��ѡ����������
			case 110:
				OLED_ShowString(0, 0, "Gyroscope ", 8);
		
//				sprintf((char *)str_buff1, "gyrox: %6d", gyrox);
//				sprintf((char *)str_buff2, "gyroy: %6d", gyroy);
//				sprintf((char *)str_buff3, "gyroz: %6d", gyroz);
//				
//				OLED_ShowString(10, 3,str_buff1, 8);
//				OLED_ShowString(10, 4,str_buff2, 8);
//				OLED_ShowString(10, 5,str_buff3, 8);		
			break;
				
			case 111 :
				OLED_ShowString(0, 0, "Accelerometer", 8);
				
				
//				sprintf((char *)str_buff1, "Accx: %6d", accx);
//				sprintf((char *)str_buff2, "Accy: %6d", accy);
//				sprintf((char *)str_buff3, "Accz: %6d", accz);
//				
//				OLED_ShowString(10, 3,str_buff1, 8);
//				OLED_ShowString(10, 4,str_buff2, 8);
//				OLED_ShowString(10, 5,str_buff3, 8);			
			break;
			
			case 112:
					OLED_ShowString(0, 0, "Euler Angle", 8);
	
//					sprintf((char *)str_buff1, "pitch:%4.2f", pitch);
//					sprintf((char *)str_buff2, "roll: %4.2f", roll);
//					sprintf((char *)str_buff3, "yaw:  %4.2f", yaw);  
//					
//					OLED_ShowString(0, 3,str_buff1, 8);
//					OLED_ShowString(0, 4,str_buff2, 8);
//					OLED_ShowString(0, 5,str_buff3, 8);
			break;
		
		
		//�ڶ���ѡ��
		case 2:                                  //�ǵ���ǰ�����˵����汣��һֱ
			OLED_ShowString(40, 0, "MENU", 16);
		  OLED_ShowString(20, 3, "MPU6050", 8);
		  OLED_ShowString(20, 4, "TASK", 8);
		  OLED_ShowString(20, 5, "TEST", 8);
		  OLED_ShowString(0, 4, ">>", 8);
		  break;
		
		//�ڶ���ѡ���������
		case 20:
		 {
			  OLED_ShowString(0, 0, "TASK", 16);
		    OLED_ShowString(20, 3, "", 8);
		    OLED_ShowString(20, 4, "", 8);
		    OLED_ShowString(20, 5, "", 8);
			  OLED_ShowString(20, 6, "", 8);
		    OLED_ShowString(0, 3, ">>", 8);
		 }
		 break;
		 
		 case 21:
		 {
			  OLED_ShowString(0, 0, "TASK", 16);
		    OLED_ShowString(20, 3, "", 8);
		    OLED_ShowString(20, 4, "", 8);
		    OLED_ShowString(20, 5, "", 8);
			  OLED_ShowString(20, 6, "", 8);
		    OLED_ShowString(0, 4, ">>", 8);
		 }
		 break;
		 
		 case 22:
		 {
			  OLED_ShowString(0, 0, "TASK", 16);
		    OLED_ShowString(20, 3, "", 8);
		    OLED_ShowString(20, 4, "", 8);
		    OLED_ShowString(20, 5, "", 8);
			  OLED_ShowString(20, 6, "", 8);
		    OLED_ShowString(0, 5, ">>", 8);
		 }
		 break;
		 
		  case 23:
		 {
			  OLED_ShowString(0, 0, "TASK", 16);
		    OLED_ShowString(20, 3, "", 8);
		    OLED_ShowString(20, 4, "", 8);
		    OLED_ShowString(20, 5, "", 8);
			  OLED_ShowString(20, 6, "", 8);
		    OLED_ShowString(0, 6, ">>", 8);
		 }
		 break;
		 
		 //�ڶ���ѡ����������    //����������ʾ������һ��������װ������ �Ժ���ò�ͬ�����õ���
		 case 120:
				OLED_ShowString(0, 0, "TASK 1 :", 16);
			 OLED_ShowString(20, 4, "", 16);              
			break;
				
			case 121 :
					OLED_ShowString(0, 0, "TASK 2 :", 16);
					OLED_ShowString(20, 4, "", 8);
			break;
			
			case 122:
					OLED_ShowString(0, 0, "TASK 3 :", 16);
					OLED_ShowString(20, 4, "", 16);
			break;
			
			case 123:
			  	OLED_ShowString(0, 0, "TASK ALL :", 16);
					OLED_ShowString(20, 4, "", 16);
		  break;
		
		//������ѡ��
		case 3:
			OLED_ShowString(40, 0, "MENU", 16);   //�ǵ���ǰ�����˵����汣��һֱ
		  OLED_ShowString(20, 3, "MPU6050", 8);
		  OLED_ShowString(20, 4, "TASK", 8);
		  OLED_ShowString(20, 5, "TEST", 8);
		  OLED_ShowString(0, 5, ">>", 8);
		  break;
		
		//������ѡ���������
		case 30:
		 {
			  OLED_ShowString(0, 0, "TEST", 16);
		    OLED_ShowString(20, 3, "motor", 8);
		    OLED_ShowString(20, 4, "ADC", 8);
		    OLED_ShowString(20, 5, "openmv", 8);
			  OLED_ShowString(20, 6, "BlueTooth", 8);
		    OLED_ShowString(0, 3, ">>", 8);
		 }
		 break;
		 
		 case 31:
		 {
			  OLED_ShowString(0, 0, "TEST", 16);
		    OLED_ShowString(20, 3, "motor", 8);
		    OLED_ShowString(20, 4, "ADC", 8);
		    OLED_ShowString(20, 5, "openmv", 8);
			  OLED_ShowString(20, 6, "BlueTooth", 8);
		    OLED_ShowString(0, 4, ">>", 8);
		 }
		 break;
		 
		 case 32:
		 {
			  OLED_ShowString(0, 0, "TEST", 16);
		    OLED_ShowString(20, 3, "motor", 8);
		    OLED_ShowString(20, 4, "ADC", 8);
		    OLED_ShowString(20, 5, "openmv", 8);
			  OLED_ShowString(20, 6, "BlueTooth", 8);
		    OLED_ShowString(0, 5, ">>", 8);
		 }
		 break;
		 
		 case 33:
		 {
			  OLED_ShowString(0, 0, "TEST", 16);
		    OLED_ShowString(20, 3, "motor", 8);
		    OLED_ShowString(20, 4, "ADC", 8);
		    OLED_ShowString(20, 5, "openmv", 8);
			  OLED_ShowString(20, 6, "BlueTooth", 8);
		    OLED_ShowString(0, 6, ">>", 8);
		 }
		 break;
		 
		 //������ѡ����������
		 case 130:
			 
		 /*********���Ա������Ƿ�����**************/

		 
   		  OLED_ShowString(0, 0, "test 1:", 16);   //��test 1 ������Ի�ȡ����������ֵ   //����ڶ�����ʾÿ��ת����Ȧ��
		    sprintf((char *)str_buff1, "M_EncNum: %5d", g_nMotorPulse);            //�����ֵȡ�������GetMotorPulse������  ��50ms���� �����ҲҪ��Ӧ1000/25=40,����Ҫ����40
				sprintf((char *)str_buff2, "rps:      %2.2f", g_nMotorPulse*(1000/SPEED_PID_PERIOD)*0.25/56/11);    //����  �������½��ض����� ���ٱ�1��56   ���������������11  
				sprintf((char *)str_buff3, "M2_EncNum:%5d", g_nMotor2Pulse);  
				sprintf((char *)str_buff4, "rps:      %2.2f", g_nMotor2Pulse*(1000/SPEED_PID_PERIOD)*0.25/56/11);
		 
				OLED_ShowString(0, 3,str_buff1, 8);
				OLED_ShowString(0, 4,str_buff2, 8);
				OLED_ShowString(0, 5,str_buff3, 8);
		    OLED_ShowString(0, 6,str_buff4, 8);		

			break;
		 
		 case 131:
				
// 		  OLED_ShowString(0, 0, "test 2:", 16);   //��test 1 ������Ի�ȡ����������ֵ   //����ڶ�����ʾÿ��ת����Ȧ��
//		  sprintf((char *)str_buff1, "ADC_Volt: %2.2f", ADC_Volt );         
//    	sprintf((char *)str_buff3, "Load_flag:%d", Load_flag);  

//			OLED_ShowString(0, 3,str_buff1, 8);
//      OLED_ShowString(0, 6,str_buff3, 8);
		   
       sprintf((char *)str_buff1, "L2_Val: %d", L2_Val);   //Ϊʲô��һλ����0��
		   sprintf((char *)str_buff2, "L1_Val: %d", L1_Val);    //����33����Ϊ�ҽ�����sprintfת��Ϊ��ʮ���Ƶ���
		   sprintf((char *)str_buff3, "M_Val: %d",  M_Val); 
		   sprintf((char *)str_buff4, "R1_Val: %d", R1_Val);
		   sprintf((char *)str_buff5, "R2_Val: %d", R2_Val);
		 
			  OLED_ShowString(0, 0, "GraySensor:", 16);
		    OLED_ShowString(0, 2,str_buff1, 8);
				OLED_ShowString(0, 3,str_buff2, 8);
				OLED_ShowString(0, 4,str_buff3, 8);
		    OLED_ShowString(0, 5,str_buff4, 8);
        OLED_ShowString(0, 6,str_buff5, 8);		 
		 
		 break;
		 
		 case 132:
				sprintf((char *)str_buff1, "TargetRoom: %c", TargetRoom);
				sprintf((char *)str_buff2, "OneTargetRoom: %c",OneTargetRoom );
				sprintf((char *)str_buff3, "LoR: %d", LoR); 
				sprintf((char *)str_buff4, "Num: %d", Num);
				sprintf((char *)str_buff5, "FindTask: %d", FindTask);
				sprintf((char *)str_buff6, "Load_flag:%d", Load_flag);
          		 
		    OLED_ShowString(0, 0, "openmv:", 16);
				OLED_ShowString(0, 2,str_buff1, 8);
				OLED_ShowString(0, 3,str_buff2, 8);
				OLED_ShowString(0, 4,str_buff3, 8);
		    OLED_ShowString(0, 5,str_buff4, 8);
        OLED_ShowString(0, 6,str_buff5, 8);	
				OLED_ShowString(0, 7,str_buff6, 8);		

		 
			break;
		 
		 case 133:
				sprintf((char *)str_buff1, "%d", BlueRxBuffer[0]);   //Ϊʲô��һλ����0��
				sprintf((char *)str_buff2, "%d", BlueRxBuffer[1]);    //����33����Ϊ�ҽ�����sprintfת��Ϊ��ʮ���Ƶ���
				sprintf((char *)str_buff3, "%d", BlueRxBuffer[2]); 
				sprintf((char *)str_buff4, "%d", BlueRxBuffer[3]);
				sprintf((char *)str_buff5, "%d", BlueRxBuffer[4]);
		 
			  OLED_ShowString(0, 0, "BlueTooth:", 16);
		    OLED_ShowString(0, 2,str_buff1, 8);
				OLED_ShowString(0, 3,str_buff2, 8);
				OLED_ShowString(0, 4,str_buff3, 8);
		    OLED_ShowString(0, 5,str_buff4, 8);
        OLED_ShowString(0, 6,str_buff5, 8);
		   
			break;
			
		}
}


void MENU_Item_KEY(void)              //�������²Ÿ������������Դ����߼�Ҫ��������
{
			switch(g_nButton)
			{
				
				//ѡ��������μ� �� ���������������һ��ѡ��󷵻ص�һ��ѡ����������һ������ȥ����
				case KEY1_PRES:     
			   	OLED_Clear();
				  Menu_Item++;
				
				     //�����ǿ���Menu_Item��switch
			    	switch(Menu_Item) 
					{
						case 4: 
							Menu_Item  = 1; 
						break;
						
						//��һ��ѡһ������
						case 13:
							Menu_Item = 10;
						break;
						
						   // ��һ��ѡ���������
					  case 111:
						case 112:
						case 113:   //���112�������л�
						 Menu_Item--;
						break;	
						
						
							//�ڶ���ѡһ������
						case 24:
							Menu_Item = 20;
						break;
						
						// �ڶ���ѡ���������
						case 121:
						case 122:
						case 123: 
						case 124:							//���124 �������л�
						 Menu_Item--;
						break;
						
						//������ѡһ������
						case 34:
							Menu_Item = 30;
						break;
						
						// ������ѡ���������
						case 131:
						case 132:
						case 133:
						case 134:    //���133�������л�
						 Menu_Item--;
						break;
					}	
 					
				  break;
				
				case KEY2_PRES:     //ȷ����
						OLED_Clear();
					switch(Menu_Item)
					{
						case 1: 
						case 2:
					  case 3:
							Menu_Item *= 10;   //�����Գ�10������ʵ��ȷ�ϵ�Ч��
						break;
						
						//���漸����Ȼ���Զ�д��һ�� ����������������ˣ����׸�
						case 10:      //��һ��ѡ��
						case 11:	
						case 12:
							Menu_Item  += 100;   //�����Խ���λ������ʵ��ȷ�ϵ�Ч��
						break;
						
						case 20:      //�ڶ���ѡ��
						case 21:	
						case 22:
						case 23:
							Menu_Item  += 100;   //�����Խ���λ������ʵ��ȷ�ϵ�Ч��
						break;
						
						case 30:      //������ѡ��
						case 31:	
						case 32:
						case 33:
							Menu_Item  += 100;   //�����Խ���λ������ʵ��ȷ�ϵ�Ч��
						break;
					}					
				break;
				
					
					//�������˵�����״̬����
				case KEY3_PRES :              //home�� 
					 Menu_Item = 0;    
//				  PWM_update(0,0,0,0);      //����״̬�ָ�Ĭ��
				
				 break;
			}
}
