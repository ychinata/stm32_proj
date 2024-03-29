#include "key.h"
#include "delay.h"
#include "led.h"


//按键及电源控制初始化
void KEY_IO_Init(void) //IO初始化
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


volatile u8 KeyMask=0;//按键标记
volatile u8 KeyDat=0;
u8 g_KeyType=0;
void Key_Scan(void)
{
    if ( KEY1 ==0)//KEY按下
    {		
        if(KeyDat & 0x01)//检查按键按下标志位
        {
            if(TIM1->CNT >= KEYDOWNMINTIME)//延时大于1s（按键有效）
            {
                if(TIM1->CNT >= KEYDOWNLONGTIME)//延时大于2s（长按键）
                {		
                    KeyMask=KEYLONGDOWN;
                    LED2=LED_ON;//按键有效								
                }								
                else//短按键事件	
                {								
                    KeyMask=KEYSHORTDOWN; 	
                    LED2=LED_OFF;//按键有效								
                }
            }	
        }
        else//按键按下瞬间
        {			
            TIM_Cmd(TIM1, ENABLE); 
            TIM_Cmd(TIM3, DISABLE); 

            TIM1->CNT=0;//清空定时器计数器	
            LED2=LED_ON;//按键按下瞬间
            KeyDat |= 0x01;	//首次按键标记
            KeyMask=KEYUNKNOW ;//瞬间按下为无效状态				
        }
    }	
    else//无按下按下，或者按键弹起
    {
        if(KeyDat & 0x01)	//按键按下后弹起
        {	
            KeyDat &= ~0x01;//清按键标志
            TIM_Cmd(TIM1, DISABLE);
            TIM_Cmd(TIM3, ENABLE); 

            LED2=LED_OFF;
            if(KeyMask==KEYSHORTDOWN )//短按键事件
            {				
                KeyMask=KEYUNKNOW;//清变量		
                g_KeyType		= KEYSHORTDOWN; 				
            }					
            else if(KeyMask==KEYLONGDOWN)//长按键事件
            {					
                KeyMask=KEYUNKNOW;//清变量
                g_KeyType		= KEYLONGDOWN; 				
            }
        }		
    }
}				    


