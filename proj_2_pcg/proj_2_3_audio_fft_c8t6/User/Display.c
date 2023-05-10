/***********************************************
	OLED��ʾ
************************************************/

#include "MainHeader.h"
#include "Display.h"
#include <string.h>

#define M 128 //OLED��
#define N 64  //OLED��

u16 TIM2_NUM;
extern u8 Spectrum_flag; //ʹ�ܱ�ʶ
uint8_t Display_Buffer[M][N]; //OLED�Դ�

void Display_IRQHandler(void);
void Display_Text(void);


void Display_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIOB->ODR=0xFF00; //LED�ʵ�Ϩ��

	TIM2_Init();
	TIM2_SetIRQHandler(Display_IRQHandler);
}

/*ָ����TIM2�жϺ���*/
void Display_IRQHandler(void)
{
	uint8_t i,j,k,data,byte_data;
	TIM2_NUM++;

	if (Spectrum_flag) {
        //OLEDˢ��̫��Ļ����п���
		if(TIM2_NUM%1600==0) {
			for (i=0;i<8;i++) {
				OLED_SetPointer(0,i);
				for (j=0;j<128;j++) {
					data = 1;
					byte_data = 0;
					for (k=0;k<8;k++) {
						if(Display_Buffer[j][(7-i)*8+(7-k)]) byte_data |= data;
						data = data << 1;
					}
					OLED_WriteData(byte_data);
				}
			}
		}
	} else {
		
	}
}

void Display_Text(void)
{
	//��Ŀ LED��������Ƶ�׽����
	//���� ������
	OLED_ShowCHinese(1,1,0);
	OLED_ShowCHinese(1,2,1);
	OLED_ShowString(1, 5, " LED");
	OLED_ShowCHinese(1,5,2);
	OLED_ShowCHinese(1,6,3);
	OLED_ShowCHinese(1,7,4);
	OLED_ShowCHinese(1,8,5);
	
	OLED_ShowCHinese(2,4,6);
	OLED_ShowCHinese(2,5,7);
	OLED_ShowCHinese(2,6,8);
	OLED_ShowCHinese(2,7,9);
	OLED_ShowCHinese(2,8,10);
	
	OLED_ShowCHinese(4,1,11);
	OLED_ShowCHinese(4,2,12);
	OLED_ShowString(4, 5, " ");
	OLED_ShowCHinese(4,4,13);
	OLED_ShowCHinese(4,5,14);
	OLED_ShowCHinese(4,6,15);
}
	
/*����*/
void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color)
{
	uint8_t i;
	for(i=0;i<=Level;i++) {
		Display_Buffer[X][i]=Color;
	}
}

/*����2*/
void Display_SetLevel2(uint8_t X,uint8_t Level,uint8_t Color)
{
	uint8_t i;
	for(i=0;i<=Level;i++) {
		Display_Buffer[X][N/2+i/2]=Color;
		Display_Buffer[X][N/2-i/2]=Color;
	}
}

/*����Դ�*/
void Display_ClearBuf(void)
{
	uint8_t i,j;
	for(i=0;i<M;i++) {
		for(j=0;j<N;j++) {
			Display_Buffer[i][j]=0;
		}
	}
}
