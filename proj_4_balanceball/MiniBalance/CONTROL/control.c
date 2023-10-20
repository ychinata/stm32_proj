#include "control.h"	
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
int Max_Target=100;
u8 Flag_Target;// �����־λ
int Flag_Move; //�趨������־λ

/**************************************************************************
�������ܣ���ʱ�жϺ��� ���еĿ��ƴ��붼��������		 
**************************************************************************/
void TIM1_UP_IRQHandler(void)
{    
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {   
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
        if(++Flag_Target>4)  Flag_Target=0;

        if(Flag_Target==1) {   //��һ��10ms �ɼ�Y������        
            PBout(14)=0;    //��X����+3.3V��ѹ
            PBout(12)=1;
            PBout(13)=1;
            PBout(15)=0;	 
            Position_Y=Get_Adc(Y_IN)/20; //����Y�����						
            PBout(14)=1;   //�ָ�֮ǰ��״̬
            PBout(12)=0;
            PBout(13)=0;
            PBout(15)=1;
        } else if (Flag_Target==2) {//�ڶ���10ms ����PS2ң�ز�����PID����        
            if(PS2_KEY>=13&&PS2_KEY<=16)Flag_Move=PS2_KEY-12;
            if(Flag_Move==0)Get_RC();      //PS2ң�ط���
            else  Setting_Move();          //�趨����
            Adjust();  //PID��������
        } else if(Flag_Target==3) {//������10ms �ɼ�X������        
            PBout(14)=1; //��Y����+3.3V��ѹ
            PBout(12)=0;
            PBout(13)=0;
            PBout(15)=1;	
            Position_X=Get_Adc(X_IN)/15; //����X�����   		
            PBout(14)=0;   //�ָ�֮ǰ��״̬
            PBout(12)=1;
            PBout(13)=1;
            PBout(15)=0;	 
        } else if(Flag_Target==4) {   //���ĸ�10ms PID����       
            Target_X=-balanceX(Position_X);
            Target_Y=-balanceY(Position_Y);
            if(Target_X<-Max_Target) Target_X=-Max_Target;	//λ�ÿ���ģʽ�У�A����������ٶ�
            if(Target_X>Max_Target)  Target_X=Max_Target;	  //λ�ÿ���ģʽ�У�A����������ٶ�
            if(Target_Y<-Max_Target) Target_Y=-Max_Target;	//λ�ÿ���ģʽ�У�B����������ٶ�
            if(Target_Y>Max_Target)  Target_Y=Max_Target;		//λ�ÿ���ģʽ�У�B����������ٶ�
            if(Flag_Stop==0)Set_Pwm(Target_X,Target_Y);
        }							
        Key();      //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬
        if(delay_flag==1) {  //===���������ṩ50ms�ľ�׼��ʱ        
            if(++delay_50==5)	 delay_50=0,delay_flag=0;       
        }		
    }       	
} 

/**************************************************************************
�������ܣ�X����ƽ��PD����
��ڲ������Ƕ�
����  ֵ��ֱ������PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balanceX(float Angle )
{  
    float  Differential,Bias,Balance_Ki=0.3;//�����ֱ�����ƫ��
    static float Last_Bias,Integration,Balance_Integration,Flag_Target;  //��һ�ε�ƫ��ֵ
    int balance;//ƽ��ķ���ֵ
    Bias=(Angle-Zero_X);  //===���ƽ��ĽǶ���ֵ �ͻ�е���  
    Differential=Bias-Last_Bias;  //���ƫ��ı仯��	 
    if(++Flag_Target>20) //��Ƶ������ֿ���
    {
        Flag_Target=0;
        if(Flag_Stop==0) Integration+=Bias;  // ��⵽С���Ҷ��ʹ�������
        else Integration=0;//��������
        if(Integration<-500) Integration=-500;	//�����޷�
        if(Integration>500)  Integration=500;	
        Balance_Integration=Integration*Balance_Ki;  //���ֿ���
    }		
    balance=Balance_Kp*Bias/100+Balance_Kd*Differential/10+Balance_Integration;   //===����ƽ����ƵĶ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
    Last_Bias=Bias;  //������һ�ε�ƫ��
    return balance;  //����ֵ
}

/**************************************************************************
�������ܣ�Y����ƽ��PD����
��ڲ������Ƕ�
����  ֵ��ֱ������PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balanceY(float Angle )
{  
    float  Differential,Bias,Balance_Ki=0.3;//�����ֱ�����ƫ��
    static float Last_Bias,Integration,Balance_Integration,Flag_Target;  //��һ�ε�ƫ��ֵ
    int balance;//ƽ��ķ���ֵ
    Bias=(Angle-Zero_Y);  //===���ƽ��ĽǶ���ֵ �ͻ�е���  
    Differential=Bias-Last_Bias;  //���ƫ��ı仯��	 
    if(++Flag_Target>20) //��Ƶ������ֿ���
    {
        Flag_Target=0;
        if(Flag_Stop==0) Integration+=Bias;  // ��⵽С���Ҷ��ʹ�������
        else Integration=0;//��������
        if(Integration<-500) Integration=-500;	//�����޷�
        if(Integration>500)  Integration=500;	
        Balance_Integration=Integration*Balance_Ki;  //���ֿ���
    }		
    balance=Balance_Kp*Bias/100+Balance_Kd*Differential/10+Balance_Integration;   //===����ƽ����ƵĶ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
    Last_Bias=Bias;  //������һ�ε�ƫ��
    return balance;  //����ֵ
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ��������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_x,int motor_y)
{
    TIM4->CCR1=740+motor_x;    //��ֵ��STM32�ļĴ���
    TIM4->CCR2=740+motor_y;
}

/**************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
    u8 temp;
    temp=click_N_Double(50);          //˫�����
    if(temp==1)	Flag_Stop=!Flag_Stop; //����ʹ��/ʧ��
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
    int temp;
    if(a<0)  temp=-a;  
    else temp=a;
    return temp;
}

/**************************************************************************
�������ܣ�PID��������
��ڲ�������
����  ֵ����
**************************************************************************/
void Adjust(void)
{
    int X_temp,Y_temp,Threshold=100;//��ֵ
    if(PS2_LX!=0||PS2_LY!=0||PS2_RX!=0||PS2_RX!=0) //��ֹ�ֱ�û�����ʱ�����
    {
        X_temp=PS2_RX-128;
        Y_temp=PS2_RY-128;
        if(PS2_KEY==5||PS2_KEY==6||PS2_KEY==7||PS2_KEY==8)//��ߵ�����4���������²��ܵ���PID��������ֹ��
        {
            if(X_temp>Threshold) Balance_Kp++;   //KP��������
            if(X_temp<-Threshold)Balance_Kp--;   //KP������С
            if(Y_temp>Threshold) Balance_Kd--;   //KD������С
            if(Y_temp<-Threshold)Balance_Kd++;   //KP��������
        }
    }
}
/**************************************************************************
�������ܣ���ȡң�����Ĳ���
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_RC(void)
{
    float zero_x,zero_y,Step=3;
    if(PS2_LX!=0||PS2_LY!=0||PS2_RX!=0||PS2_RX!=0) //��ֹ�ֱ�û�����ʱ�����
    {
        zero_x=ZERO_X+(PS2_LX-128)/3;  //ҡ��X���������X������ƶ�
        zero_y=ZERO_Y-(PS2_LY-128)/4;  //ҡ��Y���������Y������ƶ�
        if(Zero_X<zero_x) Zero_X+=Step;
        if(Zero_X>zero_x) Zero_X-=Step;
        if(Zero_Y<zero_y) Zero_Y+=Step;
        if(Zero_Y>zero_y) Zero_Y-=Step;
    }
}
/**************************************************************************
�������ܣ��ض��켣�˶�
��ڲ�������
����  ֵ����
**************************************************************************/
void Setting_Move(void)
{
    static float count;  //��������
    count++;  //�Լ�
    if(Flag_Move==1)  //����С�����������εĹ켣�˶�
    {
        if(count<40) Zero_Y++;   
        else if(count<80) Zero_Y-=2,Zero_X-=1.5;
        else if(count<120)Zero_X+=3;
        else if(count<160)Zero_Y+=1,Zero_X-=1.5,Flag_Move=0,count=0;//���һ�����ص�ԭ�� ����������
    } else if(Flag_Move==2) {  //����С��������Ĺ켣�˶�
        if(count<40) Zero_Y++;
        else if(count<40+PI*40)Zero_Y=ZERO_Y+40*cos((count-40)/20),Zero_X=ZERO_X+40*sin((count-40)/20);
        else if(count<210)Zero_Y--,Flag_Move=0,count=0;//���һ�����ص�ԭ�� ����������
    } else	if(Flag_Move==3) {  //����С�����Ų�(X)�Ĺ켣�˶�    
        if(count<40) Zero_Y++,Zero_X--;
        else if(count<120)Zero_Y--,Zero_X++;
        else if(count<160)Zero_Y++,Zero_X--;
        else if(count<200)Zero_Y++,Zero_X++;
        else if(count<280)Zero_Y--,Zero_X--;
        else if(count<320)Zero_Y++,Zero_X++,Flag_Move=0,count=0;//���һ�����ص�ԭ�� ����������
    } else if(Flag_Move==4) {//����С�����������εĹ켣�˶�
        if(count<40) Zero_Y++;
        else if(count<80) Zero_X++;
        else if(count<160)Zero_Y--;
        else if(count<240)Zero_X--;
        else if(count<320)Zero_Y++;
        else if(count<360)Zero_X++;
        else if(count<400)Zero_Y--,Flag_Move=0,count=0;//���һ�����ص�ԭ�� ����������
    }
}
