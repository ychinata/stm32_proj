/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "control.h"
#include "usart.h"

#include "tim.h"
#include "bsp_pid.h"
#include "encoder.h"
#include "Fire_protocol.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "adc.h"
#include "openmv.h"
#include "bsp_GraySensor.h"

unsigned char location_control_count = 0;  //ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���

float  MotorPWM =0.0, Motor2PWM =0.0;


float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;
float Line_Outval;
int Turn_val;

static motor_dir_t direction  = MOTOR_FWD;     // ��¼�������
static uint16_t    dutyfactor = 0;             // ��¼���ռ�ձ�
static motor_dir_t direction2  = MOTOR_FWD;     // ��¼���2����
static uint16_t    dutyfactor2 = 0;             // ��¼���2ռ�ձ�
u8 is_motor_en = 0, is_motor2_en = 0;            // ���ʹ��

//u8 LineNumToggleFlag;

u8 Spin_start_flag , Spin_succeed_flag , Stop_Flag;
u8 Line_flag = 0, Turn_flag = 0 ;
u8 stop_count , spin_count;


u16 WaitTimeCount;
u8 WaitFlag , BackFlag;


float g_fTargetJourney = 0;  //���С������������·�̺� �� ��λcm����Ҫ����һ�׶�����������

float Motor_journey_cm, Motor2_journey_cm;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    //������sysTick��������ʱ�������ж�Ҫ���ûص�����ʱ����Ҫ�����������������жϺ���ִ����Ӧ����
{
	if(htim == (&htim7))  
	{

		GetMotorPulse();  
						
		//���� Ѳ�߻�֮��С��ֱ�߾��������Ҫ����������Ѱֱ�ߵ�Ч��������  3.14���ɲ�����Ѳ��Ч�����ã���ֱ���Ļ����Լ�����С���pai��ֵ
		Motor_journey_cm	  = (  g_lMotorPulseSigma /(56*11*4) ) * (6.5*3.14);
		Motor2_journey_cm	= (  g_lMotor2PulseSigma /(56*11*4) ) * (6.5*3.14);
		
    Light_GoStraight_control();    //һֱ��ȡ�Ҷȴ�����״̬����ֻ����ִ��car_go��ʱ�򣬲Ž��в���		
		
		/*****��λ��pid�����ٶȻ���,ʵ������ʱע�͵�*****/
		
//		if(is_motor_en == 1)
//		{
//			MotorPWM = speed_pid_control();
//		}			
//		 if(is_motor2_en == 1) 
//		 {
//			 Motor2PWM  = speed2_pid_control();
//		 }
//		 MotorOutput(MotorPWM,Motor2PWM);
    
		/******/
		 
		 /******��λ������λ���ٶȴ���PIDʱ�ã�ʵ��ʹ��ʱע�͵�******/
//		 
//		 if(is_motor_en == 1 && is_motor2_en == 1)
//		 {
//			 Location_Speed_control();
//			 MotorPWM = speed_Outval;  
//			 Motor2PWM = speed2_Outval;
//			 MotorOutput(MotorPWM,Motor2PWM);
//		 }
		 /************/
		
			if(Line_flag == 1) 
			{	
				
				 //�����жϣ�ֱ�е���λ�ú�ֹͣPID���ƣ���ֹʶ��ͼƬʱ���ߵȴ�װжҩʱ������ܣ�ֱ��Ѳ�߽����Ľ׶β�������� 
				 //��ʵ���Ҫ���ú�һ�������ģ��Ļ�����ʱ��������Ҳ����Ҫ.....�����µ����������...һֱPID����Ҳûɶ
				
				
				 //	����λ���ٶȴ���PIDʱע�͵�����õ��ʧ�ܵĺ���
				 if( (Motor_journey_cm <= (g_fTargetJourney + 25)) && (Motor_journey_cm >= (g_fTargetJourney - 25)) )  //����ƫ����ô����ĺ��𣿲��������ƫ��
				 {
						stop_count++;   //stop_count���ܳ���256
						if(stop_count >=  100)  //100 * 20 = 1.6s  ����ҲҪ������Ŀ��λ��ͣ��1s  //����ʱ���жϷų��㣬�Ա�ɲ��ͣ��
						{
							Line_flag = 0;
							Stop_Flag = 1; //�����־λ���������ж��Ƿ�ִ����һ�׶�����
							stop_count = 0;
							
							//greenLED_on;    //ʵ������ʱ����Ҫ���
							
							set_motor_disable();
							set_motor2_disable();
							
						}
				 }
				 else
				 {
						Stop_Flag = 0;  
						stop_count = 0;  
						
						//greenLED_off;   //ʵ������ʱ����Ҫ���
			 	 }           
		
		
				if (is_motor_en == 1 || is_motor2_en == 1)     // �����ʹ��״̬�²Ž��п��ƴ���
				{
						Location_Speed_control(); //λ�û��ٶȻ�����PID��������ٶȻ������PWMֵ
					 
						if(Line_Num == 0)  //ÿ�λص�������Ҫ������ʱ�򣬶�������������ۼ�������ȡƽ��ֵ�����Ҳ������ת�������ص����ϵ�Ч��
						{
							long Pulse;
							
							Pulse = (g_lMotorPulseSigma + g_lMotor2PulseSigma) / 2;
							
							g_lMotorPulseSigma = Pulse;  //������ʱ��������ϸ����������
							g_lMotor2PulseSigma = Pulse;
						}
								
								
						 //����ǻҶȴ�������Ѳ�߲���
						 Line_Outval =  Line_Num;     //lineNum����PWM����װ��ֵһ�����ҲŻ������Ե�Ч��
						 MotorPWM = speed_Outval  + Line_Outval ;  
						 Motor2PWM = speed2_Outval - Line_Outval ;
						
							
						 MotorOutput( MotorPWM, Motor2PWM);   
				 }
			 }			 
				 
			if( Spin_start_flag == 1)  
			{
				
					if (is_motor_en == 1 || is_motor2_en == 1)     // �����ʹ��״̬�²Ž��п��ƴ���
					{	
						Location_Speed_control(); //λ�û��ٶȻ�����PID��������ٶȻ������PWMֵ

						
						MotorPWM = speed_Outval;
						Motor2PWM = speed2_Outval;
						
						spin_count++;
						if( spin_count >= 100)      //20ms����һ��   100*20 = 2s�����ܹ�����ɵ�תʱ��Ϊ���ޣ�Ӧ���Ѿ���ת�����
						{
							 Spin_start_flag = 0;
							 spin_count = 0;
							
							//ת���е����⣬ת��֮��һֱת,��ʱ������������
							MotorPWM =0;   
							Motor2PWM = 0;
												 
							Spin_succeed_flag = 1;  
							
							//Line_Num = 0.0;   //ת��ʱҪ�����ס��һ�εĲ���ֵ�𣿻�Ӱ�쵽ת����Ѳ�ߣ�����
							//��ת��ǰ���ߵĻ�����ס��һ�ε�Ѳ�߲����������Դﵽ�Ի��ߵ�Ч��
							//����ת��ʱת��һЩ����������ת����һ��ĻҶȴ����������ϣ��Ϳ��Դﵽ�Ի����ϵ�Ч��������					
						}
					
						MotorOutput( MotorPWM, Motor2PWM);
					}					
			}
			
					
	}
	
	else if(htim == (&htim6))  //1s����һ��TIM6���ж�   
	{
		 greenLED_Toggle;
//	   BlueSignal_Toggle;
	}
	
}




/****������Զ��� Stop_flag��1****/

/*����ӽ�λ�û�������ֱ�ӱ�̾��루cm���� ����*/   //������ֱͨ�ж������
void Car_go(int32_t location_cm)   //ֱ�ߺ���     //�������ε�ֱ�к������Ѳ�߲���ʧЧ
{
	float Car_location;
	
	Line_flag = 1;   
	Stop_Flag = 0;
	
	Spin_start_flag = 0;
	Spin_succeed_flag = 0;  
	
//	if(location_cm < 0)
//	{
//		LineNumToggleFlag = 1;
//	}
//	else LineNumToggleFlag = 0;
	
	
	Motor_journey_cm = 0;
	Motor2_journey_cm = 0;
	
	g_fTargetJourney = location_cm;   //��ֹ��ʱ��PID������
	
	//���Ѳ��Ч�����þͽ�3.142�Ӵ�
	Car_location   = ( location_cm / (6.5 * 3.14) ) *   (56*4*11) ;    //Car_location ��  location_cm ת��Ϊ��Ӧ��������   g_fTargetJourney = Ҫת����Ȧ * 1Ȧ�������� 
	//С��λ��ֱ����һ��������������ۻ��ͺã�ת��ʱ����������ʼһ��λ��ǰֱ�����㡣   
	//�򻬵���һ���ֱ���һ����תλ�ò�һ��զ��,�á�Ѳ�߻����ֲ��ͺã�ת����á�ת�򻷡�
	 
	
	  set_pid_target(&pid_location, Car_location);   
	  set_pid_target(&pid_location2, Car_location);
	
	  set_motor_enable();   //ʹ�ܵ������PWM���    
    set_motor2_enable();   //ʹ�ܵ��2����PWM���
	 
	  g_lMotorPulseSigma = 0;    //֮ǰû��������ò���
	  g_lMotor2PulseSigma = 0; 
	
		
}




/******ת�����ʱ�Զ���  Spin_start_flag == 0 �� Spin_succeed_flag == 1  ********/

/*ת������ת90����ת90����ת180���������*/


void spin_Turn(spin_dir_t zhuanxiang)   //����С�����־�(mm) 175mm  //��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
{
	
	  Line_flag = 0;  //������Ѳ�ߵĲ�����
	  Stop_Flag = 0;   //ִ��ת��ʱ����ֱ����ɵı�־λ����. �������һ����ֱ�У������ת�䣬������ҵ��������ֶ���λ
	  
	
    Spin_start_flag = 1;   
	  Spin_succeed_flag = 0; 
	
		float spin90_val;
		float Car_Turn_val;
		
		Motor_journey_cm = 0;
		Motor2_journey_cm = 0;
		
		spin90_val = 0.25*3.1416*LUN_JU;
		
	/****ת���ѭ�߿��Ե�ת��ϵ��**********/
		if(zhuanxiang == left_90)  //openmvʶ����Ҫ����߲�����
		{
			Car_Turn_val = ( spin90_val / (6.5 * 3.142) ) *   (56*4*11) ; 
			Car_Turn_val = 0.95* Car_Turn_val;   //90*0.94 = 84.6   //����Ӱ�죬����ת��������趨�Ķࡣֱ����90�Ƚ�������Ѳ�߾�Ѱ��������
			
		}
		else if(zhuanxiang == right_90)  //openmvʶ����Ҫ���ұ߲�����
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (56*4*11); 
			Car_Turn_val = 0.98 * Car_Turn_val;  //90*0.96 = 86.4    //����Ӱ�죬����ת��������趨�Ķࡣ ��������Ѳ�߿��ܾ�Ѱ��������
		}
		else if(zhuanxiang == back_180)
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (56*4*11);
			Car_Turn_val = 0.98 * Car_Turn_val;   //ͬ��
			Car_Turn_val = 2*Car_Turn_val;      //0.96*180 = 175.5
		}
		
			set_pid_target(&pid_location, -Car_Turn_val);   
			set_pid_target(&pid_location2, Car_Turn_val);
		
			g_lMotorPulseSigma = 0;    //֮ǰû��������ò��ˣ�����������
			g_lMotor2PulseSigma = 0; 
		
			set_motor_enable();   //ʹ�ܵ������PWM���    
		  set_motor2_enable();   //ʹ�ܵ��2����PWM���

}


/************Ѳ�߻����ٲ�������**************/ 

//���Ըĳ�������������Ҷȴ�����ֻ��������ʶ��ʮ��·�ڣ����м��Ǹ���⵽���ߵ�ʱ���ʾ�Ѿ��ص����ϡ�  

//���Ǳ�ƫ���ٶȻ�Ŀ��ֵ�����ͼӵ��ı�  


/*********************��PID**********************/
		/****�ٶȻ�λ�û�����PID����*****/   


void Location_Speed_control(void)          //ԭ��ת�����ֱ�� �������
{
			//mpu_dmp_get_data�������Ӱ�쵽����������Ļ�ȡ��
			 if (is_motor_en == 1 || is_motor2_en == 1)     // �����ʹ��״̬�²Ž��п��ƴ���
			 {
					 location_control_count++;
				  if(location_control_count >= 2)
					{
						location_control_count = 0; 
						
						location_Outval = location_pid_control();
						location2_Outval = location2_pid_control();
					}
					
					set_pid_target(&pid_speed, location_Outval); //ÿ�ζ�������λ�û���ֵ     //�����ٶȻ�PIDʱȡ��������
					set_pid_target(&pid_speed2, location2_Outval); //ÿ�ζ�������λ�û���ֵ
				 
           speed_Outval = speed_pid_control();    //Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
					 speed2_Outval = speed2_pid_control();  
				}

}



float location_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	
	  actual_location =  g_lMotorPulseSigma;   //1Ȧ = 2464������ = 56*11*4  //����λ����Ȧ�����档

    cont_val = location_pid_realize(&pid_location, actual_location);   
	  
	  //��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�
	    
     	/* Ŀ���ٶ����޴��� */
      if (cont_val > TARGET_SPEED_MAX)
      {
        cont_val = TARGET_SPEED_MAX;
      }
      else if (cont_val < -TARGET_SPEED_MAX)
      {
        cont_val = -TARGET_SPEED_MAX;
      }
	
	#if defined(PID_ASSISTANT_EN)
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // ��ͨ�� 1 ����ʵ��ֵ
    //BlueSignal_Toggle;  //���Խ���
  #endif
//	
	return cont_val;
}


float speed_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    int32_t actual_speed;
	
	
	  actual_speed = ((float)g_nMotorPulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	
    cont_val = speed_pid_realize(&pid_speed, actual_speed);    // ���� PID ����
  
//  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // ��ͨ�� 1 ����ʵ��ֵ
//  #endif
	
	return cont_val;
}


float location2_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	  actual_location =  g_lMotor2PulseSigma;   //1Ȧ = 2464������ = 56*11*4  //����λ����Ȧ�����档

    cont_val = location_pid_realize(&pid_location2, actual_location);   
	  
	  // �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�
	
	//Ŀ���ٶ��޷�
	    	/* Ŀ���ٶ����޴��� */
      if (cont_val > TARGET_SPEED_MAX)
      {
        cont_val = TARGET_SPEED_MAX;
      }
      else if (cont_val < -TARGET_SPEED_MAX)
      {
        cont_val = -TARGET_SPEED_MAX;
      }
	
	
//	  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // ��ͨ�� 1 ����ʵ��ֵ
//		//set_computer_value(SEND_TARGET_CMD, CURVES_CH1,&TargetSpeed, 1);                // ��ͨ�� 1 ����Ŀ��ֵ�������Ŀ�����ֵ����������ʱ���÷��ͣ������޷�����λ��������
//  #endif
	
	return cont_val;
}


float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    int32_t actual_speed;    
	
	
	  actual_speed = ((float)g_nMotor2Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed);    // ���� PID ����
    
//  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // ��ͨ�� 1 ����ʵ��ֵ
//  #else
//    printf("ʵ��ֵ��%d. Ŀ��ֵ��%.0f\n", actual_speed, get_pid_target());      // ��ӡʵ��ֵ��Ŀ��ֵ
//  #endif
//	
	return cont_val;
}



/*****************����Ŀ��ƺ���***************/

void MotorOutput(int nMotorPwm,int nMotor2Pwm)//���õ����ѹ�ͷ���
{
		if (nMotorPwm >= 0)    // �жϵ������         
		{
			set_motor_direction(MOTOR_FWD);   //������Ҫ��Ӧ
		}
		else
		{
			nMotorPwm = -nMotorPwm;    
			set_motor_direction(MOTOR_REV);   //������Ҫ��Ӧ
		}
		nMotorPwm = (nMotorPwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotorPwm;    // �ٶ����޴���
		
		
		if (nMotor2Pwm >= 0)    // �жϵ������         
		{
			set_motor2_direction(MOTOR_FWD);   //������Ҫ��Ӧ
		}
		else
		{
			nMotor2Pwm = -nMotor2Pwm;    
			set_motor2_direction(MOTOR_REV);   //������Ҫ��Ӧ
		}
		
		nMotor2Pwm = (nMotor2Pwm > PWM_MAX_PERIOD_COUNT) ? PWM_MAX_PERIOD_COUNT : nMotor2Pwm;    // �ٶ����޴���
		
		set_motor_speed(nMotorPwm);        // ���� PWM ռ�ձ�
		set_motor2_speed(nMotor2Pwm);      // ���� PWM ռ�ձ�
 }

 
/**
  * @brief  ���õ���ٶ�
  * @param  v: �ٶȣ�ռ�ձȣ�
  * @retval ��
  */
void set_motor_speed(uint16_t v)    //���ֻ��ǵ����Կ��ơ��������������Ҫ�����������Ҫ�ĳ�˫���Ե�����
{
  dutyfactor = v;
  
  if (direction == MOTOR_FWD)
  {
    SET_FWD_COMPAER(dutyfactor);     // �����ٶ�
  }
  else
  {
    SET_REV_COMPAER(dutyfactor);     // �����ٶ�
  }
}

/**
  * @brief  ���õ������
  * @param  ��
  * @retval ��
  */
void set_motor_direction(motor_dir_t dir)    //���Ҫ��Ϊƽ�⳵��ǰ���ͺ���
{
  direction = dir;
  
  if (direction == MOTOR_FWD)
  {
		SET_FWD_COMPAER(dutyfactor);     // �����ٶ�
    SET_REV_COMPAER(0);              // �����ٶ�
  }
  else
  {
		SET_FWD_COMPAER(0);              // �����ٶ�
    SET_REV_COMPAER(dutyfactor);     // �����ٶ�
  }
}

/**
  * @brief  ʹ�ܵ��
  * @param  ��
  * @retval ��
  */
void set_motor_enable(void)   //������ʹ�ܺͽ��õĺ�������˫���Կ�����˵����Ч��
{
	is_motor_en  = 1;
//  MOTOR_ENABLE_SD();
  MOTOR_FWD_ENABLE();
  MOTOR_REV_ENABLE();
}

/**
  * @brief  ���õ��
  * @param  ��
  * @retval ��
  */
void set_motor_disable(void)
{
   is_motor_en  = 0;
//  MOTOR_DISABLE_SD();
  MOTOR_FWD_DISABLE();
  MOTOR_REV_DISABLE();
}


/*****************���2�Ŀ��ƺ���***************/
/**
  * @brief  ���õ��2�ٶ�
  * @param  v: �ٶȣ�ռ�ձȣ�
  * @retval ��
  */
void set_motor2_speed(uint16_t v)
{
  dutyfactor2 = v;   
  
  if (direction2 == MOTOR_FWD)     
  {
    SET2_FWD_COMPAER(dutyfactor2);     // �����ٶ�
  }
  else
  {
    SET2_REV_COMPAER(dutyfactor2);     // �����ٶ�
  }
}

/**
  * @brief  ���õ��2����
  * @param  ��
  * @retval ��
  */
void set_motor2_direction(motor_dir_t dir)  
{
  direction2 =  dir;    
  
  if (direction2 == MOTOR_FWD) 
  {
    SET2_FWD_COMPAER(dutyfactor2);     // �����ٶ�     
    SET2_REV_COMPAER(0);              // �����ٶ�
  }
  else
  {
    SET2_FWD_COMPAER(0);              // �����ٶ�
    SET2_REV_COMPAER(dutyfactor2);     // �����ٶ�
  }
}

/**
  * @brief  ʹ�ܵ��2
  * @param  ��
  * @retval ��
  */
void set_motor2_enable(void)
{
	is_motor2_en  = 1;
//  MOTOR_ENABLE_B();
  MOTOR2_FWD_ENABLE();
  MOTOR2_REV_ENABLE();
}

/**
  * @brief  ���õ��2
  * @param  ��
  * @retval ��
  */
void set_motor2_disable(void)
{
	is_motor2_en  = 0;
//  MOTOR_DISABLE_B();
  MOTOR2_FWD_DISABLE();
  MOTOR2_REV_DISABLE();
}


/************************������ֲ��Э����Ƶ���λ���ɲ鿴���β�����PID�����ݴ�����***************/





