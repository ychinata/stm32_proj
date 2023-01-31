#include "Timer.h"
#include "led.h" 
#include "key.h"
#include "myQueue.h"

#include "adc.h"
#include "usart.h"      	
#include <string.h>

//�߼� TIM1 TIM8
//ͨ�� TIM2 TIM3 TIM4 TIM5
//���� TIM6 TIM7

//C8T6 ֻ�� 1-4 4����ʱ��
u32 Tim1_flag;
void TIM1_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

    TIM_TimeBaseStructure.TIM_Period=arr-1; // �Զ���װ�ؼĴ�����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler=psc-1; // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; // ������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //�ظ��Ĵ����������Զ�����pwmռ�ձ�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); 

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update|TIM_IT_Trigger); //��������ж�����λ
    TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_Trigger, ENABLE );//�������										
    TIM_Cmd(TIM1, DISABLE); 
}

//��ʱ���жϷ������	 
void TIM1_UP_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //�����ж� 
    { 			
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//���жϱ�־
        TIM_Cmd(TIM1, DISABLE); 
        TIM1->CNT=KEYDOWNLONGTIME; //������
    }		
}



//��ʱ����ʼ��
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
 
    TIM_TimeBaseStructure.TIM_Period = arr-1;; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
    TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); 
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //��������ж�����λ
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//�������										
    TIM_Cmd(TIM2, ENABLE); 
}

extern u8 rx_databuf[256];
extern u8 rx_length;
//��ʱ���жϷ������	 
void TIM2_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {  //�����ж�  			
        if(USART2_RX_EVENT==0) { //���ڿ���        
            USART2_TIME++;
            if(USART2_TIME>=20) {//20ms ���            
                USART2_TIME=0;
                if(USART2_RX_LEN) {                
                    USART2_RX_EVENT=1; //������������Ҫ����	
                    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); 			//����2�����ж� 
                }
            }	
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���жϱ�־
    }			
}

void TIM3_Init(u16 arr,u16 psc)
{	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    

    //��ʼ��TIM
    TIM_TimeBaseStructure.TIM_Period = arr-1;    //�Զ���װ�ؼĴ���ֵ ����pwmƵ��
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;  //Ԥ������Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶϵ��������Ƶ   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���ģʽ            
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ�� TIMx

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); 

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //��������ж�����λ
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );//�������							
    TIM_Cmd(TIM3,ENABLE);//ʹ��
}

u8 TIM3_Timing=0;
//��ʱ���жϷ������	 
void TIM3_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) { //�����ж�     
        TIM3_Timing++;
        DS1=!DS1;		
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���жϱ�־
    }     				   
}

void TIM4_Init(u16 arr,u16 psc) 
{    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef TIM_OCInitStructure;	
    // ���������ж�     NVIC_InitStructure, Ҳ�����Ӧ���жϺ���?

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
                            
    TIM_TimeBaseStructure.TIM_Period = arr-1;                	//���ü���ֵ1s����            
    TIM_TimeBaseStructure.TIM_Prescaler = psc-1;              	//����Ԥ��Ƶֵ    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶϵ��������Ƶ    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);      	

    //��ʱ������ DMA ADC
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = arr/2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//�����PWM1ҪΪLow��PWM2��ΪHigh
    TIM_OC4Init(TIM4, & TIM_OCInitStructure);

    TIM_Cmd(TIM4, ENABLE);                                       //��ʱ��ʹ�� 	
    TIM_CtrlPWMOutputs(TIM4,ENABLE);
} 
 
//		TIM3->CNT=0;//��ն�ʱ��������
//		TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
//		TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
//		TIM3->DIER|=1<<0;   //��������ж�	  
//		TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3

