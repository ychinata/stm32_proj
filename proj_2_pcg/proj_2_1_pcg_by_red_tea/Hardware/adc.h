#ifndef __ADC_H
#define __ADC_H	 
#include "sys.h"


#define	BAT_100			2606  //2606*0.806=2100mV 分压后电压为4200mV
#define	BAT_0				1861	//1861*0.806=1500mV	分压后电压为3000mV
#define	BAT_LEVEL		746 	//总等级746
#define	BAT_LOW_POWER_LEVEL		40 //低电量


#define ADCBUF_SIZE 3
extern volatile u16 ADCConvertedValue[ADCBUF_SIZE];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数

/////////////////////////////////////////////////////////////////////
//typedef union 
//{
//     u16 ADC_buf;
//		 u8 adc_buf[2];
//}_ADC_UNION;
//extern _ADC_UNION  ADC_UNION;
//				ADC_UNION.ADC_buf=ADCConvertedValue[0]; //通道0 声音
//共用体数据，低位在前	打印结果 0787 87 07 
//				Main_printf("%04x ",ADC_UNION.ADC_buf);	
//				Main_printf("%02x ",ADC_UNION.adc_buf[0]);	
//				Main_printf("%02x \r\n",ADC_UNION.adc_buf[1]);	
//_ADC_UNION  ADC_UNION;
/////////////////////////////////////////////////////////////////////

extern u16 BAT_VOL;
extern u8 BAT_Percent;
extern u8 LowPower_flag;
extern u8 power_detective;



void adc1_init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif
