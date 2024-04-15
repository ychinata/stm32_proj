#include "PID.h"

/*λ�û�*/
float   Position_KP = 1,
		Position_KI = 0,
		Position_KD = 0;

/*�ٶȻ�*/
float   Verocity_Kp = 1,
		Verocity_Ki = 0,
		Verocity_Kd = 0.1;

/**
  * @brief  λ�û�
  * @param  ��ǰֵ������ֵ
  * @retval ������
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
  * @brief  �ٶȻ�
  * @param  ��������ȡ
  * @retval ������
  */
int Verocity(int encoder_left, int encoder_right)
{
	static int PWM_OUT, Encoder_Err, Encoder_S, EnC_Err_Lowout, EnC_Err_Lowout_Last;
	float a = 0.7;
	
	Encoder_Err = (encoder_left + encoder_right)-0;//�ٶ�ƫ��
	EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_Last;//��ͨ�˲�
	EnC_Err_Lowout_Last = EnC_Err_Lowout;
	
	Encoder_S+=EnC_Err_Lowout;//����
	
	Encoder_S = Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);//�����޷�
	
	PWM_OUT = Verocity_Kp*EnC_Err_Lowout + Verocity_Ki*Encoder_S;
	
	
	return PWM_OUT;
}

