#include "control.h"

int Encoder_left, Encoder_right;

int pwm;
int pathlenth;

/*巡线PID*/

float k210_kp = 1.5,
	  k210_kd = 0;

/**
  * @brief  转向90，180
  * @param  期望，模式，pwm
  * @retval 无
  */
void car_turn(int pluse, int mode, int pwm)
{
	pathlenth = 0;
	while(1)
	{
		if(mode == 1)//左转
		{
			Load(-pwm, pwm);
		}
		else if(mode == 2)//右转
		{
			Load(pwm, -pwm);
		}
		if(pathlenth > pluse)
		{
			break;
		}
	}
	Load(0, 0);
	brake();
}
/**
  * @brief  直行
  * @param  期望距离， pwm
  * @retval 无
  */
void car_go(int distance, int pwm)
{
	pathlenth = 0;
	while(distance_limit(pathlenth, &pwm, distance) == 0)
	{
		Load(pwm, pwm);
	}
}
/**
  * @brief  距离限幅
  * @param  当前距离， pwm，目标距离
  * @retval 返回标志位
  */
int distance_limit(int now_distance, int *pwm, int target_distance)
{
	if(now_distance > target_distance)
	{
		brake();
		*pwm = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  巡线pid
  * @param  当前值，期望值
  * @retval 计算结果
  */
int PID_k210(int now, int target)
{
	static float error, PWM_OUT, last_error = 0;
	
	error = target - now;
	PWM_OUT = k210_kp*error + k210_kd*(error-last_error);
	last_error = error;
	return PWM_OUT;
}
/**
  * @brief  距离环
  * @param  目标值，pwm，模式
  * @retval 无
  */
void distance_go(int target_distance, int pwm)
{
	float pwm_line;
	pathlenth = 0;
		while(distance_limit(pathlenth, &pwm, target_distance) == 0)
		{
				pwm_line = PID_k210(coordinate, target_k210);
				Load(pwm+pwm_line, pwm-pwm_line);
		}
}
/**
  * @brief  中断执行函数5ms
  * @param  无
  * @retval 无
  */
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		Encoder_left = -Read_Speed(2);
		Encoder_right = Read_Speed(4);
		
		pathlenth += ZDS_abs(Encoder_left) + ZDS_abs(Encoder_right);
		
		
	}
}
