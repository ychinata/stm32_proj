#include "stm32f10x.h"
#include "sys.h"

  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8  Flag_Stop=1;                 //停止标志位和 显示标志位 默认停止 显示打开
int Position_X,Position_Y;
float Balance_Kp=55,Balance_Ki=0.000,Balance_Kd=58,Data_Show,Data_Show2;
float Zero_X=133,Zero_Y=100,Target_X,Target_Y;
u8 delay_50,delay_flag;         //  50ms精准演示标志位
int PS2_LX=128,PS2_LY=128,PS2_RX=128,PS2_RY=128,PS2_KEY;     //PS2遥控相关

int main(void)
{ 
    delay_init();	    	            //=====延时函数初始化	
    uart_init(128000);	            //=====串口初始化为
    JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
    LED_Init();                     //=====初始化与 LED 连接的硬件接口
    KEY_Init();                     //=====按键初始化
    MY_NVIC_PriorityGroupConfig(2); //=====中断分组
    delay_ms(100);                  //=====延时等待
    //	uart3_init(9600);               //=====串口3初始化
    Adc_Init();                     //=====adc初始化
    delay_ms(100);                  //=====延时等待
    OLED_Init();                    //=====OLED初始化	
    TIM1_Int_Init(99,7199);         //=====定时器10ms中断
    TIM4_PWM_Init(9999,143);        //=====PWM初始化
    Screen_Init();
    PS2_Init();                     //=====PS2手柄初始化
    PS2_SetInit();		 							//=====ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
    
    while(1) {
        PS2_KEY=PS2_DataKey();	         //采集PS2按键
        PS2_LX=PS2_AnologData(PSS_LX);    //采集PS2摇杆
        PS2_LY=PS2_AnologData(PSS_LY);
        PS2_RX=PS2_AnologData(PSS_RX);
        PS2_RY=PS2_AnologData(PSS_RY);

        oled_show();          //===显示屏打开
        DataScope();          //开启MiniBalance上位机
        delay_flag=1;	
        delay_50=0;
        while(delay_flag);	     //通过MPU6050的INT中断实现的50ms精准延时				
    } 
}

