#ifndef __gray_track_h_
#define __gray_track_h_

// 循迹传感器引脚配置
#define TRACK_PIN_R1	GPIO_Pin_5
#define TRACK_PIN_M		GPIO_Pin_4
#define TRACK_PIN_L1	GPIO_Pin_3
// 循迹传感器引脚配置
// 根据循迹传感器的输出电平配置
#define TRACK_INLINE	0
#define TRACK_OFFLINE	1
// 选择循迹模块的类型
#define TRACK_WAY3_WIDE 0
#define TRACK_WAY3_NARROW 1
#define TRACK_WAY5_WIDE 2
#define TRACK_WAY_TYPE TRACK_WAY3_WIDE

void TRACK_Init(void);
void TRACK_ControlPID(void);
void TRACK_GetStatus(void);
unsigned char digtal(unsigned char channel);// 什么函数?
void TRACK_ControlNoPID(void);

#endif
