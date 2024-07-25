#include "stm32f10x.h"
#include "TIM3.h"
#include "DisplayFont.h"

uint8_t Display_Buf[64][32];
uint8_t Display_ScanBuf[64][32];
uint16_t Display_Brightness;
uint8_t Display_EnableFlag=1;

/*底层：******************************************************************/
void Display_IRQHandler(void);

#define Display_R1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)(x))
#define Display_G1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)(x))
#define Display_B1(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)(x))
#define Display_R2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)(x))
#define Display_G2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)(x))
#define Display_B2(x)	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)(x))
#define Display_A(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)(x))
#define Display_B(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_1,(BitAction)(x))
#define Display_C(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)(x))
#define Display_D(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)(x))
#define Display_CLK(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)(x))
#define Display_LAT(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)(x))
#define Display_OE(x)	GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)(x))

void Display_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	Display_OE(1);
	TIM3_Init();
	TIM3_SetIRQHandler(Display_IRQHandler);
}	

void Display_IRQHandler(void)
{
	static uint8_t i,j,Flag,Gray;
	Flag=!Flag;
	if(Flag && Display_Brightness)
	{
		Gray=(j>>1)^j;
		Display_D(Gray&0x8);
		Display_C(Gray&0x4);
		Display_B(Gray&0x2);
		Display_A(Gray&0x1);
		for(i=0;i<64;i++)
		{
			Display_R1(Display_ScanBuf[i][31-Gray]&0x1);
			Display_G1(Display_ScanBuf[i][31-Gray]&0x2);
			Display_B1(Display_ScanBuf[i][31-Gray]&0x4);
			Display_R2(Display_ScanBuf[i][15-Gray]&0x1);
			Display_G2(Display_ScanBuf[i][15-Gray]&0x2);
			Display_B2(Display_ScanBuf[i][15-Gray]&0x4);
			Display_CLK(1);
			Display_CLK(0);
		}
		Display_LAT(1);
		Display_LAT(0);
		if(Display_EnableFlag)Display_OE(0);
		j++;
		j%=16;
		TIM3_SetPeriod(Display_Brightness+1);
	}
	else
	{
		Display_OE(1);
		TIM3_SetPeriod(150-Display_Brightness+2);
	}
}

/*用户层：******************************************************************/
/**
  * @brief  设置亮度
  * @param  Brightness 亮度，范围：0~150
  * @retval 无
  */
void Display_SetBrightness(uint16_t Brightness)
{
	if(Brightness>150)Brightness=150;
	Display_Brightness=Brightness;
	if(Brightness==0)Display_OE(1);
}

void Display_Enable(uint8_t EnableFlag)
{
	if(EnableFlag==0)
	{
		Display_OE(1);
	}
	Display_EnableFlag=EnableFlag;
}

void Display_ClearBuf(void)
{
	uint8_t i,j;
	for(i=0;i<64;i++)
	{
		for(j=0;j<32;j++)
		{
			Display_Buf[i][j]=0;
		}
	}
}

void Display_ClearAreaBuf(uint8_t StartX,uint8_t EndX,uint8_t StartY,uint8_t EndY)
{
	uint8_t i,j;
	for(i=StartX;i<EndX;i++)
	{
		for(j=StartY;j<EndY;j++)
		{
			Display_Buf[i][31-j]=0;
		}
	}
}

void Display_Update(void)
{
	uint8_t i,j;
	for(i=0;i<64;i++)
	{
		for(j=0;j<32;j++)
		{
			Display_ScanBuf[i][j]=Display_Buf[i][j];
		}
	}
}

void Display_SetLevel(uint8_t X,uint8_t Level,uint8_t Color)
{
	uint8_t i;
	for(i=0;i<=Level;i++)
	{
		Display_Buf[X][i]=Color;
	}
}

void Display_Clear(void)
{
	Display_ClearBuf();
	Display_Update();
}

void Display_ShowChar(uint8_t X,uint8_t Y,char Char,uint8_t SizeColor)
{
	uint8_t i,j,Size,Color;
	Size=SizeColor/16;
	Color=SizeColor%16;
	if(Size)
	{
		for(i=0;i<8;i++)
		{
			for(j=0;j<16;j++)
			{
				Display_Buf[X+i][31-(Y+j)]=(Disp8x16[Char-' '][i+j/8*8]&(0x01<<(j%8)))?Color:0x00;
			}
		}
	}
	else
	{
		for(i=0;i<6;i++)
		{
			for(j=0;j<8;j++)
			{
				Display_Buf[X+i][31-(Y+j)]=(Disp6x8[Char-' '][i]&(0x01<<j))?Color:0x00;
			}
		}
	}
}

void Display_ShowString(uint8_t X,uint8_t Y,char *String,uint8_t SizeColor)
{
	uint8_t i,Size;
	Size=SizeColor/16;
	for(i=0;String[i]!='\0';i++)
	{
		if(Size)
		{
			Display_ShowChar(X+8*i,Y,String[i],SizeColor);
		}
		else
		{
			Display_ShowChar(X+6*i,Y,String[i],SizeColor);
		}
	}
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

void Display_ShowNumber(uint8_t X,uint8_t Y,uint16_t Number,uint8_t Length,uint8_t SizeColor)
{
	uint8_t i,Size;
	Size=SizeColor/16;
	for(i=Length;i>0;i--)
	{
		if(Size)
		{
			Display_ShowChar(X+8*(Length-i),Y,Number/Display_Pow(10,i-1)%10+'0',SizeColor);
		}
		else
		{
			Display_ShowChar(X+6*(Length-i),Y,Number/Display_Pow(10,i-1)%10+'0',SizeColor);
		}
	}
}

void Display_ShowPoint(uint8_t X,uint8_t Y,uint8_t Color)
{
	Display_Buf[X][31-Y]=Color;
}
