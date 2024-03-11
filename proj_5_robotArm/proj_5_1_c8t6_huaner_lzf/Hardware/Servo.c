#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PWM2.h"

void Servo_Init(void)
{
	PWM_Init();     //PA0-3
	PWM2_Init();    //PA8/PA9
}

void Servo_SetAngle1(float Angle)
{
	PWM_SetCompare1(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle2(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle3(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle4(float Angle)
{
	PWM_SetCompare4(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle5(float Angle)
{
	PWM_SetCompare5(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle6(float Angle)
{
	PWM_SetCompare6(Angle / 180 * 2000 + 500);
}
