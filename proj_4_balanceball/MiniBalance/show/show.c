#include "show.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void oled_show(void)
{                         
    if(Flag_Stop==0)OLED_ShowString(00,00,"Servo O-N");
    else            OLED_ShowString(00,00,"Servo OFF");
    OLED_ShowString(80,00,"KEY");
    OLED_ShowNumber(100,00, PS2_KEY,3,12);  //PS2������
    //=============��ʾPS2��״̬=======================//	
    OLED_ShowString(00,10,"LY");
    OLED_ShowNumber(15,10, PS2_LY,3,12);  //PS2������
    OLED_ShowString(60,10,"LX");
    OLED_ShowNumber(75,10, PS2_LX,3,12);

    OLED_ShowString(00,20,"RY");
    OLED_ShowNumber(15,20, PS2_RY,3,12);  //PS2������
    OLED_ShowString(60,20,"RX");
    OLED_ShowNumber(75,20, PS2_RX,3,12);
    
    //=============��ʾPID����=======================//	
    OLED_ShowString(00,30,"KP");
    OLED_ShowNumber(15,30, Balance_Kp,3,12);  //PS2������
    OLED_ShowString(60,30,"KD");
    OLED_ShowNumber(75,30, Balance_Kd,3,12);
    
    //=============��ʾĿ��ֵ��״̬=======================//	
    OLED_ShowString(0,40,"T-");
    if( Zero_X<0)	    	OLED_ShowString(30,40,"-"),
                    OLED_ShowNumber(45,40,-Zero_X,3,12);
    else                 	OLED_ShowString(30,40,"+"),
                      OLED_ShowNumber(45,40, Zero_X,3,12); 
    //		
    if( Position_X<0)		  OLED_ShowString(80,40,"-"),
                          OLED_ShowNumber(95,40,-Position_X,3,12);
    else                 	OLED_ShowString(80,40,"+"),
                          OLED_ShowNumber(95,40, Position_X,3,12);
        //=============��ʾ���D��״̬=======================//	
                                                OLED_ShowString(0,50,"P-");
    if( Zero_Y<0)	    	  OLED_ShowString(30,50,"-"),
                          OLED_ShowNumber(45,50,-Zero_Y,3,12);
    else                 	OLED_ShowString(30,50,"+"),
                          OLED_ShowNumber(45,50, Zero_Y,3,12); 
    //		
    if( Position_Y<0)		  OLED_ShowString(80,50,"-"),
                          OLED_ShowNumber(95,50,-Position_Y,3,12);
    else                 	OLED_ShowString(80,50,"+"),
                          OLED_ShowNumber(95,50, Position_Y,3,12);
    //=============ˢ��=======================//
    OLED_Refresh_Gram();	
}

/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data(Position_X, 1 );   //��ʾ�Ƕ� ��λ���ȣ��㣩
		DataScope_Get_Channel_Data(Position_Y, 2 );   //��ʾ�����������ľ��� ��λ��CM 
// 		DataScope_Get_Channel_Data(0, 3 );   //��ʾ��ص�ѹ ��λ��V
 //		DataScope_Get_Channel_Data(0, 4 );   
//		DataScope_Get_Channel_Data(0, 5 ); //����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(2);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
