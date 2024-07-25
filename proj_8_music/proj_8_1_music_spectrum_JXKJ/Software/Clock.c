#include "stm32f10x.h"
#include "Display.h"
#include "DS3231.h"
#include "AT24C02.h"
#include "Delay.h"

uint8_t Clock_FlashFlag;

uint8_t Clock_ColorSelect;
uint8_t Clock_Color[][4]=
{
	7,7,7,7,
	1,1,1,1,
	2,2,2,2,
	3,3,3,3,
	4,4,4,4,
	5,5,5,5,
	6,6,6,6,
	1,2,3,4,
	3,5,2,6,
	2,1,4,7,
	6,5,3,1,
	1,2,4,5,
};

void Clock_Init(void)
{
	DS3231_Init();
	Clock_ColorSelect=AT24C02_ReadByte(0);
	if(Clock_ColorSelect>11){Clock_ColorSelect=0;}
	Display_ClearBuf();
}

void Clock_Display(void)
{
	uint8_t i;
	Display_ShowString(1,4,"20  -  -  ",Clock_Color[Clock_ColorSelect][0]);
	Display_ShowString(2*8+4,12,":",0x10|Clock_Color[Clock_ColorSelect][1]);
	Display_ShowNumber(2*6+1,4,DS3231_Time[0],2,Clock_Color[Clock_ColorSelect][0]);
	Display_ShowNumber(5*6+1,4,DS3231_Time[1],2,Clock_Color[Clock_ColorSelect][0]);
	Display_ShowNumber(8*6+1,4,DS3231_Time[2],2,Clock_Color[Clock_ColorSelect][0]);
	Display_ShowNumber(0+4,13,DS3231_Time[3],2,0x10|Clock_Color[Clock_ColorSelect][1]);
	Display_ShowNumber(3*8+4,13,DS3231_Time[4],2,0x10|Clock_Color[Clock_ColorSelect][1]);
	Display_ShowNumber(6*8+1,17,DS3231_Time[5],2,Clock_Color[Clock_ColorSelect][2]);
	Display_ClearAreaBuf(51,60,26,27);
	for(i=51;i<51+DS3231_Time[5]%10;i++){Display_ShowPoint(i,26,Clock_Color[Clock_ColorSelect][3]);}
	Display_Update();
}

void Clock_TimeSetAdd(uint8_t Select)
{
	DS3231_Time[Select]++;
	if(Select==0){if(DS3231_Time[0]>99)DS3231_Time[0]=0;}
	if(Select==1){if(DS3231_Time[1]>12)DS3231_Time[1]=1;}
	if(Select==0 || Select==1 || Select==2){
		if(DS3231_Time[1]==1 || DS3231_Time[1]==3 || DS3231_Time[1]==5 || 
		   DS3231_Time[1]==7 || DS3231_Time[1]==8 || DS3231_Time[1]==10 || 
		   DS3231_Time[1]==12)
		{
			if(DS3231_Time[2]>31)DS3231_Time[2]=1;
		}
		if(DS3231_Time[1]==4 || DS3231_Time[1]==6 || 
		   DS3231_Time[1]==9 || DS3231_Time[1]==11)
		{
			if(DS3231_Time[2]>30)DS3231_Time[2]=1;
		}
		if(DS3231_Time[1]==2)
		{
			if(DS3231_Time[0]%4==0){if(DS3231_Time[2]>29)DS3231_Time[2]=1;}
			else {if(DS3231_Time[2]>28)DS3231_Time[2]=1;}
		}
	}
	if(Select==3){if(DS3231_Time[3]>23)DS3231_Time[3]=0;}
	if(Select==4){if(DS3231_Time[4]>59)DS3231_Time[4]=0;}
	if(Select==5){if(DS3231_Time[5]>59)DS3231_Time[5]=0;}
}

void Clock_TimeSetDisplay(uint8_t Select)
{
	uint8_t i;
	Display_ShowString(1,4,"20  -  -  ",Clock_Color[Clock_ColorSelect][0]);
	Display_ShowString(2*8+4,12,":",0x10|Clock_Color[Clock_ColorSelect][1]);
	if(Select==0 && Clock_FlashFlag)Display_ShowString(2*6+1,4,"  ",Clock_Color[Clock_ColorSelect][0]);
	else Display_ShowNumber(2*6+1,4,DS3231_Time[0],2,Clock_Color[Clock_ColorSelect][0]);
	if(Select==1 && Clock_FlashFlag)Display_ShowString(5*6+1,4,"  ",Clock_Color[Clock_ColorSelect][0]);
	else Display_ShowNumber(5*6+1,4,DS3231_Time[1],2,Clock_Color[Clock_ColorSelect][0]);
	if(Select==2 && Clock_FlashFlag)Display_ShowString(8*6+1,4,"  ",Clock_Color[Clock_ColorSelect][0]);
	else Display_ShowNumber(8*6+1,4,DS3231_Time[2],2,Clock_Color[Clock_ColorSelect][0]);
	if(Select==3 && Clock_FlashFlag)Display_ShowString(0+4,13,"  ",0x10|Clock_Color[Clock_ColorSelect][1]);
	else Display_ShowNumber(0+4,13,DS3231_Time[3],2,0x10|Clock_Color[Clock_ColorSelect][1]);
	if(Select==4 && Clock_FlashFlag)Display_ShowString(3*8+4,13,"  ",0x10|Clock_Color[Clock_ColorSelect][1]);
	else Display_ShowNumber(3*8+4,13,DS3231_Time[4],2,0x10|Clock_Color[Clock_ColorSelect][1]);
	if(Select==5 && Clock_FlashFlag)Display_ShowString(6*8+1,16,"  ",Clock_Color[Clock_ColorSelect][2]);
	else Display_ShowNumber(6*8+1,17,DS3231_Time[5],2,Clock_Color[Clock_ColorSelect][2]);
	Display_ClearAreaBuf(51,60,26,27);
	for(i=51;i<51+DS3231_Time[5]%10;i++){Display_ShowPoint(i,26,Clock_Color[Clock_ColorSelect][3]);}
	Display_Update();
}

void Clock_ChangeColor(void)
{
	Clock_ColorSelect++;
	Clock_ColorSelect%=12;
	Display_ClearBuf();
	AT24C02_WriteByte(0,Clock_ColorSelect);
}

void Clock_Loop(void)
{
	Clock_FlashFlag=!Clock_FlashFlag;
}
