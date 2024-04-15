/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2023-03-02

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2023-03-02

All rights reserved
***********************************************/

#include "MPU6050.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "Header.h"
#include "ioi2c.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "filter.h"


#define INT PBin(9)




//�򵥵Ĳ��Գ��򣬽��в��Ե�ʱ����Ҫע�͵�ԭ����main������Ȼ��Ѳ��Գ�����ע�͵��Ļָ�
//����mup6050ģ����Կ����Ƕȱ仯


/**************************************************************************
Function: Get angle
Input   : way��The algorithm of getting angle 1��DMP  2��kalman  3��Complementary filtering
Output  : none
�������ܣ���ȡ�Ƕ�	
��ڲ�����way����ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	if(way==1)                           //DMP�Ķ�ȡ�����ݲɼ��ж϶�ȡ���ϸ���ѭʱ��Ҫ��
	{	
		Read_DMP();                      //��ȡ���ٶȡ����ٶȡ����
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //��ȡX��������
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //��ȡX����ٶȼ�
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
		if(Gyro_X>32768)  Gyro_X-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת��
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת��
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //��������ת��
		if(Accel_X>32768) Accel_X-=65536;                //��������ת��
		if(Accel_Y>32768) Accel_Y-=65536;                //��������ת��
		if(Accel_Z>32768) Accel_Z-=65536;                //��������ת��
		Accel_Angle_x=atan2(Accel_Y,Accel_Z)*180/Pi;     //������ǣ�ת����λΪ��	
		Accel_Angle_y=atan2(Accel_X,Accel_Z)*180/Pi;     //������ǣ�ת����λΪ��
		Gyro_X=Gyro_X/16.4;                              //����������ת�������̡�2000��/s��Ӧ������16.4���ɲ��ֲ�
		Gyro_Y=Gyro_Y/16.4;                              //����������ת��	
		if(way==2)		  	
		{
			Roll= -Kalman_Filter_x(Accel_Angle_x,Gyro_X);//�������˲�
			Pitch = -Kalman_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		else if(way==3) 
		{  
			Roll = -Complementary_Filter_x(Accel_Angle_x,Gyro_X);//�����˲�
			Pitch= -Complementary_Filter_y(Accel_Angle_y,Gyro_Y);
		}
	}
}
/***********�����ǲ��Գ�����ע��main.c�е�������******************/

///**************************************************************************
//Function: main
//Input   : none
//Output  : none
//�������ܣ����Գ���������	
//��ڲ�������
//����  ֵ����
//**************************************************************************/	
//int main(void)
//{
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�����
//	DEBUG_USART_Init();					//���Դ������ã�����1��������115200
//	OLED_Init();						//OLED��ʼ��
//	MPU6050_Init();						//MPU6050��ʼ��
//	while(1)
//	{
//		/****************��ʾ����ʾ�Ƕ�*******************/
//		OLED_ShowString(0,0,"Pitch:");
//		if(Pitch<0)		
//		{
//			OLED_ShowString(70,0,"-");	
//			OLED_ShowNumber(80,0,-(int)Pitch,3,12);
//		}
//		else
//		{
//			OLED_ShowString(70,0,"+");	
//			OLED_ShowNumber(80,0,(int)Pitch,3,12);
//		}	
//		
//		OLED_ShowString(0,10,"Roll:");
//		if(Pitch<0)		
//		{
//			OLED_ShowString(70,10,"-");	
//			OLED_ShowNumber(80,10,-(int)Roll,3,12);
//		}
//		else
//		{
//			OLED_ShowString(70,10,"+");	
//			OLED_ShowNumber(80,10,(int)Roll,3,12);
//		}			
//		
//		
//		OLED_ShowString(0,20,"Yaw:");
//		if(Pitch<0)		
//		{
//			OLED_ShowString(70,20,"-");	
//			OLED_ShowNumber(80,20,-(int)Yaw,3,12);
//		}
//		else
//		{
//			OLED_ShowString(70,20,"+");	
//			OLED_ShowNumber(80,20,(int)Yaw,3,12);

//		}			
//		OLED_Refresh_Gram();	
//		/****************��ʾ����ʾ�Ƕ�*******************/
//	}
//	
//	
//}



///**************************************************************************
//Function: EXTI9_5_IRQHandler
//Input   : none
//Output  : none
//�������ܣ��ж϶�ȡ���ݣ��ⲿ�ж�	
//��ڲ�������
//����  ֵ����
//**************************************************************************/	
//int EXTI9_5_IRQHandler(void) 
//{
//	if(INT==0)		
//	{
//		EXTI->PR=1<<9;    //����ж�        
//		Get_Angle(1);     //��ȡ�Ƕȣ�1.DMP 2.�������˲� 3.�����˲� 									

//	}

//	return 0;	  

//}


