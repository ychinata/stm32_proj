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

/* 
 *Func: 灰度传感器初始化 
 *History:
 	1.Rename:gray_init->TRACK_Init 2024.8.5
 */
void TRACK_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOB时钟和复用功能（要先打开复用才能修改复用功能）
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);                      //PB3,PB4上电之后默认是SWJ调试端口，配置输入输出无效
                                                                                 //使能SWJ调试端口 才可正常使用
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;               //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = TRACK_PIN_L1 | TRACK_PIN_M | TRACK_PIN_R1;     //三路寻迹
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/* 
 *Func: 根据寻迹模块检测到的状态，修改pid目标值 
 *History:
   1.Rename:track->TRACK_Control 2024.8.5
 */

// 五路红外循迹：白纸亮灯，高电平5V；黑线灭灯，低电平0V
// 三路巡迹
void TRACK_Control(void)                        
{
    TRACK_GetStatus();
	if(D1==TRACK_OFFLINE && D2== TRACK_INLINE && D3==TRACK_OFFLINE){			// 居中1|0|1
        motor_target_set(60,60);
    } else if (D1==TRACK_INLINE && D2==TRACK_OFFLINE && D3==TRACK_OFFLINE){	// 右偏大	0|11
        motor_target_set(30,60);
    } else if (D1==TRACK_OFFLINE && D2==TRACK_OFFLINE && D3==TRACK_INLINE){	// 左偏大 11|0
        motor_target_set(60,30);
    } else if (D1==TRACK_OFFLINE && D2==TRACK_OFFLINE && D3==TRACK_OFFLINE) { // 没压到线 111   
        motor_target_set(30,30);												// 偏离，但检测不到是偏哪边，减速
	} else if (D1==TRACK_INLINE && D2==TRACK_INLINE && D3==TRACK_INLINE) { // 离地 000	
		motor_target_set(0,0);
	}
}

void TRACK_Control5Way(void)                        
{
    TRACK_GetStatus();
	if(D1==TRACK_OFFLINE && D2== TRACK_INLINE && D3==TRACK_OFFLINE){			// 居中1|0|1
        motor_target_set(60,60);
    } else if (D1==TRACK_INLINE && D2==TRACK_INLINE && D3==TRACK_OFFLINE){	// 右偏小
        motor_target_set(45,60);
    } else if (D1==TRACK_INLINE && D2==TRACK_OFFLINE && D3==TRACK_OFFLINE){	// 右偏大	0|11
        motor_target_set(30,60);
    } else if (D1==TRACK_OFFLINE && D2==TRACK_INLINE && D3==TRACK_INLINE){	// 左偏小
        motor_target_set(60,45);
    } else if (D1==TRACK_OFFLINE && D2==TRACK_OFFLINE && D3==TRACK_INLINE){	// 左偏大 11|0
        motor_target_set(60,30);
    } else if (D1==TRACK_OFFLINE && D2==TRACK_OFFLINE && D3==TRACK_OFFLINE) { // 没压到线 111   
        motor_target_set(30,30);												// 偏离，但检测不到是偏哪边，减速
    }
}


/* 
 *Func: 读取寻迹模块引脚的状态（三路循迹）
 *History:
   1.Rename:Read_digital->TRACK_GetStatus 2024.8.5 
 */
void TRACK_GetStatus(void){
    D3 = GPIO_ReadInputDataBit(GPIOB, TRACK_PIN_R1);
    D2 = GPIO_ReadInputDataBit(GPIOB, TRACK_PIN_M);
    D1 = GPIO_ReadInputDataBit(GPIOB, TRACK_PIN_L1);
}


