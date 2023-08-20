/*软件改进点*/
1.移植轮趣的蓝牙调试代码
2.上电时显示PID参数6个

/*硬件改进点*/
1.增加电机的开关
2.增加按键
3.兼容TB6612
4.兼容MPU6050的INT脚
5.修改PCB尺寸及打孔

/*参数设置*/
//机械0点
Mechanical_balance
//Set_PWM
motor2<0
//电机死区
Dead_Zone
//PID参数
PID

//PID调试参考《WHEELTEC B570 平衡小车开发手册V5.7（第6章 PID参数调节）》
一.单独调试直立环
kp为负时，倾斜时会加速倒下，故极性不对，应该为正。
kp=+160
kd为负时，倾斜时会加速倒下，故极性不对，应该为正。
kp=+0.7，向前倾斜时，小车也会前进，有一个跟随作用。如果kp=±0.1，现象都不明显。、

二.单独调试速度环
kp为负时，-100，小车直立时轮子会慢慢地转，此时手动正转右轮，左轮会反转，证明是负反馈，此时极性错误。
kp为正时，ki也设置为正。{kp, ki}={100,0.5}/{50,0.25}（轮趣建议ki=kp/200）。此时小车直立时，两个轮子都会满速转动。
设置直立环{kp,kd}={160,0.7}，速度环{kp,ki}={50,0.25}，小车可以平衡，但会慢慢的旋转。

三.单独调试转向环
单独kd为负时，-1，把小车摁在地面，向一侧转向，正反馈，小车会迅速转向。
单独kd为正时，+1，把小车摁在地面，向一侧转向，负反馈，小车有对抗力。
单独kp为正时，+50/+100，航向角超过大约±20度时，小车就会迅速转向。
单独kp为负时，-50，只要航向角不为零，小车就会转向恢复到航向角为零的位置。如何解释？
kp为-50/-10归正时会抖动得非常厉害。而且此时小车是不能转向的，只能前进/后退。
kd为+1时，小车可以转向，但抖动比较明显。为+0.25/0.5时，抖动比较小。

/**************** 重要函数 ********************/
Car_Task_200HZ
Car_Task_100HZ
Car_Task_5HZ
Car_Task_Interaction

/**************** 硬件连接 ********************/
// OLED
SCL <-> PB8
SDA <-> PB9
// 超声波
TRIG <-> PA2
ECHO <-> PA8
// 四路红外寻迹
LA <-> PB5
LB <-> PB4
RA <-> PA15
RB <-> PB3
//蓝牙
TX <-> PB11
RX <-> PB10
//MPU6050 //如何做到与OLED共用接口？
SCL <-> PB8
SDA <-> PB9
//DRV8833
AIN1 <-> PA7
AIN2 <-> PA3
BIN1 <-> PA6
BIN2 <-> PA4
//电机编码器
AC1 <-> PA1
AC2 <-> PA0
BC1 <-> PB7
BC2 <-> PB6


/**************** 软件模块 ********************/
// OLED-MX_GPIO_Init-OLED_Init(StartTask_200HZ)
SCL <-> PB8
SDA <-> PB9
//MPU6050 //如何做到与OLED共用接口？通过地址区分？
StartTask_200HZ->mpu_dmp_init
#define MPU_ADDR		0X68
#define OLED0561_ADD	0x78
i2c_write->MPU_Write_Len->MPU_IIC_Send_Byte->IIC_Send_Byte

// 超声波-MX_GPIO_Init
TRIG <-> PA2
ECHO <-> PA8
// 四路红外寻迹
LA <-> PB5
LB <-> PB4
RA <-> PA15
RB <-> PB3
//蓝牙-MX_USART3_UART_Init/ User_USART_Init /HAL_UART_MspInit(引脚)
TX <-> PB11
RX <-> PB10


//DRV8833 //ABIN2-MX_GPIO_Init //ABIN1-
AIN1 <-> PA7
BIN1 <-> PA6
AIN2 <-> PA3
BIN2 <-> PA4
//电机编码器 //A01/B67-HAL_TIM_Encoder_MspInit
AC1 <-> PA1
AC2 <-> PA0
BC1 <-> PB7
BC2 <-> PB6

/**************** 定时器 ********************/
TIM2：编码器AC1/2:MX_TIM2_Init-HAL_TIM_Encoder_Init-HAL_TIM_Encoder_MspInit
TIM3：PWM：       MX_TIM3_Init-HAL_TIM_PWM_Init    -HAL_TIM_MspPostInit
TIM4: 编码器BC1/2:MX_TIM4_Init-HAL_TIM_Encoder_Init-HAL_TIM_Encoder_MspInit

    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2 
	//CH3/4?
// 这四个通道是怎么映射的？
#define PWMA1   TIM3->CCR2
#define PWMA2   TIM3->CCR3
#define PWMB1   TIM3->CCR1
#define PWMB2   TIM3->CCR4	