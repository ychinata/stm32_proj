#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define Ain1 	PBout(12)
#define Ain2 	PBout(13)

#define Bin1 	PBout(14)
#define Bin2 	PBout(15)


int ZDS_abs(int p);
void Load(int motor1, int motor2);
void Limit(int *motorA, int *motorB);
void Motor_Init(void);
void brake(void);


#endif
