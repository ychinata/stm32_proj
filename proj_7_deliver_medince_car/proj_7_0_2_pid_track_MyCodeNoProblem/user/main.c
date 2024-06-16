#include "headfile.h"

/* 
���ߣ�Bվ.�ҵĴ���û����
Ӳ����TB6612+��ȤMG310���+8·�Ҷȴ�����(δ�����������)
https://www.bilibili.com/video/BV1A1421671G
���ڣ�2024.5.26
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
