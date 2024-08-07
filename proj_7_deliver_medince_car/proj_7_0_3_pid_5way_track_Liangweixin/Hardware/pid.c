#include "stm32f10x.h"      
#include "pid.h"
#include "gray_track.h" 
#include "motor.h"
#include "serial.h"

int16_t left_count = 0;     //编码器计算值
int16_t right_count = 0;

pid_t pid_motor_left;           //左电机 PID结构体
pid_t pid_motor_right;          //右电机 PID结构体

/*  
    初始化PID参数
*/
void pid_init(pid_t *pid,uint8_t mode,float p,float i,float d){
    pid->pid_mode = mode;       //设置模式
    pid->p = p;                 
    pid->i = i;
    pid->d = d;
    motor_target_set(0,0);
} 


/*  
    PID控制函数
*/
void pid_contorl(void){
    //1、根据寻迹模块 设置目标值
    TRACK_ControlPID();
    //2、获取当前速度
    pid_motor_left.now = left_count;
    pid_motor_right.now = right_count;
    //3、进行PID计算
    pid_cal(&pid_motor_left);
    pid_cal(&pid_motor_right);
    //4、将PID输出值设置为PWM
    Motor_Left_SetSpeed(pid_motor_left.out);
    Motor_Right_SetSpeed(pid_motor_right.out);
    datavision_send();
} 





/*  
    PID计算函数
*/
void pid_cal(pid_t *pid)
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];  
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// 输出限幅
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=0)	
		pid->out=0;
	
}


/* 
    设置左右轮PID目标值
*/
void motor_target_set(int left_spe1, int right_spe2)
{
	if(left_spe1 >= 0)
	{
        Motor_Left_Fwd();
		pid_motor_left.target = left_spe1;
        
	}
	else
	{
        Motor_Left_Rev();
		pid_motor_left.target = -left_spe1;
	}
	
	if(right_spe2 >= 0)
	{
        Motor_Right_Fwd();
		pid_motor_right.target = right_spe2;
	}
	else
	{
        Motor_Right_Rev();
		pid_motor_right.target = -right_spe2;
	}
}

void datavision_send()  // 上位机波形发送函数
{
	// 数据包头      如果使用山外多功能调试助手 不可以修改包头和包尾
	Serial_SendByte(0x03);
	Serial_SendByte(0xfc);

	// 发送数据   如果使用山外多功能调试助手 需要注意顺序
    Serial_SendByte((uint8_t)pid_motor_right.now);          //1、右电机 当前值
	Serial_SendByte((uint8_t)pid_motor_right.target);       //2、右电机 目标值
    
    Serial_SendByte((uint8_t)pid_motor_left.now);           //3、左电机 当前值
	Serial_SendByte((uint8_t)pid_motor_left.target);        //4、左电机 目标值
	
	// 数据包尾
	Serial_SendByte(0xfc);
	Serial_SendByte(0x03);
}

