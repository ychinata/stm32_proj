#ifndef __PID_h_
#define __PID_h_
#include "stm32f10x.h"                  // Device header

#define MAX_DUTY 1000
enum
{
  POSITION_PID = 0,  // 位置式
  DELTA_PID,         // 增量式
};

typedef struct
{
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;   
	
	uint32_t pid_mode;

}pid_t;

void pid_cal(pid_t *pid);
void motor_target_set(int left_spe1, int right_spe2);
void pid_init(pid_t *pid,uint8_t mode,float p,float i,float d);
void pid_contorl(void);

extern int16_t left_count;     
extern int16_t right_count;
extern pid_t pid_motor_left;           //左电机 PID结构体
extern pid_t pid_motor_right;          //右电机 PID结构体

void datavision_send(void);  // 上位机波形发送函数
#endif
