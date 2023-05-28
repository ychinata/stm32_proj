3.WHEELTEC B570 平衡小车源码（库函数版）


PID参数调节：
一.单独调试直立环 Balance
1.Balance_Kp极性为正：设为300，倾斜时会加速倒下
2.Balance_Kp极性为负：设为300，倾斜时不会加速倒下
3.Balance_Kd极性为正：设为1，倾斜时会加速倒下
4.Balance_Kd极性为负：设为1，倾斜时不会加速倒下
5.Balance_Kp，Balance_Kd极性为负，一直会往前跑，无法平衡

二.单独调试速度环 Velocity
1.Velocity_Kp极性为正：设为100，向前转动右轮，左轮会向后转动。这是负反馈，而速度环应该是正反馈。
2.Velocity_Kp和Velocity_Ki必须为同极性，故都设为正。Velocity_Ki设为0。转动一边轮子，另一边轮子会跟着转动，并且一直加速。

三.单独调试转向环 Turn
直接修改turn公式的值，不修改全局变量
1.Turn_Kp,Turn_Kd极性都设置为正：设Turn_Kp=0，设gyro*Kd=gyro*100，用手摁住小车在地面，向一边转向，有阻力作用，表明Turn_Kd极性为正是正确的。
2.如果把Turn_Kd设为正，则小车用手转向时，会加速转向。



