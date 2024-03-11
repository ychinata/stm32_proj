#include "stm32f10x.h"                  // Device header

//void PWM2_Init(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	TIM_InternalClockConfig(TIM4);
//	
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
//	
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 3000;		//CCR
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//	
//	TIM_ARRPreloadConfig(TIM4, ENABLE);  
//	TIM_CtrlPWMOutputs(TIM4,ENABLE);
//	TIM_Cmd(TIM4, ENABLE);
//}
void PWM2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;   					 				//��ʼ��TIM1����ARR,PSC�������PWM������
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //PWMͨ������
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);     			  //ʹ�ܶ�ʱ��4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);      				//AFIO���ù���ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   				//GPIOAʹ��
	
	
	//���ø�����Ϊ�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	       //TIM1_CH1/CH2/CH3/CH4
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;                                     //�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);                                               //��ʼ��GPIO
	
	
	//��ʼ��TIM1,����TIM4��ARR��PSC
	TIM_TimeBaseStruct.TIM_Period = 20000 - 1;                   					 //�����Զ���װ������ֵ   //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStruct.TIM_Prescaler = 72 - 1;                					 //����Ԥ��Ƶֵ           //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;               				 //����ʱ�ӷָ�TDTS = Tck_tim	
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;         //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStruct);                      //����ָ��������ʼ��TIMx  //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_OCInitStructure.TIM_Pulse = 3000;


	
	//��ʼ��������Ƚϲ���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;          //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);                            //����TIMX�Ĳ����趨��ʼ������ TIM1 ch1 ch2 ch3 ch4	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);                   //ʹ��Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);                                
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);    
	
	

}




void PWM_SetCompare5(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}

void PWM_SetCompare6(uint16_t Compare)
{
	TIM_SetCompare2(TIM1, Compare);
}

