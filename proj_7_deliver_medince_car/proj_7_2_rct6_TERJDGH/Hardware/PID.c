#include "PID.h"

/*位置环*/
float   Position_KP = 1,
		Position_KI = 0,
		Position_KD = 0;

/*速度环*/
float   Verocity_Kp = 1,
		Verocity_Ki = 0,
		Verocity_Kd = 0.1;

/**
  * @brief  位置环
  * @param  当前值，期望值
  * @retval 计算结果
  */
int Position_PID(float value, float Target)
{
	static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=value-Target;                                    
	 Integral_bias+=Bias;	                               
	 Pwm=Position_KP*Bias+                                 
	     Position_KI*Integral_bias+                        
	     Position_KD*(Bias-Last_Bias);                     
	 Last_Bias=Bias;                                       
	 return Pwm;       
}
/**
  * @brief  速度环
  * @param  编码器读取
  * @retval 计算结果
  */
int Verocity(int encoder_left, int encoder_right)
{
	static int PWM_OUT, Encoder_Err, Encoder_S, EnC_Err_Lowout, EnC_Err_Lowout_Last;
	float a = 0.7;
	
	Encoder_Err = (encoder_left + encoder_right)-0;//速度偏差
	EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_Last;//低通滤波
	EnC_Err_Lowout_Last = EnC_Err_Lowout;
	
	Encoder_S+=EnC_Err_Lowout;//积分
	
	Encoder_S = Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);//积分限幅
	
	PWM_OUT = Verocity_Kp*EnC_Err_Lowout + Verocity_Ki*Encoder_S;
	
	
	return PWM_OUT;
}

