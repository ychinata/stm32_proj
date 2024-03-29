#include "math.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "contrl.h"



//需要在两个文件里改成你的编码器（.c）和pwm(.h)对应的寄存器

int   Dead_Zone=4250;    //电机死区4250 //3500会不会太小
int   control_turn=64;                             //转向控制


//PID调节参数
struct pid_arg PID = {
    .Balance_Kp=160, 	
    .Balance_Kd=0.7,	
    .Velocity_Kp=40,	
    .Velocity_Ki=0.2,	
    .Turn_Kp = 0,	
    .Turn_Kd = 0.5,	
};

// 2023.8.21调试结果
//.Balance_Kp=160,
//.Balance_Kd=0.7,
//.Velocity_Kp=40,	
//.Velocity_Ki=0.2,	
//.Turn_Kp = 0,	
//.Turn_Kd = 0.5,	

//struct pid_arg PID = {
//	.Balance_Kp=-280 ,//-230 245
//	.Balance_Kd=-0.7,//-0.16 0.50
//	.Velocity_Kp=-120,//-48 70 100
//	.Velocity_Ki=-2.9,//-0.24 0.21 0.325
//	.Turn_Kp = 70,
//	.Turn_Kd = 0.5,
//};


/**************************************************************************************************************
*函数名:Read_Encoder()
*功能:读取编码器值(当作小车当前前进的速度)
*形参:(u8 TIMX):x为编码器1或者2
*返回值:无
*************************************************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;

    switch(TIMX)
    {
    case 1:
        Encoder_TIM= (short)TIM2 -> CNT;
        TIM2 -> CNT=0;
        break;
    case 2:
        Encoder_TIM= (short)TIM4 -> CNT;
        TIM4 -> CNT=0;
        break;
    default:
        Encoder_TIM=0;
    }
    return Encoder_TIM;
}



/**************************************************************************************************************
 *功能:直立环PD控制
 *形参:(float Angle):x轴的角度/(float Gyro):x轴的角速度
 *返回值:经过PID转换之后的PWM值
 **************************************************************************************************************/
int	Vertical_Ring_PD(float Angle,float Gyro)
{
    float Bias;//目标偏差
    int balance;
    Bias=Angle-Mechanical_balance;//目标值减去机械中值（不一定为0）
    balance=PID.Balance_Kp*Bias+ Gyro*PID.Balance_Kd;

    return balance;

    //printf("balance = %f\n",balance);
}


/**************************************************************************************************************
*功能；速度环PI控制
*形参:(int encoder_left):左轮编码器值/(int encoder_right):编码器右轮的值/(float Angle):x轴角度值
*返回值:
**************************************************************************************************************/
int Vertical_speed_PI(int encoder_left,int encoder_right,float Angle,float Movement )
{
    static float Velocity,Encoder_Least,Encoder;
    static float Encoder_Integral;
    Encoder_Least =(encoder_left+encoder_right)-0;    //获取最新速度偏差=测量速度（左右编码器之和）-目标速度（此处为零）
    Encoder *= 0.8f;																	//一阶低通滤波器 ，上次的速度占85%
    Encoder += Encoder_Least*0.2f;                   //一阶低通滤波器， 本次的速度占15%
    Encoder_Integral +=Encoder;                       //积分出位移 积分时间：10ms
    Encoder_Integral=Encoder_Integral-Movement;
    //if(Movement == 0 ) Encoder_Integral=0;
    if(Encoder_Integral>10000)  	Encoder_Integral=10000;           //积分限幅
    if(Encoder_Integral<-10000)	  Encoder_Integral=-10000;            //积分限幅

    Velocity=Encoder*PID.Velocity_Kp+Encoder_Integral*PID.Velocity_Ki;      //速度控制


    if(Turn_off(Angle)==1)   
		Encoder_Integral=0;            //电机关闭后清除积分
    return Velocity;
}


/**************************************************************************************************************
*功能:转向环PD
*形参:taget_yaw 目标yaw， yaw 陀螺仪yaw ， gyro 陀螺仪yaw方向角速度
*返回值:无
***************************************************************************************************************/
int Vertical_turn_PD(float taget_yaw,float yaw,float gyro)
{
    float Turn;
    float Bias_yaw;
    Bias_yaw=taget_yaw-yaw;
		if (Bias_yaw<-180) Bias_yaw+=360;
		if (Bias_yaw>180) Bias_yaw-=360;
	
    Turn=-Bias_yaw*PID.Turn_Kp-gyro*PID.Turn_Kd;
    return Turn;
}



/**************************************************************************************************************
*函数名:PWM_Limiting()
*功能:PWM限幅函数
*形参:无
*返回值:无
***************************************************************************************************************/
void PWM_Limiting(int *motor1,int *motor2)
{
    int Amplitude=7999;//3204	// MX_TIM3_Init的周期也是7999
    if(*motor1<-Amplitude) *motor1=-Amplitude;
    if(*motor1>Amplitude)  *motor1=Amplitude;
    if(*motor2<-Amplitude) *motor2=-Amplitude;
    if(*motor2>Amplitude)  *motor2=Amplitude;
}


/**************************************************************************************************************
*函数名:Turn_off()
*功能:关闭电机
*形参:(const float Angle):x轴角度值
*返回值:1:小车当前处于停止状态/0:小车当前处于正常状态
***************************************************************************************************************/
u8 FS_state;

u8 Turn_off(const float Angle)
{
    u8 temp;
    if(fabs(Angle)>60) {
        FS_state=1;
        temp=1;
        PWMA1=0;
        PWMA2=0;
        PWMB1=0;
        PWMB2=0;
    }
    else
        temp=0;
    FS_state=0;
    return temp;
}

/**************************************************************************************************************
*函数名:Set_PWM()
*功能:输出PWM控制电机
*形参；(int motor1):电机1对应的PWM值/(int motor2):电机2对应的PWM值
*返回值:无
*************************************************************************************************************/


//void Set_PWM(int motor1,int motor2)
//{
//	if(motor1<0)	{
//		AIN2(0);
//		PWMA=Dead_Zone+(abs(motor1))*1.17;
//	}
//	else 	{
//		AIN2(1);
//	PWMA=7999-Dead_Zone-(abs(motor1))*1.17;
//	}
//	if(motor2>0)	{
//		BIN2(1);
//		PWMB=7999-Dead_Zone-(abs(motor2))*1.17;
//	}
//	else {
//		BIN2(0);
//		PWMB=Dead_Zone+(abs(motor2))*1.17;
//	}
//	printf("PWMA = %d\n",PWMA);
//	printf("PWMB = %d\n",7999-PWMB);
//}


void Set_PWM(int motor1,int motor2)
{
    if(motor1>0) {
        PWMA2=Dead_Zone+(abs(motor1))*1.17;
        PWMA1=0;
    } else {
        PWMA2 =0;
        PWMA1=Dead_Zone+(abs(motor1))*1.17;
    }

    if(motor2<0) {  // 提示：这里有坑，要把>改成<
        PWMB2 = Dead_Zone+(abs(motor2))*1.17;
        PWMB1 = 0;
    } else {   
        PWMB2 = 0;
        PWMB1=Dead_Zone+(abs(motor2))*1.17;
    }
}

