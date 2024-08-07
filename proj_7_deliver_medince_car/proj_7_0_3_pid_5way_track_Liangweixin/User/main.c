#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Encoder.h"
#include "motor.h"
#include "pwm.h"
#include "pid.h"
#include "gray_track.h"
#include "serial.h"

//待封装函数
extern uint8_t D1,D2,D3;

int main(void)
{
	OLED_Init();                 //OLED初始化
	Timer_Init(200,7200);        //定时3器中断   设置重装载值和预分频系数 此处溢出时间为20ms 
	Encoder_Init();              //定时器2、4 编码器初始化
	PWM_Init(1000,7);            //定时器1  PWM初始化    设置重装载值和预分频系数 此处PWM频率为10.285kHZ 
	Motor_Init();
	LED_Init();
    TRACK_Init();               // 灰度巡迹
    
    // 
    pid_init(&pid_motor_right,POSITION_PID,10,1.5,0);    //PID初始化
    pid_init(&pid_motor_left,POSITION_PID,10,1.5,0);
    Serial_Init();
//    SysTick_Config(SystemCoreClock / 1000);    /* 初始化SysTick定时器中断 1ms*/
    OLED_ShowString(1,1,"LeftPwm:");         
    OLED_ShowString(2,1,"RightPwm:");
	OLED_ShowString(3,1,"Track:");
	
	while (1) {
        OLED_ShowSignedNum(1,10,left_count,4);
        OLED_ShowSignedNum(2,10,right_count,4);
		OLED_ShowNum(3,7,D1,1);
		OLED_ShowNum(3,8,D2,1);
		OLED_ShowNum(3,9,D3,1);
	}
}

/* 
    定时器4中断服务函数 20ms进行一次PID计算
*/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {        
         //单位时间20ms 编码器捕获的值
        right_count = Encoder_Right_Get();     
        left_count  = -Encoder_Left_Get();    
        pid_contorl();     
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

