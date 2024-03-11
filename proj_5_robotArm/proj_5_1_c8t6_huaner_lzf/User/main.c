#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "Servo.h"
#include <math.h>
#include "CountSensor.h"

/*
 MPU6050：获取的数据分别表示什么？
 舵机1-6：分别控制哪个？
 OLED
 传感器B14
*/

int16_t AX, AY, AZ, GX, GY, GZ, ON;
float jiaodu1, jiaodu2, jiaodu3, jiaodu4, jiaodu5;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	Servo_Init();
	OLED_ShowString(1, 1, "Servo12:");
	OLED_ShowString(2, 1, "Servo34:");
	OLED_ShowString(3, 1, "Servo5:");
	OLED_ShowString(4, 1, "Servo6:");
	CountSensor_Init();
	
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		CountSensor_Get(&ON);
		if(AX/24 > -90 | AX/24 < 90){jiaodu1 = AX/24+90;}
		if(AY/24 > -90 | AY/24 < 90){jiaodu2 = AY/24+90;}
		if(AZ/24 > -90 | AZ/24 < 90){jiaodu3 = AZ/24+40;}
		if(AX/24 > -90 | AX/24 < 90){jiaodu4 = AX/35+90;}
		if(GY/24 > -90 | GY/24 < 90){jiaodu5 = GY/24+90;}
		if(ON == 1){
            OLED_ShowString(4, 9, "out");		
            Servo_SetAngle6(0);//b0
        } else if(ON == 0){
            OLED_ShowString(4, 9, "off");		
            Servo_SetAngle6(90);//b1
        }
		OLED_ShowNum(1, 9, jiaodu2, 3);
		OLED_ShowNum(1, 13, jiaodu3, 3);
		OLED_ShowNum(2, 9, jiaodu4, 3);
		OLED_ShowNum(2, 13, jiaodu1, 3);
		OLED_ShowNum(3, 9, jiaodu2, 3);
//		OLED_ShowNum(3, 9, CountSensor_Get(), 2);
//		OLED_ShowNum(4, 8, GZ/24+90, 3);
		Servo_SetAngle1(jiaodu2);//a0
		Servo_SetAngle2(jiaodu3);//a1
		Servo_SetAngle3(jiaodu4);//a2
        Servo_SetAngle4(jiaodu1);//a3
		Servo_SetAngle5(jiaodu2);//b0
		
	}
}
