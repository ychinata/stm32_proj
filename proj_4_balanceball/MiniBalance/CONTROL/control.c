#include "control.h"	
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
int Max_Target=100;
u8 Flag_Target;// 任务标志位
int Flag_Move; //设定动作标志位

/**************************************************************************
函数功能：定时中断函数 所有的控制代码都在这里面		 
**************************************************************************/
void TIM1_UP_IRQHandler(void)
{    
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {   
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
        if(++Flag_Target>4)  Flag_Target=0;

        if(Flag_Target==1) {   //第一个10ms 采集Y轴数据        
            PBout(14)=0;    //给X方向+3.3V电压
            PBout(12)=1;
            PBout(13)=1;
            PBout(15)=0;	 
            Position_Y=Get_Adc(Y_IN)/20; //测量Y方向的						
            PBout(14)=1;   //恢复之前的状态
            PBout(12)=0;
            PBout(13)=0;
            PBout(15)=1;
        } else if (Flag_Target==2) {//第二个10ms 接收PS2遥控并调节PID参数        
            if(PS2_KEY>=13&&PS2_KEY<=16)Flag_Move=PS2_KEY-12;
            if(Flag_Move==0)Get_RC();      //PS2遥控方向
            else  Setting_Move();          //设定动作
            Adjust();  //PID参数调节
        } else if(Flag_Target==3) {//第三个10ms 采集X轴数据        
            PBout(14)=1; //给Y方向+3.3V电压
            PBout(12)=0;
            PBout(13)=0;
            PBout(15)=1;	
            Position_X=Get_Adc(X_IN)/15; //测量X方向的   		
            PBout(14)=0;   //恢复之前的状态
            PBout(12)=1;
            PBout(13)=1;
            PBout(15)=0;	 
        } else if(Flag_Target==4) {   //第四个10ms PID控制       
            Target_X=-balanceX(Position_X);
            Target_Y=-balanceY(Position_Y);
            if(Target_X<-Max_Target) Target_X=-Max_Target;	//位置控制模式中，A电机的运行速度
            if(Target_X>Max_Target)  Target_X=Max_Target;	  //位置控制模式中，A电机的运行速度
            if(Target_Y<-Max_Target) Target_Y=-Max_Target;	//位置控制模式中，B电机的运行速度
            if(Target_Y>Max_Target)  Target_Y=Max_Target;		//位置控制模式中，B电机的运行速度
            if(Flag_Stop==0)Set_Pwm(Target_X,Target_Y);
        }							
        Key();      //===扫描按键状态 单击双击可以改变小车运行状态
        if(delay_flag==1) {  //===给主函数提供50ms的精准延时        
            if(++delay_50==5)	 delay_50=0,delay_flag=0;       
        }		
    }       	
} 

/**************************************************************************
函数功能：X方向平衡PD控制
入口参数：角度
返回  值：直立控制PWM
作    者：平衡小车之家
**************************************************************************/
int balanceX(float Angle )
{  
    float  Differential,Bias,Balance_Ki=0.3;//定义差分变量和偏差
    static float Last_Bias,Integration,Balance_Integration,Flag_Target;  //上一次的偏差值
    int balance;//平衡的返回值
    Bias=(Angle-Zero_X);  //===求出平衡的角度中值 和机械相关  
    Differential=Bias-Last_Bias;  //求得偏差的变化率	 
    if(++Flag_Target>20) //错频处理积分控制
    {
        Flag_Target=0;
        if(Flag_Stop==0) Integration+=Bias;  // 检测到小球且舵机使能则积分
        else Integration=0;//否则清零
        if(Integration<-500) Integration=-500;	//积分限幅
        if(Integration>500)  Integration=500;	
        Balance_Integration=Integration*Balance_Ki;  //积分控制
    }		
    balance=Balance_Kp*Bias/100+Balance_Kd*Differential/10+Balance_Integration;   //===计算平衡控制的舵机PWM  PD控制   kp是P系数 kd是D系数 
    Last_Bias=Bias;  //保存上一次的偏差
    return balance;  //返回值
}

/**************************************************************************
函数功能：Y方向平衡PD控制
入口参数：角度
返回  值：直立控制PWM
作    者：平衡小车之家
**************************************************************************/
int balanceY(float Angle )
{  
    float  Differential,Bias,Balance_Ki=0.3;//定义差分变量和偏差
    static float Last_Bias,Integration,Balance_Integration,Flag_Target;  //上一次的偏差值
    int balance;//平衡的返回值
    Bias=(Angle-Zero_Y);  //===求出平衡的角度中值 和机械相关  
    Differential=Bias-Last_Bias;  //求得偏差的变化率	 
    if(++Flag_Target>20) //错频处理积分控制
    {
        Flag_Target=0;
        if(Flag_Stop==0) Integration+=Bias;  // 检测到小球且舵机使能则积分
        else Integration=0;//否则清零
        if(Integration<-500) Integration=-500;	//积分限幅
        if(Integration>500)  Integration=500;	
        Balance_Integration=Integration*Balance_Ki;  //积分控制
    }		
    balance=Balance_Kp*Bias/100+Balance_Kd*Differential/10+Balance_Integration;   //===计算平衡控制的舵机PWM  PD控制   kp是P系数 kd是D系数 
    Last_Bias=Bias;  //保存上一次的偏差
    return balance;  //返回值
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：舵机PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_x,int motor_y)
{
    TIM4->CCR1=740+motor_x;    //赋值给STM32的寄存器
    TIM4->CCR2=740+motor_y;
}

/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
    u8 temp;
    temp=click_N_Double(50);          //双击检测
    if(temp==1)	Flag_Stop=!Flag_Stop; //单击使能/失能
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
    int temp;
    if(a<0)  temp=-a;  
    else temp=a;
    return temp;
}

/**************************************************************************
函数功能：PID参数调节
入口参数：无
返回  值：无
**************************************************************************/
void Adjust(void)
{
    int X_temp,Y_temp,Threshold=100;//阈值
    if(PS2_LX!=0||PS2_LY!=0||PS2_RX!=0||PS2_RX!=0) //防止手柄没插入的时候调节
    {
        X_temp=PS2_RX-128;
        Y_temp=PS2_RY-128;
        if(PS2_KEY==5||PS2_KEY==6||PS2_KEY==7||PS2_KEY==8)//左边的任意4个按键按下才能调节PID参数，防止误触
        {
            if(X_temp>Threshold) Balance_Kp++;   //KP参数增加
            if(X_temp<-Threshold)Balance_Kp--;   //KP参数减小
            if(Y_temp>Threshold) Balance_Kd--;   //KD参数减小
            if(Y_temp<-Threshold)Balance_Kd++;   //KP参数增加
        }
    }
}
/**************************************************************************
函数功能：获取遥控器的参数
入口参数：无
返回  值：无
**************************************************************************/
void Get_RC(void)
{
    float zero_x,zero_y,Step=3;
    if(PS2_LX!=0||PS2_LY!=0||PS2_RX!=0||PS2_RX!=0) //防止手柄没插入的时候调节
    {
        zero_x=ZERO_X+(PS2_LX-128)/3;  //摇杆X方向控制球X方向的移动
        zero_y=ZERO_Y-(PS2_LY-128)/4;  //摇杆Y方向控制球Y方向的移动
        if(Zero_X<zero_x) Zero_X+=Step;
        if(Zero_X>zero_x) Zero_X-=Step;
        if(Zero_Y<zero_y) Zero_Y+=Step;
        if(Zero_Y>zero_y) Zero_Y-=Step;
    }
}
/**************************************************************************
函数功能：特定轨迹运动
入口参数：无
返回  值：无
**************************************************************************/
void Setting_Move(void)
{
    static float count;  //计数变量
    count++;  //自加
    if(Flag_Move==1)  //控制小球沿着三角形的轨迹运动
    {
        if(count<40) Zero_Y++;   
        else if(count<80) Zero_Y-=2,Zero_X-=1.5;
        else if(count<120)Zero_X+=3;
        else if(count<160)Zero_Y+=1,Zero_X-=1.5,Flag_Move=0,count=0;//最后一步，回到原点 计数器清零
    } else if(Flag_Move==2) {  //控制小球沿着球的轨迹运动
        if(count<40) Zero_Y++;
        else if(count<40+PI*40)Zero_Y=ZERO_Y+40*cos((count-40)/20),Zero_X=ZERO_X+40*sin((count-40)/20);
        else if(count<210)Zero_Y--,Flag_Move=0,count=0;//最后一步，回到原点 计数器清零
    } else	if(Flag_Move==3) {  //控制小球沿着叉(X)的轨迹运动    
        if(count<40) Zero_Y++,Zero_X--;
        else if(count<120)Zero_Y--,Zero_X++;
        else if(count<160)Zero_Y++,Zero_X--;
        else if(count<200)Zero_Y++,Zero_X++;
        else if(count<280)Zero_Y--,Zero_X--;
        else if(count<320)Zero_Y++,Zero_X++,Flag_Move=0,count=0;//最后一步，回到原点 计数器清零
    } else if(Flag_Move==4) {//控制小球沿着正方形的轨迹运动
        if(count<40) Zero_Y++;
        else if(count<80) Zero_X++;
        else if(count<160)Zero_Y--;
        else if(count<240)Zero_X--;
        else if(count<320)Zero_Y++;
        else if(count<360)Zero_X++;
        else if(count<400)Zero_Y--,Flag_Move=0,count=0;//最后一步，回到原点 计数器清零
    }
}
