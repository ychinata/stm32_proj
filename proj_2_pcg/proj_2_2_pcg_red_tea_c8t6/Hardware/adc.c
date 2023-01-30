#include "adc.h"
#include "dma.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2022/04/12
//�汾��V1.2
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////


//1��ADC����APB2�ϣ�ʱ��Ϊ72MHz��ͨ����Ƶ�ķ�ʽ��ADC �ṩʱ�ӣ�Ԥ��Ƶ��2��4��6��8 ���ַ�ʽ��
//2��ͨ������ʱ�䣬ͨ������ʱ���Ӱ������ľ��ȡ�
//3��ת��ʱ��		��ʽ��TCONV = ����ʱ��+ 12.5 ������ 
//		���磺��ADCCLK=14MHz ��1.5 ���ڵĲ���ʱ��   TCONV = 1.5 + 12.5 = 14 ���� = 1��s
//		1��һ�����������������������ôת��ʱ�伴�ǲ������ڡ�
//		2����ͨ����ʱ�����д�������ADC������Ҫ���϶�ʱ�������ʱ�䡣
//4��ȷ��������
//		1�����������Ҫ�ﵽ400KHz��Ϊ�˴ﵽ��õľ��ȣ�����ѡȡADCʱ��Ϊ12MHz����6��Ƶ��
//��12MHz �Լ���֤�����ʵ�����£�����ʱ��Խ���䣬׼ȷ�Ծ�Խ�ã�ÿ�β���ʱ��Ϊ1000000us/400000HZ = 2.5us��
//		2�����Լ��� 2.5us = ��12.5 + ����ʱ�䣩/ 12MHz ��������ò���ʱ��Ϊ17.5�����Բ���ʱ���ѡ��
//����С�ڵ���17.5�����ڣ����ܱ�֤��������400KHz ���ϡ��������ǿ���ѡ��1.5��7.5��13.5��Ϊ��ø�
//�ߵľ�׼�ȣ����ǿ���ѡ��13.5�����ڡ��мǲ�����������ﵽҪ��

/*
����ϵͳ��Ƶ72M��
RCC_ADCCLKConfig(RCC_PCLK2_Div6);��ôADC��ʱ�Ӿ���12M��
ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1,ADC_SampleTime_239Cycles5);
������Ϊ  12M/��239.5+12.5��=47.619KHz
STM32 ADC��ʱ�Ӳ�Ҫ����14MHz������ת�����Ȼ��½���
*/

volatile u16 ADCConvertedValue[ADCBUF_SIZE];//�������ADCת�������Ҳ��DMA��Ŀ���ַ,3ͨ����ÿͨ���ɼ�10�κ���ȡƽ����

u16 BAT_VOL;
u8 BAT_Percent;
u8 LowPower_flag;
u8 power_detective;

//����ADC
// IO-PA7/PB1
void ADC1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//��Ƶ   ����adcʱ�ӷ�Ƶ
    //RCC_ADCCLKConfig(RCC_PCLK2_Div2);//��Ƶ   ����adcʱ�ӷ�Ƶ

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;       //adc��������
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;   //ģ������ 
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;       //adc��������
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;   //ģ������ 
    GPIO_Init(GPIOB,&GPIO_InitStructure); 

    ADC_DeInit(ADC1);//��λADC1����Ϊȱʡֵ


    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;     //����ģʽ
    //ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;     //ADCͬ�� 1 2 DMA��

    //ADC_InitStructure.ADC_ScanConvMode = DISABLE; //��ģת����ɨ�裨��ͨ����ģʽ=ENABLE  ���Σ���ͨ����ģʽ=DISABLE
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //��ģת����ɨ�裨��ͨ����ģʽ=ENABLE  ���Σ���ͨ����ģʽ=DISABLE
    
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //����ִ�л��ǵ���ִ�� ��ʱ��������Ҫ�ر�
    //ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //DISABLE��ͨ��ģʽ��enable��ͨ��ɨ��ģʽ 

    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //������ʽ ���
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;   //������ʽ ��ʱ��4 CC4

    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 3;                 //˳����й���ת����ͨ����  
    ADC_Init(ADC1, &ADC_InitStructure);


    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 3, ADC_SampleTime_239Cycles5 );

    ADC_DMACmd(ADC1, ENABLE);//����DMA ADC�ɼ�
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_ResetCalibration(ADC1);//����ָ��adcУ׼�Ĵ���		
    while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ���״̬
    ADC_StartCalibration(ADC1);//��ʼָ��adcУ׼״̬
    while(ADC_GetCalibrationStatus(ADC1));//��ȡָ��adc��У׼����	
        
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������		    
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);//�����ⲿ����ģʽʹ��
    ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��ο���ѹ 1.2V ʹ���ڲ��ο���ѹ��������ٷֱ�
}


////��� ADC ֵ
////ch:ͨ��ֵ
//u16 Get_Adc(u8 ch)
//{
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ�����ת������
//		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
//		return ADC_GetConversionValue(ADC1); //�������һ�� ADC1 �������ת�����
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




