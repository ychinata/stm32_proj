#ifndef __CONTROL_H
#define __CONTROL_H

#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx.h"
#include "usart.h"
#include "tim.h"
#include "main.h"
#include  "bsp_sys.h"



/* �ۼ� TIM_Period�������һ�����»����ж�*/		
/* ����ʱ����0������PWM_PERIOD_COUNT����ΪPWM_PERIOD_COUNT+1�Σ�Ϊһ����ʱ���� */
#define PWM_PERIOD_COUNT     (1000)     //���Գ��԰�����������PID���ƿ��Ը�˳��
#define PWM2_PERIOD_COUNT     (1000)


/* ���Ƚ�ֵ */
#define PWM_MAX_PERIOD_COUNT              (PWM_PERIOD_COUNT - 30)    //���PWMŪ�������ģ�һЩ������ͻ�������⣨Ӳ���ϵ�ԭ��
#define PWM2_MAX_PERIOD_COUNT              (PWM2_PERIOD_COUNT - 30)


/****************��ߵ�����ų�ʼ��**************/
/* �����ٶȣ�ռ�ձȣ� */
#define SET_FWD_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,ChannelPulse)    // ���ñȽϼĴ�����ֵ   //AIN1    
#define SET_REV_COMPAER(ChannelPulse)     __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,ChannelPulse)    // ���ñȽϼĴ�����ֵ   //AIN2   

/* ʹ����� */
#define MOTOR_FWD_ENABLE()      HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);  
#define MOTOR_REV_ENABLE()      HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);  

/* ������� */
#define MOTOR_FWD_DISABLE()     HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
#define MOTOR_REV_DISABLE()     HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_4);



/****************�ұߵ�����ų�ʼ��**************/      //����������ת��һ�µ�ʱ��Ҫͬʱ������ĺ궨���encode.c�������������������
/* �����ٶȣ�ռ�ձȣ�2 */     
#define SET2_FWD_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,ChannelPulse)    // ���ñȽϼĴ�����ֵ   //BIN1
#define SET2_REV_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,ChannelPulse)    // ���ñȽϼĴ�����ֵ   //BIN2



/* ʹ�����2 */
#define MOTOR2_FWD_ENABLE()                  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);   
#define MOTOR2_REV_ENABLE()                  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);    


/* �������2 */
#define MOTOR2_FWD_DISABLE()                 HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4);
#define MOTOR2_REV_DISABLE()                 HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);     
    


/* ����������ֱ��� */
#define ENCODER_RESOLUTION                     11

/* ������Ƶ֮����ֱܷ��� */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4��Ƶ����ֱܷ��� */

/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO  56

/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define SPEED_PID_PERIOD  20    //���Ҫ����ʱ��7���ж�����
#define TARGET_SPEED_MAX  110  //// 60rpm����3s����60cm

#define  LookCM 32  //openmv���ﶼ���ڳ�ͷ�����������31cm��ȥʶ�����ֵ�

#define HideCM 30

#define LUN_JU 17.5   //��λcm
#define HeadToWheelCM 13
	

typedef enum
{
  left_90,
	right_90,
	back_180
}spin_dir_t;



/* ����������ö�� */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

extern unsigned char g_ucMainEventCount;
extern unsigned char g_ucMainEventCountLong;
//extern unsigned char TASK_NUM;
extern uint8_t is_motor_en;

extern int32_t  actual_speed;

extern float g_fJourney;   //���С������������·�̺� �� ��λcm, ��Ҫ��ʱ����
extern float g_fTargetJourney;  //���С������������·�̺� �� ��λcm����Ҫ����һ�׶�����������

extern u8 Line_flag , Turn_flag;
extern  u8  Spin_succeed_flag , Stop_Flag;

extern u8  WaitFlag;
extern u16 WaitTimeCount;

//extern u8 LineNumToggleFlag;


void set_motor_speed(uint16_t v);
void set_motor_speed(uint16_t v);
void set_motor_direction(motor_dir_t dir);
void set_motor_enable(void);
void set_motor_disable(void);
void set_motor2_speed(uint16_t v);
void set_motor2_direction(motor_dir_t dir);
void set_motor2_enable(void);
void set_motor2_disable(void);
void show_help(void);
void deal_serial_data(void);

float speed_pid_control(void);
float location_pid_control(void);
float speed2_pid_control(void);
float location2_pid_control(void);
void Location_Speed_control(void);



void Car_go(int32_t location_cm);

void spin_Turn(spin_dir_t zhuanxiang);
void slow_GoForward(void);

void MotorOutput(int nMotorPwm,int nMotor2Pwm);




#endif 

