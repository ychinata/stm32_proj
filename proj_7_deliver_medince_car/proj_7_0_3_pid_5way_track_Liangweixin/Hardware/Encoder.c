#include "stm32f10x.h"                  // Device header

static void Encoder_Timer4_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);     //定时器4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //开启GPIOB时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//向上计数 此处无无作用
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;						//ARR	 自动重装器
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;						//PSC  预分配器0 不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);                                   //结构体初始化
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;						  //选择通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;									  //配置滤波
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
//1；选择定时器 2；编码器计算模式 3；IC1的极性	4；IC2的极性	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	 
	TIM_Cmd(TIM4, ENABLE); //开启定时器
}




static void Encoder_Timer2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);     //定时器2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //开启GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 			//向上计数 此处无无作用
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;							//ARR	 自动重装器
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;							//PSC  预分配器0 不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);                                   //结构体初始化
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;						 //选择通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;								    //配置滤波
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
//1；选择定时器 2；编码器计算模式 3；IC1的极性	4；IC2的极性	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	 
	TIM_Cmd(TIM2, ENABLE); //开启定时器
}


void Encoder_Init(void){
    Encoder_Timer2_Init();
    Encoder_Timer4_Init();
}
    

int16_t Encoder_Right_Get(void)
{
	int16_t Temp; //定义临时变量 存放CNT的值
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);   //给CNT清零
	return Temp;
}


int16_t Encoder_Left_Get(void)
{
	int16_t Temp; //定义临时变量 存放CNT的值
	Temp = TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2, 0);   //给CNT清零
	return Temp;
}

