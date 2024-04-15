#ifndef __PID_H
#define __PID_H

#include "sys.h"
int Verocity(int encoder_left, int encoder_right);
int Position_PID(float value, float Target);

#endif
