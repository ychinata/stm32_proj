/**********************************************************************************
 * 程序名称:	LED点阵屏显示[极简版]
 * 程序作者:	HGD/B站[曹同學]
 * 创建时间:	2019.07.28
 * 最后修改:	2022.03.12
 * 开源协议:	GPL3.0
 * 共享请保留此头部信息
***********************************************************************************/
#include "MainHeader.h"
#include "LedFont.h"
#include <string.h>

#define LED(x)	GPIO_WriteBit(GPIOC,GPIO_Pin_13,(BitAction)(x))

#define LED_Width 128
#define LED_Height 64

u16 TIM2_NUM;

extern uint16_t Display_Brightness;

uint8_t LED_Buffer[LED_Width][LED_Height];
//uint8_t LED_ScanBuf[LED_Width][LED_Height];

void Display_ShowPicture_Single(uint8_t X,uint8_t Y,uint8_t W,uint8_t H,uint8_t Color);

uint16_t time;

void Display_ShowPoint(uint16_t x,uint16_t y,uint8_t color);
void Display_IRQHandler(void);

void Display_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,(BitAction)(1));
	
	TIM2_Init();
	TIM2_SetIRQHandler(Display_IRQHandler);
}	
void Display_IRQHandler(void)
{
	TIM2_NUM++;
	
	Display();
}

void Display(void)
{
	uint8_t i,j,k,X=0,Y=0,data,byte_data;
	static uint16_t a=0,flag;
	a++;
	if(a%1600==0) //OLED刷新不能太快
	{
		for(i=0;i<8;i++)
		{
			OLED_SetPointer(0,i);
			for(j=0;j<128;j++)
			{
				data = 1;
				byte_data = 0;
				for(k=0;k<8;k++)
				{
					if(LED_Buffer[j][(7-i)*8+(7-k)]) byte_data |= data;
					data = data << 1;
				}
				OLED_WriteData(byte_data);
			}
		}


		LED(flag);
		flag=!flag;
	}
}

void Display_Update(void)
{
//	uint8_t i,j;
//	for(i=0;i<64;i++)
//	{
//		for(j=0;j<32;j++)
//		{
//			LED_ScanBuf[i][j]=LED_Buffer[i][j];
//		}
//	}
}

uint32_t Display_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}

/*竖线*/
void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color)
{
	uint8_t i;
	for(i=0;i<=Level;i++)
	{
		LED_Buffer[X][i]=Color;
	}
}
/*竖线*/
void Display_SetLevel2(uint8_t X,uint8_t Level,uint8_t Color)
{
	uint8_t i;
	for(i=0;i<=Level;i++)
	{
		LED_Buffer[X][LED_Height/2+i/2]=Color;
		LED_Buffer[X][LED_Height/2-i/2]=Color;
	}
}
void Display_ShowChar(uint8_t X,uint8_t Y,char Char,uint8_t Color)
{
	uint8_t i,j;

		for(i=0;i<8;i++)
		{
			for(j=0;j<16;j++)
			{
				/*将字库数据(字库数据是躺着放的)按列填充到Display_Buf[][]*/
				LED_Buffer[X+i][LED_Height-1-(Y+j)]=(Disp8x16[Char-' '][i+j/8*8]&(0x01<<(j%8)))?Color:0x00;
			}
		}

}
void Display_ShowString(int X,int Y,char *String,uint8_t Color)
{
	uint8_t i,j,k;

	for(i=0;String[i]!='\0';i++)//12
	{
//		LED_ShowChar(X+8*i,Y,String[i],Color);
		Display_ShowChar(X+8*i,Y,String[i],Color);
	}	
}
void Display_ShowNumber(uint8_t X,uint8_t Y,uint16_t Number,uint8_t Length,uint8_t Color)
{
	uint8_t i;

	for(i=Length;i>0;i--)
	{

			Display_ShowChar(X+8*(Length-i),Y,Number/Display_Pow(10,i-1)%10+'0',Color);

	}
}

void Display_ClearBuf(void)
{
	uint8_t i,j;
	for(i=0;i<LED_Width;i++)
	{
		for(j=0;j<LED_Height;j++)
		{
			LED_Buffer[i][j]=0;
		}
	}
}

void Display_ShowPoint(uint16_t X,uint16_t Y,uint8_t Color)
{
	LED_Buffer[X][LED_Height-1-Y]=Color;
}

void Display_ShowPicture_Single(uint8_t X,uint8_t Y,uint8_t W,uint8_t H,uint8_t Color)
{
	uint8_t i,j;
	if(W>LED_Width) W=LED_Width;
	if(H>LED_Height) H=LED_Height;
	
	for(i=0;i<H;i++)//32
	{
		for(j=0;j<W;j++)//64
		{
			LED_Buffer[W-1-j+X][H-1-i+Y]=(gImage_2[i][7-(j)/8]&(0x01<<(j)%8))?Color:0x00;
		}
	}
		
}
