/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "encoder.h"
#include "stm32f1xx.h"
#include "tim.h"//����timͷ�ļ�
#include "usart.h"

long g_lMotorPulseSigma =0;
long g_lMotor2PulseSigma=0;
short g_nMotorPulse=0,g_nMotor2Pulse=0;//ȫ�ֱ����� ������������ֵ


//�����������ʱ�򲶻����ֵ������Ļ���������cubemx�ı�����������������˲�����������1~16��ֵ


/*******************ʵ������ʱ��ȡ��������ֵ************************/   //����������ת��һ�µ�ʱ��Ҫͬʱ��������������������������control.h���PWMͨ���궨��
void GetMotorPulse(void)//��ȡ�������
{
	g_nMotorPulse = (short)(__HAL_TIM_GET_COUNTER(&htim3));//��ȡ������ֵ   Ϊʲô���ڿ���������������
	
	__HAL_TIM_SET_COUNTER(&htim3,0);//TIM3����������
	
	g_nMotor2Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim8));//��ȡ������ֵ
	g_nMotor2Pulse = -g_nMotor2Pulse;
	__HAL_TIM_SET_COUNTER(&htim8,0);//TIM8����������
	
	
	g_lMotorPulseSigma += g_nMotorPulse;//λ���⻷ʹ�õ������ۻ�      //���һ��λ�ÿ���֮��������
	g_lMotor2PulseSigma += g_nMotor2Pulse;//λ���⻷ʹ�õ������ۻ�   //��¼��֮��ҲҪ��ʱ����ѽ������

}
