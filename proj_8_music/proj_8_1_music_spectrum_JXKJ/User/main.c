#include "MainHeader.h"

/* 引脚分配：

串口-PA9/10

*/

void Init(void)
{
	RCC_Init();
	NVIC_GroupConfig();

	Serial_Init();    
    //printf("Init end.\r\n");	
    Serial_Printf("hello usart:%d\r\n", 444);
    
	Key_Init();
	//AT24C02_Init();
	Light_Init();
	Spectrum_Init();
	//DS3231_Init();
	//BrightnessSetInit();
	Display_Init();
	TIM4_SetIRQHandler(MainLoop);
	TIM4_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "FFT DEMO");
    printf("Init end2.\r\n");	
}

int main(void)
{
	Init();
	if (Key_GetKeyNum()==1) {
		SystemTest();
	}
	while(1) {
		SpectrumShow();
	/*
		KeyNum=Key_GetKeyNumber();
		switch(Mode)
		{
			case 10:TimeShow();break;
			case 11:TimeSet();break;
			case 20:SpectrumShow();break;
			case 21:SpectrumAutoSwitch();break;
			case 30:BrightnessSet();break;
		}
		*/
		//Global();
		//ModeChange();
	}
}

/*系统测试*/
void SystemTest(void)
{
	uint8_t i,j,KeyNum,TestMode=1,AT24C02Flag;
	AT24C02_WriteByte(10,0xAA);
	AT24C02_WriteByte(11,0x55);
	if(AT24C02_ReadByte(10)==0xAA && AT24C02_ReadByte(11)==0x55)
	{
		AT24C02Flag=1;
	}
	else
	{
		AT24C02Flag=0;
	}
	Display_SetBrightness(100);
	while(1)
	{
		KeyNum=Key_GetKeyNum();
		if(KeyNum==1)
		{
			TestMode++;
			if(TestMode>7){TestMode=1;}
		}
		else if(KeyNum==2)
		{
			TestMode=8;
		}
		if(TestMode<8)
		{
			for(i=0;i<64;i++)
			{
				for(j=0;j<32;j++)
				{
					Display_ShowPoint(i,j,TestMode);
				}
			}
			Display_Update();
		}
		else
		{
			Display_ClearBuf();
			Sample_GetValue();
			DS3231_ReadTime();
			Display_ShowString(0,0,"T:  :  :",0x07);
			Display_ShowString(0,8,"L:",0x07);
			Display_ShowString(0,16,"S:",0x07);
			if(AT24C02Flag==1)
			{
				Display_ShowString(0,24,"D:OK ",0x07);
			}
			else
			{
				Display_ShowString(0,24,"D:ERR",0x07);
			}
			Display_ShowNumber(2*6,0,DS3231_Time[3],2,0x07);
			Display_ShowNumber(5*6,0,DS3231_Time[4],2,0x07);
			Display_ShowNumber(8*6,0,DS3231_Time[5],2,0x07);
			Display_ShowNumber(2*6,8,Light_GetValue(),4,0x07);
			Display_ShowNumber(2*6,16,Sample_Value[0],4,0x07);
			Display_Update();
		}
	}
}

/*模式切换*/
void ModeChange(void)
{
	static uint8_t ModeChangeFlag=0,Step=0;
	if(ModeNext!=Mode)
	{
		ModeChangeFlag=1;
	}
	if(ModeChangeFlag)
	{
		if(Step==0)
		{
			ModeStopFlag=1;
			Step=1;
		}
		else if(Step==1)
		{
			ModeStopFlag=0;
			ModeStartFlag=1;
			ModeLast=Mode;
			Mode=ModeNext;
			Step=2;
		}
		else if(Step==2)
		{
			ModeStartFlag=0;
			Step=0;
			ModeChangeFlag=0;
		}
	}
}

/*全局循环*/
void Global(void)
{
	if(Brightness==0)
	{
		LightValue=Light_GetValue();
		if(LightValue<600){LightValue=0;}
		else {LightValue-=600;}
		Display_SetBrightness(LightValue/20+1);
	}
	else
	{
		Display_SetBrightness(Brightness*18-17);
	}
}

/*亮度设置模式*/
void BrightnessSetInit(void)
{
	Brightness=AT24C02_ReadByte(1);
	if(Brightness>9){Brightness=0;}
}

void BrightnessSet(void)
{
	if(KeyNum==KEY2_LONG_PRESSED)
	{
		ModeNext=ModeLast;
	}
	if(KeyNum==KEY1_PRESSED)
	{
		if(Brightness<9)Brightness++;
		AT24C02_WriteByte(1,Brightness);
	}
	if(KeyNum==KEY2_PRESSED)
	{
		if(Brightness>0)Brightness--;
		AT24C02_WriteByte(1,Brightness);
	}
	if(ModeStartFlag)
	{
		Display_Clear();
		Display_ShowString(4,0," Light ",0x17);
	}
	if(Brightness)
	{
		Display_ShowNumber(32-4,16,Brightness,1,0x14);
	}
	else
	{
		Display_ShowString(32-4,16,"A",0x12);
	}
	Display_Update();
}

/*时间模式*/
void TimeShow(void)
{
	if(ModeStartFlag)
	{
		Clock_Init();
	}
	if(KeyNum==KEY1_PRESSED)
	{
		ModeNext=20;
	}
	if(KeyNum==KEY2_PRESSED)
	{
		Clock_ChangeColor();
	}
	if(KeyNum==KEY1_LONG_PRESSED)
	{
		ModeNext=11;
	}
	if(KeyNum==KEY2_LONG_PRESSED)
	{
		ModeNext=30;
	}
	if(Spectrum_GetModeInFlag())
	{
		ModeNext=20;
	}
	DS3231_ReadTime();
	Clock_Display();
}

/*时间设置模式*/
void TimeSet(void)
{
	if(ModeStartFlag)
	{
		Clock_Init();
	}
	if(ModeStopFlag)
	{
		if(TimeSet_Select!=0){DS3231_SetTime();}
		TimeSet_Select=0;
	}
	if(KeyNum==KEY1_LONG_PRESSED)
	{
		ModeNext=10;
	}
	if(KeyNum==KEY1_PRESSED)
	{
		TimeSet_Select++;
		TimeSet_Select%=6;
	}
	if(KeyNum==KEY2_PRESSED || KeyNum==KEY2_LONG_PRESSED_COUNT)
	{
		Clock_TimeSetAdd(TimeSet_Select);
	}
	Clock_TimeSetDisplay(TimeSet_Select);
}

/*频谱模式*/
void SpectrumShow(void)
{
	if(ModeStartFlag)
	{
		Spectrum_Init();
	}
	if(KeyNum==KEY1_PRESSED)
	{
		ModeNext=10;
	}
	if(KeyNum==KEY2_PRESSED)
	{
		Spectrum_ChangeType();
	}
	if(KeyNum==KEY1_LONG_PRESSED)
	{
		ModeNext=21;
	}
	if(KeyNum==KEY2_LONG_PRESSED)
	{
		ModeNext=30;
	}
	if(Spectrum_GetModeOutFlag())
	{
		ModeNext=10;
	}
	Spectrum_GetValue();
	Spectrum_Show();
	Delay_ms(100);
}

/*自动切换*/
void SpectrumAutoSwitch(void)
{
	if(ModeStartFlag)
	{
		Display_Clear();
	}
	if(KeyNum==KEY1_LONG_PRESSED)
	{
		ModeNext=20;
	}
	if(KeyNum==KEY2_PRESSED)
	{
		Spectrum_AutoSwitchChange();
	}
	Spectrum_AutoSwitchShow();
}

/*主程序中断*/
void MainLoop(void)
{
	static uint8_t LoopCount[3]={0,1,2};
	if(LoopCount[0]++>=2)
	{
		LoopCount[0]=0;
		Key_Loop();
	}
	if(LoopCount[1]++>=2)
	{
		LoopCount[1]=0;
		Light_Loop();
	}
	if(LoopCount[2]++>=20)
	{
		LoopCount[2]=0;
		Clock_Loop();
	}
	if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update))
	{
		flag=1;
	}
}

