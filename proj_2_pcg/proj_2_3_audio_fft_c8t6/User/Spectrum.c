/*************************************************************
根据傅里叶输出写入显存

频谱显示思路是将音频的时域经快速傅里叶计算转化为频域，
再将各频率量化的值，以竖着柱状线的形式填充进显存。
经OLED显示出显存画面就是看到的频谱显示。
**************************************************************/


#include "MainHeader.h"
#include "FFT.h"

#define M	128
#define N	64
// 可调整参数
//可以控制柱状幅度,原始值为64
//MAX9814为512,max4466更差. LM393为16
#define SPECTRUM_LEVEL_DIV 512   
// MAX9814为512是因为底噪太大?

#define ADC_VALUE_DC_OFFSET 1550      //ADC采样值的直流偏移?初始值为2048. 1550对应1.25V(3.3V)

enum SpectrumType {
    SPEC_TYPE_HIST_THIN = 0, 
    SPEC_TYPE_HIST_THICK,
    SPEC_TYPE_PLOT
};

uint8_t Spectrum_Type=0; //频谱样式
uint8_t Spectrum_Level[M]; //柱长度

uint16_t Spectrum_Level2[M/2]; ////柱长度2

uint8_t Spectrum_SoundFlag; //有声音标识
uint8_t Spectrum_StopFlag=1; //停止标识
uint16_t Spectrum_Count; //一个计数

double Spectrum_Average; //频谱平均值，用来控制彩灯依据
extern u16 TIM2_NUM; //定时器2计数
	
extern uint16_t DMA1_ADCValue[M*2]; //ADC采样数组

void Spectrum_GetValue(void);

void Spectrum_Init(void)
{
	FFT_Init();
	Spectrum_GetValue();
	Spectrum_Count=100;
}

/*获取频谱值*/
void Spectrum_GetValue(void)
{
	uint16_t i;
	uint32_t Sum=0;

	for(i=0;i<M*2;i++)
	{
		FFT_Input[i]=DMA1_ADCValue[i]-ADC_VALUE_DC_OFFSET;
	}
	FFT();
	for(i=0;i<M;i++)
	{
		Spectrum_Level[i]=FFT_Output[i]/SPECTRUM_LEVEL_DIV;
		if(i!=0){Sum+=Spectrum_Level[i];}
	}
	Spectrum_Average=Sum/M;
	Spectrum_Level[0]=(Spectrum_Level[1]+Spectrum_Level[2])/2;  // ?
	Display_ClearBuf();
	if (Sum>120) {
		Spectrum_SoundFlag=1;
	} else {
		Spectrum_SoundFlag=0;
	}
}

/*将频谱值填充显存*/
void Spectrum_Show(void)
{
	uint16_t i;
	
	if(Spectrum_SoundFlag==0){
		Spectrum_Count++;
		if(Spectrum_Count>200)
		{
			Spectrum_StopFlag=1;
		}
		if(Spectrum_Count>1000)
		{
			Spectrum_Count=1000;
		}
	}else{
		Spectrum_Count=0;
		Spectrum_StopFlag=0;
	}

	if(Spectrum_Type==0)
	{
		for(i=1;i<=M;i++)
		{
			if(Spectrum_StopFlag)
                Spectrum_Level[i]=0;
            //超过最大显示范围
			if(Spectrum_Level[i]>(N-1))
                Spectrum_Level[i]=(N-1);
            //画竖线
			Display_SetLevel(i,Spectrum_Level[i],0x07);
		}
		
		Spectrum_Average/=2;
		if(Spectrum_Average>8)
            Spectrum_Average=8;
        //控制8个LED彩灯,分成8个阶梯
		switch((u8)Spectrum_Average) {
			case 0:GPIOB->ODR|=0xFF00;break;                    // 0b1111,1111 // 全灭
			case 1:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFEFF;break; // 0b1111,1110
			case 2:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFCFF;break; // 0b1111,1100
			case 3:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xF8FF;break; // 0b1111,1000
			case 4:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xF0FF;break; // 0b1111,0000
			case 5:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xE0FF;break; // 0b1110,0000
			case 6:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xC0FF;break; // 0b1100,0000
			case 7:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0x80FF;break; // 0b1000,0000
			case 8:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0x00FF;break; // 0b0000,0000 // 全亮
		}
	}
	if(Spectrum_Type==1)
	{
		for(i=1;i<=M;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>(N-1))Spectrum_Level[i]=(N-1);
			if(i%4==0)
			{
				Spectrum_Level2[i/4]=(Spectrum_Level[i]+Spectrum_Level[i-1]+Spectrum_Level[i-2]+Spectrum_Level[i-3])/4;
				if(Spectrum_Level2[i/4]>(N-1))Spectrum_Level2[i/4]=(N-1);

				Display_SetLevel(i,Spectrum_Level2[i/4],0x07);
				Display_SetLevel((i-1),Spectrum_Level2[i/4],0x07);
				Display_SetLevel((i-2),Spectrum_Level2[i/4],0x07);
//				Display_SetLevel((i-3),Spectrum_Level2[i/4],0x07);
			}
		}
		
		Spectrum_Average/=2;
		if(Spectrum_Average>8)Spectrum_Average=8;
		switch((u8)Spectrum_Average)
		{
			case 0:GPIOB->ODR|=0xFF00;break;
			case 1:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFEFF;break;
			case 2:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFCFF;break;
			case 3:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xF8FF;break;
			case 4:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xF0FF;break;
			case 5:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xE0FF;break;
			case 6:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xC0FF;break;
			case 7:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0x80FF;break;
			case 8:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0x00FF;break;
		}
	}
	if(Spectrum_Type==2)
	{
		for(i=0;i<M;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>(N-1))Spectrum_Level[i]=(N-1);

			Display_SetLevel2(i,Spectrum_Level[i],0x07);
		}
		
		Spectrum_Average/=2;
		if(Spectrum_Average>8)Spectrum_Average=8;
		switch((u8)Spectrum_Average)
		{
			case 1:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFEFF;break;
			case 2:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFDFF;break;
			case 3:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xFBFF;break;
			case 4:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xF7FF;break;
			case 5:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xEFFF;break;
			case 6:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xDFFF;break;
			case 7:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0xBFFF;break;
			case 8:GPIOB->ODR|=0xFF00;GPIOB->ODR&=0x7FFF;break;
		}
	}	
}
