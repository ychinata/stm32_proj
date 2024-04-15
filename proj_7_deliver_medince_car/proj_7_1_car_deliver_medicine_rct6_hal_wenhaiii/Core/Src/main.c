/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bsp_key.h"
#include "bsp_sys.h"
#include "bsp_delay.h"
#include "control.h"
#include "bsp_led.h"
#include "menu.h"
#include "Fire_protocol.h"

#include "bsp_pid.h"
#include "openmv.h"
#include "bsp_GraySensor.h"

#include "bsp_BlueTooth.h"



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

u8 Do_count = 0, Do2_count = 0;

u16 showOLEDTime = 0;
u16 SendTime = 0;

extern unsigned char BMP3[];
extern unsigned char BMP4[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	LED_Init();
	KEY_GPIO_Init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	delay_init(72);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	
	
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);   //����TIM3�ı������ӿ�ģʽ
  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);   //����TIM8�ı������ӿ�ģʽ


	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);       //Motor 2  
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);     //Motor1
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);   
	
	
	delay_ms(100);    //��ʱһ�£���OLED�������
	OLED_Init();
	delay_ms(100);    //��ʱһ�£���OLED�������
	
  OLED_Clear(); //��Ϊ��ֱ�ӽ����ģ��������һ�±ȽϺ�
	
	PID_param_init();  

  set_motor2_enable();    //��λ������pidʱ����Ҫ�ֶ�ʹ�ܵ����PWM���
	set_motor_enable();  
	
	 HAL_TIM_Base_Start_IT(&htim7);
	 HAL_TIM_Base_Start_IT(&htim6);
	 
	 
	 HAL_UART_Receive_IT(&huart3,&uart3_rxbuff,1);  //openmvͨ�Ŵ���
	 
	 HAL_UART_Receive_IT(&huart2,&uart2_rxbuff,1);  //����ͨ�Ŵ���
	
	 __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);    //����1�����жϣ�ֻ���������жϣ�����λ��ͨ��
	
	
		protocol_init();       //�����ʼ���˲���ͨ����λ������ѽ, �ٿ����ע������
		
	 __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	 
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		receiving_process();   //����Ұ����λ��ͨ��Э��,���Ե��λ���ٶȴ���PIDʱ��
		
		
		if(SendTime >= 21)
		{
			SendTime = 0;
			
			SendDataToOpenmv();   //����̫�죬����ᳬ��openmv�Ľ������ݻ�����
			
		}
		
		if(showOLEDTime >= 200)
		{
			showOLEDTime = 0;
			OLED_Display(Menu_Item);
			
//			if(OneLoadFlag == 1)
//			{
//				BlueSignal_Toggle;
//			}
			//printf("123");
		  
			/****�ж��Ƿ��з�ҩƷ******/  
			LoadOrNot();
		}
		
	 
		
	  /**************�ȴ�ʶ�𲡷���**************/   
		if(TASK == 1)   
		{
			SetTargetRoom();
			
		}
		
		
		else if(TASK == 2)    
	  {
			 //���Ϸ��͸�openmv ������
				if(Load_flag == 1)    //����ҩ��
				{
						//���˲�������openmv�滮	�� �������Զ�������
						if(TargetRoom == 'A')   
						{
							switch(Do_count)
							{
								case 0:
									 Do_count++;								 
									 Car_go(60-5+15+HeadToWheelCM);   //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
								
								break;
						 
								case 1:
									 if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
									 {
										 Do_count++;
										 spin_Turn(left_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
									 }
								break;
								 
								case 2:
									 if(Spin_succeed_flag == 1)
									 {
										 Do_count++;
										 Car_go(40-5+15-HeadToWheelCM +3);      //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
									 }
								break;
									 
								case	3:
									if(Stop_Flag ==1)
									{
										RedSignal_on;
										
											//������ɫָʾ��
										
									}
								break;
							}			 
						}
						
						
						else if(TargetRoom == 'B')
						{
							switch(Do_count)
							{
								case 0:
									 Do_count++;								 
																									
										Car_go(60-5+15+HeadToWheelCM);  //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��

								break;
						 
								case 1:
									 if(Stop_Flag ==1)
									 {
										 Do_count++;
										 spin_Turn(right_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
									 }
								break;
								 
								case 2:
									 if(Spin_succeed_flag == 1)
									 {
										 Do_count++;
										 Car_go(40-5+15-HeadToWheelCM +3);        //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
									 }
								break;
									 
								case	3:
									if(Stop_Flag ==1)
									{
										
										RedSignal_on;
											//������ɫָʾ��
										
									}
								break;
							}			 
						}
						
						if(OneLoadFlag == 1)
						{
								//ȥ�ж���ҩ����һ���ȴ�Ŀ�귿����ȷ�������÷���
							if(OneTargetRoom == 'C')
							{
								 switch(Do_count)
								 {
										case 0:
											 Do_count++;								 
											 Car_go(160+HeadToWheelCM);    //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
											 
										break;

										case 1:
											 if(Stop_Flag ==1)      
											 {
												 Do_count++;
												 spin_Turn(right_90 );   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
												
											 }
										break;
										 
										case 2:
											 if(Spin_succeed_flag == 1)   //�����������һ���ж˲���ͨ����  ����һ�£�����ͷЧ���û���ֱ�Ӻ���Ч����
											 {
												 Do_count++;   
												 Car_go(HideCM);        //Stop_Flag�ٴ���1    //������˵Ļ���Ѳ�߲�����ֵ��Ҫ�෴  
												
											 }
										break;
											 
										case 3:
											if(Stop_Flag ==1)        
											{
												
												YellowSignal_on; //��һ��ȡҩ
												
											}
										break;
											 
								 }
							}
									
								
							else if(OneTargetRoom == 'D' )
							{
								 switch(Do_count)
								 {
										case 0:
											 Do_count++;								 
											 Car_go(150-5+15+HeadToWheelCM);    //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
											 
										break;

										case 1:
											 if(Stop_Flag ==1)         
											 {
												 Do_count++;
												 spin_Turn(left_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
												
											 }
										break;
										 
										case 2:
											 if(Spin_succeed_flag == 1)   //�����������һ���ж˲���ͨ����  ����һ�£�����ͷЧ���û���ֱ�Ӻ���Ч����
											 {
												 Do_count++;  
												 Car_go(HideCM);        //Stop_Flag�ٴ���1   //������˵Ļ���Ѳ�߲�����ֵ��Ҫ�෴  
												
											 }
										break;
											 
										case 3:
											if(Stop_Flag ==1)          
											{
												
												YellowSignal_on;  //��һ��ȡҩ
												
											}
										break;
											 
										
								 }
							}
						}
				}
			
				else if(Load_flag == 2)       // ֻ�ڽ��˲ŷ���,�������Զ�������Ч��  
				{
						if(TargetRoom == 'A')
						{
							switch(Do2_count)
							{
								case 0:
									Do2_count++;
									 
									spin_Turn(back_180 );
									RedSignal_off; //�رպ�ɫָʾ��
									
								break;
							
								
								case 1:
									if(Spin_succeed_flag == 1)
									{
										Do2_count++;
									
										
										Car_go(40-5+15-HeadToWheelCM+3);   //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
									}
								break;
						 
								case 2:
									 if(Stop_Flag ==1)
									 {
										 Do2_count++;
										 spin_Turn(right_90 );   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
									 }
								break;
								 
								case 3:
									 if(Spin_succeed_flag == 1)
									 {
										 
										 
										 Do2_count++;
										 Car_go(60-5+15-HeadToWheelCM +3 );   //Stop_Flag�ٴ���1
									
									 }
								break;
									 
								case	4:
									if(Stop_Flag ==1)
									{
									
										GreenSignal_on; //������ɫָʾ��
										
									}
								break;
							}	               

						}
						
						else if(TargetRoom == 'B')
						{
							switch(Do2_count)
							{
								case 0:
									Do2_count++;
									 
									spin_Turn(back_180 );
									RedSignal_off; //�رպ�ɫָʾ��
									
								break;
							
								
								case 1:
									if(Spin_succeed_flag == 1)
									{
										Do2_count++;
									
										
										Car_go(40-5+15-HeadToWheelCM+3);     //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
									}
								break;
						 
								case 2:
									 if(Stop_Flag ==1)
									 {
										 Do2_count++;
										 spin_Turn(left_90 );   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
									 }
								break;
								 
								case 3:
									 if(Spin_succeed_flag == 1)
									 {
										 
										 
										 Do2_count++;
										 Car_go(60-5+15-HeadToWheelCM +3);    //Stop_Flag�ٴ���1
										
									 }
								break;
									 
								case	4:
									if(Stop_Flag ==1)
									{
									
										GreenSignal_on; //������ɫָʾ��
										
									}
								break;
							}
						}
				}
			
			
			if(OneLoadFlag == 2)
			{

        if(OneTargetRoom == 'C' || OneTargetRoom == 'D')
				{
					switch(Do_count)
					{
						
						case 3:
							//������ʱ�ı�־λ
							 WaitFlag = 1;
						   YellowSignal_off;	 
						
							 if(WaitTimeCount >=  6000)  //��ʱ6s
							 {
								 Do_count++;
								 
								 WaitFlag = 0;
								 WaitTimeCount = 0;
								 
								 spin_Turn(back_180);
							 
						   }
					  break;
							 
						case	4:
							if(Spin_succeed_flag ==  1)
							{
								 Do_count++;
								
								 Car_go(HideCM + 40-5+15-HeadToWheelCM +3);
							}
						break;
							
					  case 5:
							if(Stop_Flag == 1)
							{
								Do_count++;
								
								RedSignal_on;
							}
						
						break; 
					}
				}  				
			
				//ȥԶ��ȡҩ�����ù��Ƿ�װ��ҩƷ�������ж˵�һ��������ȥԶ�ˡ� 
			  if(OneTargetRoom == 'E' || OneTargetRoom == 'F'  ||  OneTargetRoom == 'G' ||OneTargetRoom == 'H' )   // ��һ�����͵ı�־λ���ƶ�������
				{
					switch(Do_count)
					{
						 case 0:
							 Do_count++;								 
							 Car_go(150-5+15+HeadToWheelCM-2);    //��ʱ Stop_Flag ==0  ��ɺ��Զ���1        //���˲�����Ԥ��openmvʶ���λ��
							 
						 break;

						 case 1:
							 if(Stop_Flag ==1)         
							 {
								 if(OneTargetRoom == 'E' || OneTargetRoom == 'F')
								 {
									 Do_count++;
									 spin_Turn(right_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								 }
								 
								 else if(OneTargetRoom == 'G' ||OneTargetRoom == 'H')
								 {
									 Do_count++;
									 spin_Turn(left_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								 }
								
							 }
						 break;
							 
						 case 2:					 
							 if(Spin_succeed_flag == 1)
							 {
									Do_count++;
									Car_go(HideCM);
							 }
						 break;
							 
						 case 3:
							 if(Stop_Flag == 1)
							 {
									WaitFlag = 1;
								  Do_count++;
							 }
						 break;
							 
						 case 4:
							 if(WaitTimeCount >= 3000 )  //�ȴ�3s
							{
								Do_count++;
								WaitTimeCount = 0;
								WaitFlag = 0;
								
								spin_Turn(back_180);
							}
             break;						 
							 
							 
						 case 5:
							 if(Spin_succeed_flag == 1)
							 {
								 Do_count++;
							   Car_go(HideCM );
							 }
						 break;
							
						 case	6:
							 if(Stop_Flag ==1)         
							 {
								 if(OneTargetRoom == 'E' || OneTargetRoom == 'F')
								 {
									 Do_count++;
									 spin_Turn(right_90 );   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								 }
								 
								 else if(OneTargetRoom == 'G' ||OneTargetRoom == 'H')
								 {
									 Do_count++;
									 spin_Turn(left_90 );   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								 }
								
							 }
						 break;

						 case	7:
							 if(Spin_succeed_flag == 1)
							 {
									Do_count++;
									Car_go(90- (LookCM + HeadToWheelCM) );
							 }
						 break;

						 case	8:
							 if(Stop_Flag == 1 )
							 {
									FindStartFlag = 1;
											
									if(FindTimeCount < 500)    //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��     ����������Ҫ���㣺ʱ�䡢ʶ��ɹ���־λ��λ��
									{
										if(RoomNum == TargetNum && LoR == 1) 
										{
											 Do_count++; 
											 TargetRoom = 'E';      //�ȼٶ���E
											 Car_go(LookCM+HeadToWheelCM);      //���ظ�openmvʶ��Ԥ����λ�ã�ͬʱ�ó���ѹ�ߣ��Ա����ת��
											
											//ֹͣ��ʱ,������
											 FindTimeCount = 0;
											 FindStartFlag = 0;
										}
										else if(RoomNum == TargetNum && LoR == 2 )  	//ʶ��Ŀ������, ����λ��ƫ��
										{
											 Do_count++; 
											 TargetRoom = 'G';    //�ȼٶ���G
											 Car_go(LookCM + HeadToWheelCM);      //���ظ�openmvʶ��Ԥ����λ�ã�ͬʱ�ó���ѹ�ߣ��Ա����ת��
											
											//ֹͣ��ʱ,������
												 FindTimeCount = 0;
												 FindStartFlag = 0;
										}										
										
									}										 

									else if(FindTimeCount >= 500)   //��Ȼ��ʱ��δʶ�𵽣�������ʼ���Ǹ����'E'
									{
										 Do_count++; 
										 Car_go(LookCM + HeadToWheelCM);      //���ظ�openmvʶ��Ԥ����λ�ã�ͬʱ�ó���ѹ�ߣ��Ա����ת��
											
											//ֹͣ��ʱ,������
										 FindTimeCount = 0;
										 FindStartFlag = 0;
									}
										
								}
						 break;
						 
						 case	9:
								if(Stop_Flag == 1)
								{
									Do_count++;
									if(TargetRoom == 'E')
									{
										spin_Turn(left_90);
									}
									else if(TargetRoom == 'G')
									{
										spin_Turn(right_90);
									}
								}
						 break;
								
						 case 10:
								if(Spin_succeed_flag == 1)
								{
									Do_count++;
									Car_go(90 -HeadToWheelCM -LookCM);
								}
						 break;
						 
						 case 11:
								if(Stop_Flag == 1)
								{
									FindStartFlag = 1;
									if(FindTimeCount < 500)
									{
										if(TargetRoom == 'E')
										{
												if(RoomNum == TargetNum && LoR == 1)    //ʶ��RoomNum, ����λ��ƫ��    
											 {
												 Do_count ++;  
												 TargetRoom = 'E';
												 Car_go(LookCM+HeadToWheelCM);      //���ظ�openmvʶ��Ԥ����λ�ã�ͬʱ�ó���ѹ�ߣ��Ա����ת��
												 
												 FindStartFlag = 0;
												 FindTimeCount =0;
											 }										 
											 
											 else if(RoomNum == TargetNum && LoR == 2)   //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
											 {
												 Do_count++;  
												 TargetRoom = 'F';
												 Car_go(LookCM + HeadToWheelCM);
												 
												 FindStartFlag = 0;
												 FindTimeCount =0;
											 }
										}
										
										else if(TargetRoom == 'G')
										{
												if(RoomNum == TargetNum && LoR == 1)    //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��    
											 {
												 Do_count++; 
												 TargetRoom = 'G';
												 Car_go(LookCM+HeadToWheelCM);												 
												 
												 FindStartFlag = 0;
												 FindTimeCount =0;
											 }										 
											 
											 else if(RoomNum == TargetNum && LoR == 2)   //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
											 {
												 Do_count++;  
												 TargetRoom = 'H';
												 Car_go(LookCM + HeadToWheelCM);
												 
												 FindStartFlag = 0;
												 FindTimeCount =0;
											 }
										}		
									}
									
									else if(FindTimeCount >= 500)   
									{
										Do_count++;  
										Car_go(LookCM + HeadToWheelCM);
										
										
										FindStartFlag=0;
										FindTimeCount=0;
									}
										
								}
						 break;
						 
						 case 12:
								if(Stop_Flag == 1)
								{
									Do_count++;
									if( TargetRoom == 'E' || TargetRoom == 'G')
									{
										spin_Turn(left_90);
									}
									else if( TargetRoom == 'F' || TargetRoom == 'H')
									{
										spin_Turn(right_90);
									}
								}
						 break;
						 
						 case 13:
								if(Spin_succeed_flag == 1)
								{
									Do_count++;
									Car_go(40-5+15-HeadToWheelCM +3);
								}
						 break;					 
						
						 case 14:
								if(Stop_Flag == 1)
									{
										
										RedSignal_on;
										 //������ɫָʾ��
									}
						 break;					 
						
					}
				}
				
			}
		}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
