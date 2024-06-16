#include "headfile.h"

/* 
作者：B站.我的代码没问题
硬件：TB6612+轮趣MG310电机+8路灰度传感器(未来世界机器人)
https://www.bilibili.com/video/BV1A1421671G
日期：2024.5.26
*/

int main(void)
{
	OLED_Init();
	
	motor_init();
	encoder_init();
//	motor_duty(10000);
	uart_init(UART_1,9600,0);
	
	pid_init(&motorA, DELTA_PID, 10, 10, 5);
	pid_init(&motorB, DELTA_PID, 10, 10, 5);
	
//	motor_target_set(250, 250);
	
	gray_init();
	
	tim_interrupt_ms_init(TIM_3,10,0);
    
    OLED_ShowString(1,1, "My Code");
    
	while (1)
	{
//		printf("speedA_now:%d, speedB_now:%d\r\n", (int)motorA.now, (int)motorB.now);
//		printf("%d\\r\n", (uint8_t)motorA.out);
	} 
}
