#include "Timer.h"
#include "led.h" 
#include "key.h"
#include "myQueue.h"

#include "adc.h"
#include "usart.h"      	
#include <string.h>

//高级 TIM1 TIM8
//通用 TIM2 TIM3 TIM4 TIM5
//基本 TIM6 TIM7

//C8T6 只有 1-4 4个定时器
u32 Tim1_flag;
void TIM1_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

    TIM_TimeBaseStructure.TIM_Period=arr-1; // 自动重装载寄存器的值
    TIM_TimeBaseStructure.TIM_Prescaler=psc-1; // 时钟预分频数
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; // 采样分频
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //重复寄存器，用于自动更新pwm占空比
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); 

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update|TIM_IT_Trigger); //清除更新中断请求位
    TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_Trigger, ENABLE );//允许更新										
    TIM_Cmd(TIM1, DISABLE); 
}

//定时器中断服务程序	 
void TIM1_UP_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //更新中断 
    { 			
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清中断标志
        TIM_Cmd(TIM1, DISABLE); 
        TIM1->CNT=KEYDOWNLONGTIME; //长按键
    }		
}



//定时器初始化
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
 
    TIM_TimeBaseStructure.TIM_Period = arr-1;; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
    TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); 
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除更新中断请求位
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//允许更新										
    TIM_Cmd(TIM2, ENABLE); 
}

extern u8 rx_databuf[256];
extern u8 rx_length;
//定时器中断服务程序	 
void TIM2_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {  //更新中断  			
        if(USART2_RX_EVENT==0) { //串口空闲        
            USART2_TIME++;
            if(USART2_TIME>=20) {//20ms 解包            
                USART2_TIME=0;
                if(USART2_RX_LEN) {                
                    USART2_RX_EVENT=1; //串口有数据需要处理	
                    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 			//串口2接收中断 
                }
            }	
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志
    }			
}

void TIM3_Init(u16 arr,u16 psc)
{	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    

    //初始化TIM
    TIM_TimeBaseStructure.TIM_Period = arr-1;    //自动重装载寄存器值 决定pwm频率
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;  //预分设置预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分频系数，不分频   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数模式            
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化 TIMx

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); 

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除更新中断请求位
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );//允许更新							
    TIM_Cmd(TIM3,ENABLE);//使能
}

u8 TIM3_Timing=0;
//定时器中断服务程序	 
void TIM3_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) { //更新中断     
        TIM3_Timing++;
        DS1=!DS1;		
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清中断标志
    }     				   
}

void TIM4_Init(u16 arr,u16 psc) 
{    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef TIM_OCInitStructure;	
    // 无需配置中断     NVIC_InitStructure, 也无需对应的中断函数?

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
                            
    TIM_TimeBaseStructure.TIM_Period = arr-1;                	//设置计数值1s更新            
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;              	//设置预分频值    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频系数，不分频    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);      	

    //定时器触发 DMA ADC
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = arr/2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//如果是PWM1要为Low，PWM2则为High
    TIM_OC4Init(TIM4, & TIM_OCInitStructure);

    TIM_Cmd(TIM4, ENABLE);                                       //定时器使能 	
    TIM_CtrlPWMOutputs(TIM4,ENABLE);
} 
 
//		TIM3->CNT=0;//清空定时器计数器
//		TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
//		TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
//		TIM3->DIER|=1<<0;   //允许更新中断	  
//		TIM3->CR1|=0x01;    //使能定时器3

