#ifndef _headfile_h_
#define _headfile_h_

#include "stm32f10x.h"                  // Device header
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#include "ml_uart.h"
#include "ml_tim.h"
#include "ml_pwm.h"
#include "ml_oled.h"
#include "ml_delay.h"
#include "ml_gpio.h"
#include "ml_nvic.h"
#include "ml_adc.h"
#include "ml_exti.h"
#include "ml_i2c.h"
#include "ml_mpu6050.h"

#include "motor.h"
#include "pid.h"
#include "gray_track.h"


//pin
/* 
   软件模拟I2C协议
	 修改下面三个参数 
	 自定义OLED的SCL和SDA引脚
	 需将对应引脚配置成开漏输出	 
*/
#define OLED_GPIO     GPIO_B
#define OLED_SCL_Pin  Pin_8
#define OLED_SDA_Pin  Pin_9

//pin
#endif
