#include "show.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{                         
    if(Flag_Stop==0)OLED_ShowString(00,00,"Servo O-N");
    else            OLED_ShowString(00,00,"Servo OFF");
    OLED_ShowString(80,00,"KEY");
    OLED_ShowNumber(100,00, PS2_KEY,3,12);  //PS2的数据
    //=============显示PS2的状态=======================//	
    OLED_ShowString(00,10,"LY");
    OLED_ShowNumber(15,10, PS2_LY,3,12);  //PS2的数据
    OLED_ShowString(60,10,"LX");
    OLED_ShowNumber(75,10, PS2_LX,3,12);

    OLED_ShowString(00,20,"RY");
    OLED_ShowNumber(15,20, PS2_RY,3,12);  //PS2的数据
    OLED_ShowString(60,20,"RX");
    OLED_ShowNumber(75,20, PS2_RX,3,12);
    
    //=============显示PID参数=======================//	
    OLED_ShowString(00,30,"KP");
    OLED_ShowNumber(15,30, Balance_Kp,3,12);  //PS2的数据
    OLED_ShowString(60,30,"KD");
    OLED_ShowNumber(75,30, Balance_Kd,3,12);
    
    //=============显示目标值的状态=======================//	
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
        //=============显示电机D的状态=======================//	
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
    //=============刷新=======================//
    OLED_Refresh_Gram();	
}

/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
		DataScope_Get_Channel_Data(Position_X, 1 );   //显示角度 单位：度（°）
		DataScope_Get_Channel_Data(Position_Y, 2 );   //显示超声波测量的距离 单位：CM 
// 		DataScope_Get_Channel_Data(0, 3 );   //显示电池电压 单位：V
 //		DataScope_Get_Channel_Data(0, 4 );   
//		DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
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
