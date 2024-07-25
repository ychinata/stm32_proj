#include "stm32f10x.h"
#include "RCC.h"
#include "NVIC.h"
#include "Delay.h"
#include "IO.h"
#include "Display.h"
#include "math.h"
#include "DS3231.h"
#include "Key.h"
#include "Light.h"
#include "TIM4.h"
#include "Clock.h"
#include "Spectrum.h"
#include "Sample.h"
#include "AT24C02.h"

void SystemTest(void);
void BrightnessSetInit(void);
void BrightnessSet(void);
void TimeShow(void);
void TimeSet(void);
void SpectrumShow(void);
void SpectrumAutoSwitch(void);
void MainLoop(void);
void Global(void);
void ModeChange(void);

/*信息获取*/
uint8_t KeyNum;
uint16_t LightValue;

/*模式切换*/
uint8_t Mode,ModeNext=10,ModeLast,ModeStartFlag,ModeStopFlag;

/*全局功能*/
uint8_t Brightness=0;

/*TimeSet*/
uint8_t TimeSet_Select;

/*其它*/
uint8_t num;
uint8_t flag;

