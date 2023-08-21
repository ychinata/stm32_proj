#ifndef _CAR_TASK_H
#define _CAR_TASK_H

struct mpu6050_data{
	short acc_x;
	short acc_y;
	short acc_z;
	
	short gyro_x;
	short gyro_y;
	short gyro_z;
	//pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
	//roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
	//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��		
	float pitch; 
	float roll;
	float yaw;
};

enum EnumCarTaskType {
	//С������ģʽ��0 ң��ģʽ��1 ����ģʽ��2 ���������棻3Ѳ��ģʽ��ûд
	ENUM_CAR_TASK_REMOTE = 0,
	ENUM_CAR_TASK_AVOID,
	ENUM_CAR_TASK_FOLLOW,
	ENUM_CAR_TASK_TRACK,
	ENUM_CAR_TASK_END
};

extern struct mpu6050_data outMpu;
extern float Movement;
extern int Contrl_Turn;

void Car_Task_200HZ(void);
void Car_Task_100HZ(void);
void Car_Task_5HZ(void);
void HC_SRC04_Start(void);
void Car_Task_Interaction(void);
#endif

