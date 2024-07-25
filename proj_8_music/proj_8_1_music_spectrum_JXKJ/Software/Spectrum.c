#include "stm32f10x.h"
#include "Sample.h"
#include "FFT.h"
#include "Display.h"
#include "AT24C02.h"
#include "Delay.h"

uint8_t Spectrum_Type;
uint8_t Spectrum_Level[64];
uint16_t Spectrum_Dot[64];

uint16_t Spectrum_Level1[16];
uint16_t Spectrum_Dot1[16];

uint8_t Spectrum_SoundFlag;
uint8_t Spectrum_StopFlag=1;

uint16_t Spectrum_Count;
uint8_t Spectrum_ModeSwitchFlag=0;
uint8_t Spectrum_ModeOutFlag=0;


void Spectrum_GetValue(void);

void Spectrum_Init(void)
{
	Spectrum_Type=AT24C02_ReadByte(2);
	if(Spectrum_Type>15){Spectrum_Type=0;}
	Spectrum_ModeSwitchFlag=AT24C02_ReadByte(3);
	if(Spectrum_ModeSwitchFlag>1){Spectrum_ModeSwitchFlag=0;}
	Sample_Init();
	FFT_Init();
	Spectrum_GetValue();
	Spectrum_Count=100;
	Spectrum_ModeOutFlag=0;
}

void Spectrum_GetValue(void)
{
	uint8_t i;
	uint32_t Sum=0;
	Sample_GetValue();
	for(i=0;i<128;i++)
	{
		FFT_Input[i]=Sample_Value[i]-2048;
	}
	FFT();
	for(i=0;i<64;i++)
	{
		Spectrum_Level[i]=FFT_Output[i]/64;
		if(i!=0){Sum+=Spectrum_Level[i];}
	}
	Spectrum_Level[0]=(Spectrum_Level[1]+Spectrum_Level[2])/2;
	Display_ClearBuf();
	if(Sum>120)
	{
		Spectrum_SoundFlag=1;
	}
	else
	{
		Spectrum_SoundFlag=0;
	}
}

uint8_t Spectrum_GetModeInFlag(void)
{
	if(Spectrum_ModeSwitchFlag)
	{
		Spectrum_GetValue();
		if(Spectrum_SoundFlag)
		{
			Spectrum_StopFlag=0;
			Spectrum_ModeOutFlag=0;
			Spectrum_Count=0;
			return 1;
		}
	}
	return 0;
}	

uint8_t Spectrum_GetModeOutFlag(void)
{
	if(Spectrum_ModeSwitchFlag)
	{
		return Spectrum_ModeOutFlag;
	}
	return 0;
}

void Spectrum_AutoSwitchChange(void)
{
	Spectrum_ModeSwitchFlag=!Spectrum_ModeSwitchFlag;
	AT24C02_WriteByte(3,Spectrum_ModeSwitchFlag);
}	

void Spectrum_AutoSwitchShow(void)
{
	Display_ShowString(8,0,"AutoSw",0x17);
	if(Spectrum_ModeSwitchFlag)
	{
		Display_ShowString(16,16," ON ",0x12);
	}
	else
	{
		Display_ShowString(13,16," OFF ",0x14);
	}
	Display_Update();	
}

void Spectrum_Show(void)
{
	uint8_t i;
	if(Spectrum_SoundFlag==0)
	{
		Spectrum_Count++;
		if(Spectrum_Count>200)
		{
			Spectrum_StopFlag=1;
		}
		if(Spectrum_Count>1000)
		{
			Spectrum_Count=1000;
			Spectrum_ModeOutFlag=1;
		}
	}
	else
	{
		Spectrum_Count=0;
		Spectrum_StopFlag=0;
		Spectrum_ModeOutFlag=0;
	}

	if(Spectrum_Type<=3)
	{
		for(i=0;i<64;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>30)Spectrum_Level[i]=30;
			if(Spectrum_Dot[i]/10<Spectrum_Level[i]+1)
			{
				Spectrum_Dot[i]=(Spectrum_Level[i]+1)*10;
			}
			else if(Spectrum_Dot[i]/10>1)
			{
				Spectrum_Dot[i]-=4;
			}
			if(Spectrum_Type==0)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x01);
				Display_ShowPoint(i,31-Spectrum_Dot[i]/10,0x02);
			}
			else if(Spectrum_Type==1)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x03);
				Display_ShowPoint(i,31-Spectrum_Dot[i]/10,0x06);
			}
			else if(Spectrum_Type==2)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x05);
				Display_ShowPoint(i,31-Spectrum_Dot[i]/10,0x07);
			}
			else if(Spectrum_Type==3)
			{
				Display_SetLevel(i,Spectrum_Level[i],i%7+1);
				Display_ShowPoint(i,31-Spectrum_Dot[i]/10,i%7+1);
			}
		}
	}
	else if(Spectrum_Type<=7)
	{
		for(i=0;i<64;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>31)Spectrum_Level[i]=31;
			if(Spectrum_Type==4)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x01);
			}
			else if(Spectrum_Type==5)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x03);
			}
			else if(Spectrum_Type==6)
			{
				Display_SetLevel(i,Spectrum_Level[i],0x05);
			}
			else if(Spectrum_Type==7)
			{
				Display_SetLevel(i,Spectrum_Level[i],i%7+1);
			}
		}
	}
	else if(Spectrum_Type<=11)
	{
		for(i=0;i<64;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>30)Spectrum_Level[i]=30;
			if(i%4==3)
			{
				Spectrum_Level1[i/4]=(Spectrum_Level[i]+Spectrum_Level[i-1]+Spectrum_Level[i-2]+Spectrum_Level[i-3])/3;
				if(Spectrum_Level1[i/4]>30)Spectrum_Level1[i/4]=30;
				if(Spectrum_Dot1[i/4]/10<Spectrum_Level1[i/4]+1)
				{
					Spectrum_Dot1[i/4]=(Spectrum_Level1[i/4]+1)*10;
				}
				else if(Spectrum_Dot1[i/4]/10>1)
				{
					Spectrum_Dot1[i/4]-=3;
				}
				if(Spectrum_Type==8)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x01);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x01);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x01);
					Display_ShowPoint(i-1,31-Spectrum_Dot1[i/4]/10,0x02);
					Display_ShowPoint(i-2,31-Spectrum_Dot1[i/4]/10,0x02);
					Display_ShowPoint(i-3,31-Spectrum_Dot1[i/4]/10,0x02);
				}
				else if(Spectrum_Type==9)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x03);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x03);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x03);
					Display_ShowPoint(i-1,31-Spectrum_Dot1[i/4]/10,0x06);
					Display_ShowPoint(i-2,31-Spectrum_Dot1[i/4]/10,0x06);
					Display_ShowPoint(i-3,31-Spectrum_Dot1[i/4]/10,0x06);
				}
				else if(Spectrum_Type==10)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x05);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x05);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x05);
					Display_ShowPoint(i-1,31-Spectrum_Dot1[i/4]/10,0x07);
					Display_ShowPoint(i-2,31-Spectrum_Dot1[i/4]/10,0x07);
					Display_ShowPoint(i-3,31-Spectrum_Dot1[i/4]/10,0x07);
				}
				else if(Spectrum_Type==11)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],i/4%7+1);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],i/4%7+1);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],i/4%7+1);
					Display_ShowPoint(i-1,31-Spectrum_Dot1[i/4]/10,i/4%7+1);
					Display_ShowPoint(i-2,31-Spectrum_Dot1[i/4]/10,i/4%7+1);
					Display_ShowPoint(i-3,31-Spectrum_Dot1[i/4]/10,i/4%7+1);
				}
			}
		}
	}
	else if(Spectrum_Type<=15)
	{
		for(i=0;i<64;i++)
		{
			if(Spectrum_StopFlag)Spectrum_Level[i]=0;
			if(Spectrum_Level[i]>31)Spectrum_Level[i]=31;
			if(i%4==3)
			{
				Spectrum_Level1[i/4]=(Spectrum_Level[i]+Spectrum_Level[i-1]+Spectrum_Level[i-2]+Spectrum_Level[i-3])/3;
				if(Spectrum_Level1[i/4]>31)Spectrum_Level1[i/4]=31;
				if(Spectrum_Type==12)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x01);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x01);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x01);
				}
				else if(Spectrum_Type==13)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x03);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x03);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x03);
				}
				else if(Spectrum_Type==14)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],0x05);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],0x05);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],0x05);
				}
				else if(Spectrum_Type==15)
				{
					Display_SetLevel(i-1,Spectrum_Level1[i/4],i/4%4+1);
					Display_SetLevel(i-2,Spectrum_Level1[i/4],i/4%4+1);
					Display_SetLevel(i-3,Spectrum_Level1[i/4],i/4%4+1);
				}
			}
		}
	}
	Display_Update();
}

void Spectrum_ChangeType(void)
{
	Spectrum_Type++;
	Spectrum_Type%=16;
	AT24C02_WriteByte(2,Spectrum_Type);
}
