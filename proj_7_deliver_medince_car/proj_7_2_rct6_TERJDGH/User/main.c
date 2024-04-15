/*
	项目名称：智能送药小车
	项目作者：ZDS
	项目日期：2024年3月
*/


#include "sys.h"  


uint8_t flag = 0;
uint8_t num[6] = {0};
uint8_t i = 0;

void contorl_hanshu(void);
int main(void)
{
	NVIC_PriorityGroupConfig(2);//中断优先级分组
	delay_init();
	OLED_Init();
	Serial_Init();
	USART2_Init();
	Zigbee_Init();
	Motor_Init();
	LED_Init();
	PWM_Init_TIM1(0, 7200-1);
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Timer_Init();
	Servo_Init();
	
	light_Init();
	
	OLED_ShowString(1,1,"X:");
	OLED_ShowString(2,1,"Num:");
	
	Servo_SetAngle(160);//舵机回正
	while(coordinate == 0);//等到识别数字
	while(k210_num == 0);//等待巡到红线
	OLED_ShowNum(2, 5, k210_num, 1);
	while(wait_pha_ok() == 0);//等待药品放下
	
	TIM_Cmd(TIM5, ENABLE);
	while (1)
	{
		OLED_ShowSignedNum(1, 5, coordinate, 3);
		contorl_hanshu();
	}
}


/**
  * @brief  送药控制函数
  * @param  无
  * @retval 无
  */
void contorl_hanshu(void)
{
	/*近端*/
		if(k210_num == 1)
		{
			if(flag == 0)
			{
				distance_go(6400, 2000);//巡线直行
//				car_go(1600, 1000);//开环直行
				flag = 1;
				delay_ms(200);
			}
			else if(flag == 1)
			{
				car_turn(1150, 1, 2000);//左转90°
				delay_ms(200);
				distance_go(4200, 2000);
				delay_ms(200);
				LED1_ON();
				while(wait_pha_ok() == 1);//等待药品拿下
				delay_ms(200);
				LED1_OFF();
				flag = 2;
			}
			else if(flag == 2)
			{
				car_turn(2500, 1, 2000);//旋转180°
				delay_ms(200);
				distance_go(4200, 2000);
				delay_ms(200);
				flag = 3;
			}
			else if(flag == 3)
			{
				car_turn(1100, 2, 2000);
				delay_ms(200);
				distance_go(6100, 2000);
				delay_ms(200);
				flag = 4;
			}
		}	
		
		
		
		else if(k210_num == 2)
		{
			if(flag == 0)
			{
				distance_go(6400, 2000);
				flag = 1;
				delay_ms(1000);
			}
			else if(flag == 1)
			{
				car_turn(1100, 2, 2000);//
				delay_ms(500);
				distance_go(4200, 2000);
				delay_ms(200);
				LED1_ON();
				while(wait_pha_ok() == 1);//等待药品拿下
				delay_ms(200);
				LED1_OFF();
				flag = 2;
			}
			else if(flag == 2)
			{
				car_turn(2400, 1, 2000);//旋转180°
				delay_ms(500);
				distance_go(4200, 2000);
				delay_ms(500);
				flag = 3;
			}
			else if(flag == 3)
			{
				car_turn(1200, 1, 2000);
				delay_ms(500);
				distance_go(6100, 2000);
				delay_ms(500);
				flag = 4;
			}
		}
		
		/*中端*/
		else if(k210_num != 1 && k210_num != 2)
		{
			if(flag == 0)
			{
				distance_go(11500, 2000);//前进到十字路口
				delay_ms(1000);
				num[0] = k210_Rx;//接收当前数字
				flag = 1;
			}
			else if(flag == 1)
			{
				if(k210_num == num[0])//数字在左
				{
					distance_go(3000, 2000);
					delay_ms(500);
					car_turn(1200, 1, 2000);
					
					delay_ms(500);
					distance_go(4200, 2000);
					delay_ms(500);
					LED1_ON();
					Servo_SetAngle(160);
					while(wait_pha_ok() == 1);
					delay_ms(500);
					LED1_OFF();
					car_turn(2400, 1, 2000);//旋转180°
					distance_go(4200, 2000);
					car_turn(1250, 2, 2000);
//					for(i=0; i<5; i++)
					Zigbee_SendByte(0x01);
					
					delay_ms(500);
					distance_go(14500, 2000);
					Load(0, 0);
					delay_ms(2000);
					flag = 2;
				}
				else if(k210_num != num[0])
				{
					Servo_SetAngle(130);
					Load(0,0);
					delay_ms(1500);
					num[1] = k210_Rx;
					delay_ms(500);
					if(k210_num == num[1])
					{
						distance_go(2930, 2000);
						delay_ms(500);
						car_turn(1200, 2, 2000);
						delay_ms(500);
						distance_go(4200, 2000);
						delay_ms(500);
						LED1_ON();
						while(wait_pha_ok() == 1);
						delay_ms(500);
						LED1_OFF();
						car_turn(2400, 1, 2000);//旋转180°
						delay_ms(500);
						distance_go(4200, 2000);
						delay_ms(500);
						car_turn(1200, 1, 2000);
						Zigbee_SendByte(0x02);
						delay_ms(500);
						Servo_SetAngle(160);
						distance_go(14500, 2000);
						Load(0, 0);
						delay_ms(2000);
						flag = 2;
					}
					
					
					
					/*远端*/
					else 
					{
							distance_go(10550, 2000);//前进到十字路口
							delay_ms(500);
							car_turn(1100, 1, 2000);
							delay_ms(500);
							distance_go(4700, 2000);
							delay_ms(800);
							num[2] = k210_Rx;//接收当前数字
							Servo_SetAngle(160);
							Load(0, 0);
							delay_ms(1500);
							num[3] = k210_Rx;
							
							if(k210_num == num[3])
							{
								distance_go(2800, 2000);
								delay_ms(500);
								car_turn(1150, 1, 2000);
								delay_ms(500);
								distance_go(4200, 2000);
								delay_ms(500);
								LED1_ON();
								while(wait_pha_ok() == 1);
								Zigbee_SendByte(0x02);
								delay_ms(500);
								LED1_OFF();	
								car_turn(2300, 1, 2000);//旋转180°
								delay_ms(500);
								distance_go(4200, 1500);
								delay_ms(500);
								car_turn(1150, 2, 2000);
								delay_ms(500);
								distance_go(7400, 1500);
								delay_ms(500);
								car_turn(1150, 2, 2000);
								delay_ms(500);
								distance_go(22000, 2000);
								delay_ms(2000);
								flag = 3;
							}
							if(k210_num == num[2])
							{
								distance_go(2700, 2000);
								delay_ms(500);
								car_turn(1150, 2, 2000);
								delay_ms(500);
								distance_go(4200, 2000);			
								delay_ms(500);
								LED1_ON();
								while(wait_pha_ok() == 1);
								Zigbee_SendByte(0x02);
								delay_ms(500);
								LED1_OFF();
								car_turn(2300, 1, 2000);//旋转180°
								delay_ms(500);
								distance_go(4200, 1500);
								delay_ms(500);
								car_turn(1150, 1, 2000);
								delay_ms(500);
								distance_go(7700, 1500);
								delay_ms(500);
								car_turn(1150, 2, 2000);
								delay_ms(500);
								distance_go(22000, 2000);
								delay_ms(2000);
								flag = 3;
						}	
					}
					if(k210_num != num[3] && k210_num != num[2])
					{
						car_turn(2400, 1, 2000);//旋转180°
						delay_ms(800);
						distance_go(9400, 2000);
						delay_ms(500);
						num[4] = k210_Rx;
						Servo_SetAngle(130);
						Load(0, 0);
						delay_ms(1500);						
						num[5] = k210_Rx;
						delay_ms(500);
						if(k210_num == num[4])
						{
							distance_go(2700, 2000);
							delay_ms(500);
							car_turn(1150, 1, 2000);
							delay_ms(500);
							distance_go(4200, 2000);							
							delay_ms(500);
							LED1_ON();
							while(wait_pha_ok() == 1);
							Zigbee_SendByte(0x01);
							delay_ms(500);
							LED1_OFF();
							car_turn(2300, 1, 2000);//旋转180°
							delay_ms(500);
							distance_go(4200, 1500);
							delay_ms(500);
							car_turn(2400, 2, 2000);
							delay_ms(500);
							distance_go(7800, 1500);
							delay_ms(500);
							car_turn(1150, 1, 2000);
							delay_ms(500);
							distance_go(22000, 2000);
							delay_ms(2000);
							flag = 3;
						}
						if(k210_num == num[5])
						{
							distance_go(2700, 2000);
							delay_ms(500);
							car_turn(1150, 2, 2000);
							delay_ms(500);
							distance_go(4200, 2000);
							delay_ms(500);
							LED1_ON();						
							while(wait_pha_ok() == 1);
							Zigbee_SendByte(0x01);
							delay_ms(500);
							LED1_OFF();
							car_turn(2300, 1, 2000);//旋转180°
							delay_ms(500);
							distance_go(4200, 1500);
							delay_ms(500);
							car_turn(1150, 1, 2000);
							delay_ms(500);
							distance_go(7800, 1500);
							delay_ms(500);
							car_turn(1150, 1, 2000);
							delay_ms(500);
							distance_go(22000, 2000);
							delay_ms(2000);
							flag = 3;
						}
					}
					
				}
								
				}
				}
			}


