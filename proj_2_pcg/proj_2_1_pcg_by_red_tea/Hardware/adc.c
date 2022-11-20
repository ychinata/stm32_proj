#include "adc.h"
#include "dma.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/04/12
//版本：V1.2
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//1）ADC接在APB2上，时钟为72MHz，通过分频的方式给ADC 提供时钟，预分频有2、4、6、8 四种方式。
//2）通道采样时间，通道采样时间会影响采样的精度。
//3）转换时间		公式：TCONV = 采样时间+ 12.5 个周期 
//		例如：当ADCCLK=14MHz 和1.5 周期的采样时间   TCONV = 1.5 + 12.5 = 14 周期 = 1μs
//		1）一般情况，如果是软件启动，那么转换时间即是采样周期。
//		2）若通过定时器进行触发启动ADC，则还需要加上定时器的相关时间。
//4）确定采样率
//		1）如果采样率要达到400KHz，为了达到最好的精度，我们选取ADC时钟为12MHz，即6分频。
//在12MHz 以及保证采样率的情况下，采样时间越长其，准确性就越好，每次采样时间为1000000us/400000HZ = 2.5us。
//		2）可以计算 2.5us = （12.5 + 采样时间）/ 12MHz ，可以求得采样时间为17.5；所以采样时间的选择
//必须小于等于17.5个周期，才能保证采样率在400KHz 以上。所以我们可以选择1.5、7.5、13.5，为获得更
//高的精准度，我们可以选择13.5个周期。切记采样点数必须达到要求

/*
假设系统主频72M。
RCC_ADCCLKConfig(RCC_PCLK2_Div6);那么ADC的时钟就是12M。
ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1,ADC_SampleTime_239Cycles5);
采样率为  12M/（239.5+12.5）=47.619KHz
STM32 ADC的时钟不要超过14MHz，否则转换精度会下降。
*/

volatile u16 ADCConvertedValue[ADCBUF_SIZE];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数

u16 BAT_VOL;
u8 BAT_Percent;
u8 LowPower_flag;
u8 power_detective;

//设置ADC
// IO-PA7/PB1
void ADC1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频   设置adc时钟分频
    //RCC_ADCCLKConfig(RCC_PCLK2_Div2);//分频   设置adc时钟分频

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;       //adc输入引脚
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;   //模拟输入 
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;       //adc输入引脚
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;   //模拟输入 
    GPIO_Init(GPIOB,&GPIO_InitStructure); 

    ADC_DeInit(ADC1);//复位ADC1，设为缺省值


    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;     //独立模式
    //ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;     //ADC同步 1 2 DMA用

    //ADC_InitStructure.ADC_ScanConvMode = DISABLE; //数模转换：扫描（多通道）模式=ENABLE  单次（单通道）模式=DISABLE
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //数模转换：扫描（多通道）模式=ENABLE  单次（单通道）模式=DISABLE
    
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //连续执行还是单次执行 定时器触发需要关闭
    //ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //DISABLE单通道模式，enable多通道扫描模式 

    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //触发方式 软件
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;   //触发方式 定时器4 CC4

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 3;                 //顺序进行规则转换的通道数  
    ADC_Init(ADC1, &ADC_InitStructure);


    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 3, ADC_SampleTime_239Cycles5 );

    ADC_DMACmd(ADC1, ENABLE);//开启DMA ADC采集
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1);//重新指定adc校准寄存器		
    while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器状态
    ADC_StartCalibration(ADC1);//开始指定adc校准状态
    while(ADC_GetCalibrationStatus(ADC1));//获取指定adc的校准程序	
        
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能或者失能指定的ADC的软件转换启动功能		    
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);//开启外部触发模式使能
    ADC_TempSensorVrefintCmd(ENABLE); //开启内部参考电压 1.2V 使用内部参考电压计算电量百分比
}


////获得 ADC 值
////ch:通道值
//u16 Get_Adc(u8 ch)
//{
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能软件转换功能
//		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
//		return ADC_GetConversionValue(ADC1); //返回最近一次 ADC1 规则组的转换结果
//}


//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//		u32 temp_val=0;
//		u8 t;
//		for(t=0;t<times;t++)
//		{ 
//				temp_val+=Get_Adc(ch);
//				delay_ms(5);
//		}
//		return temp_val/times;
//}




