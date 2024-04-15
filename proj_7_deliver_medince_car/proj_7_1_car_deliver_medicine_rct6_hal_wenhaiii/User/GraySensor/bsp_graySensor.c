/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "bsp_GraySensor.h"
#include "main.h"
#include "adc.h"
#include "control.h"

//����Ѳ�ߵ� '�Ҷȴ�����' �� ��������Ƿ����ҩƷ�� '���⴫����' �Ĵ��붼�������.c�ļ�����

u8 L2_Val, L1_Val, M_Val, R1_Val, R2_Val;
int Line_Num;


u16 ADC_Value;
float ADC_Volt;
u8 Load_flag =0;  // 0��ʾ��ûת��ҩƷ����ʾת����ϣ�-1��ʾҩƷ��

u8 LoadCount,NotLoadCount;

/*******************�Ҷȴ�����Ѳ��*************/
//ֻҪ���ٶȻ����ƺã�����ֵ����ȷ����  ��ͬ�ٶ�ֵ��Ҫ��ͬ�Ĳ���ֵ��  ���Ժ�һ����õġ�  ��ͬ��ת��Ҫ��Ӧ��ͬ�Ĳ���ϵ������ֵ
void Light_GoStraight_control(void)   //�Ҷ�Ѳ��ֱ��, ��Ҫ�и��ж���Ҫֱ�ж೤����ĺ���    //����Ҫ���м�Ĵ������⵽�ص�����Ϊֹ
{
	  Get_Light_TTL();  
	
	//���Ҫ���Ӿ�׼�Ŀ��ƣ�ֻҪ����Ѳ�߻�������Ҫ��λ�ý��в�����  �ɷ�ֱ�ӽ� g_lMotorPulseSigma  ��g_lMotor2PulseSigma ������ֵ�Ӽ�����
  
	//���Ѱֱ���õĴ��������Զ�ȡģ�����Ļ�������������������Ѳֱ��Ѱͦ�ȵ�


		if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 0 )  Line_Num =  0;
	  if(L1_Val == 0 &&  M_Val == 1 && R1_Val == 1) Line_Num =  400;
	  if(L1_Val == 0 &&  M_Val == 0 && R1_Val == 1 )  Line_Num =  500;
	  if(L1_Val == 1 &&  M_Val == 0 && R1_Val == 0) Line_Num = -500;   
    if(L1_Val == 1 &&  M_Val == 1 && R1_Val == 0) Line_Num = -400;	
	
//	 if(LineNumToggleFlag == 1)
//	 {
//		 Line_Num = - Line_Num;
//	 }
	
}


//�ѻҶȴ���������ֻ����ߵ͵�ƽ�� �ߵ�ƽ��ʶ�𵽺����ˡ�
void Get_Light_TTL(void)  
{
	if(HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin) == GPIO_PIN_SET){L2_Val = 1;} else {L2_Val = 0;}
	if(HAL_GPIO_ReadPin(L1_GPIO_Port, L1_Pin) == GPIO_PIN_SET){L1_Val = 1;} else {L1_Val = 0;}
	if(HAL_GPIO_ReadPin(M_GPIO_Port, M_Pin) == GPIO_PIN_SET)  {M_Val = 1;} else {M_Val = 0;}
	if(HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin) == GPIO_PIN_SET){R1_Val = 1;} else {R1_Val = 0;}
	if(HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin) == GPIO_PIN_SET){R2_Val = 1;} else {R2_Val = 0;}
}



/********����Ƿ�ת��ҩƷ*************/
void LoadOrNot()
{
			HAL_ADC_Start(&hadc1);      
			if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
			{
				ADC_Value = HAL_ADC_GetValue(&hadc1);
				ADC_Volt =  (float)ADC_Value*3.3/4096.0;
			}
			
			if(ADC_Volt <= 1.6)
			{
				NotLoadCount = 0;
				
				LoadCount++;
				if(LoadCount > 3) //3�γ���ִ�е���֮���ٴ��ж�
				{
					Load_flag = 1;
				}
			}
			else if(ADC_Volt >  1.6)
			{
				LoadCount = 0;
				
				if(Load_flag == 1)    //������װ�ع�ҩƷ������²����ж��Ƿ�����ҩƷ
				{
					NotLoadCount++;
					if(NotLoadCount > 3 )  //3�γ���ִ�е���֮���ٴ��ж�
					{
						Load_flag = 2;
					}
			  }
			}
}
