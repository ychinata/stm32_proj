/**
  *************************************************************************************************************************
  * @file    motor.h
  * @author  amkl
  * @version V1.0
  * @date    2022-09-22
  * @brief   xx模块.h文件配置
  *************************************************************************************************************************
  * @attention
  * 
  * 
  * 
  *************************************************************************************************************************
  */


/* Define to prevent recursive inclusion --------------------------------------------------------------------------------*/
#ifndef _MOTOR_H
#define	_MOTOR_H

/* Includes -------------------------------------------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sys.h" 


/* 定义 -----------------------------------------------------------------------------------------------------------------*/
#define Ain1  PBout(14)
#define Ain2  PBout(15)

#define Bin1  PBout(13)/*【4】*/
#define Bin2  PBout(12)

/* 提供给其他C文件调用的函数 --------------------------------------------------------------------------------------------*/
void Motor_Init(void);/*【5】*/
void Limit(int *motoA,int *motoB,float *servo);
int  abs(int p);
void Load(int moto2,int moto1,uint16_t Target_Position);


void set_motor_enable(void);
void set_motor_disable(void);


#endif /* _MOTOR_H */
/*****************************************************END OF FILE*********************************************************/	

