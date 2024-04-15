#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

#define target_k210	110			//巡线目标中值


void car_turn(int pluse, int mode, int pwm);

extern int pathlenth;
extern int Encoder_left, Encoder_right;

int distance_limit(int now, int *pwm, int target);
void car_go(int distance, int pwm);
void distance_go(int target_distance, int pwm);
int PID_k210(int now, int target);


#endif
