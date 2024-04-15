#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "stm32f1xx.h"

//#define K1   HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)   ���ذ������ⲿ�ж�
//#define K1_PRES  1


#define KEY1   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)
#define KEY2   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)
#define KEY3   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)

#define KEY1_PRES 1
#define KEY2_PRES 2
#define KEY3_PRES 3

#define K1_DOWN_LEVEL 0

typedef enum
{
  KEY0_UP   = 0,
  KEY0_DOWN = 1,
}KEYState_TypeDef;
 
extern char g_nButton;   //�����ⲿ���������������ط����á���ʹ�ö�ʱ��������ʱ�������,�����¼����Ч������ʲô��
extern int iButtonFlag;  //��¼ȷʵ���������£�����Ч

void KEY_GPIO_Init(void);
uint8_t KEY_Scan(void);

void ButtonScan(void);


KEYState_TypeDef K1_StateRead(void);

#endif



