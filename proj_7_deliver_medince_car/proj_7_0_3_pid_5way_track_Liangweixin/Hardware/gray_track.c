#include "stm32f10x.h"                  // Device header
#include "gray_track.h"               
#include "pid.h"     

uint8_t D1,D2,D3 = 0;
/* 
     三路寻迹模块 
     PB5 ——> L1 左1
     PB4 ——> M  中
     PB3 ——> R1 右1

*/

/* 灰度传感器初始化 */
void gray_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOB时钟和复用功能（要先打开复用才能修改复用功能）
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);                      //PB3,PB4上电之后默认是SWJ调试端口，配置输入输出无效
                                                                                 //失能SWJ调试端口 才可正常使用
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5 | GPIO_Pin_3;     //三路寻迹
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/* 
    根据寻迹模块 修改pid目标值 
*/
void track(void)                        
{
    Read_digital();
	if(D1==0 && D2== 1 && D3==0){
        motor_target_set(60,60);
    }    
    else if(D1==1 && D2==1 && D3==0){
        motor_target_set(45,60);
    }  
    else if(D1==1 && D2==0 && D3==0){
        motor_target_set(30,60);
    }  
    else if(D1==0 && D2==1 && D3==1){
        motor_target_set(60,45);
    }      
    else if(D1==0 && D2==0 && D3==1){
        motor_target_set(60,30);
    }
    
}

/* 
    读取寻迹模块引脚的电平
*/
void Read_digital(void){
    D3=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);     //R1
    D2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);    //M
    D1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);    //L1
}


