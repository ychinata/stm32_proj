/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdio.h"
#include "echo.h"
#include "mpu6050.h"
#include "CAR_TASK.h"
#include "inv_mpu.h"
#include "tim.h"
#include "dma.h"
#include "oled.h"
#include "CAR_TASK.h"
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
/* USER CODE BEGIN Variables */
int task_flag = 0;//�����ɼ������Ƿ�ʼ��� 0�����ڳ�ʼ����1Ϊ��ʼ
/* USER CODE END Variables */
osThreadId Task_200HZHandle;
osThreadId Task_100HZHandle;
osThreadId Task_5HZHandle;
osThreadId Task_InteractioHandle;
osThreadId sw1_pwm_taskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask_200HZ(void const * argument);
void StartTask_100HZ(void const * argument);
void StartTask_5HZ(void const * argument);
void StartTask_Interaction(void const * argument);
void Start_sw_1Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
 
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_200HZ */
  osThreadDef(Task_200HZ, StartTask_200HZ, osPriorityRealtime, 0, 128);
  Task_200HZHandle = osThreadCreate(osThread(Task_200HZ), NULL);

  /* definition and creation of Task_100HZ */
  osThreadDef(Task_100HZ, StartTask_100HZ, osPriorityHigh, 0, 128);
  Task_100HZHandle = osThreadCreate(osThread(Task_100HZ), NULL);

  /* definition and creation of Task_5HZ */
  osThreadDef(Task_5HZ, StartTask_5HZ, osPriorityIdle, 0, 128);
  Task_5HZHandle = osThreadCreate(osThread(Task_5HZ), NULL);

  /* definition and creation of Task_Interactio */
  osThreadDef(Task_Interactio, StartTask_Interaction, osPriorityIdle, 0, 128);
  Task_InteractioHandle = osThreadCreate(osThread(Task_Interactio), NULL);

  /* definition and creation of sw1_pwm_task */
  osThreadDef(sw1_pwm_task, Start_sw_1Task, osPriorityIdle, 0, 128);
  sw1_pwm_taskHandle = osThreadCreate(osThread(sw1_pwm_task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTask_200HZ */
/**
  * @brief  Function implementing the Task_200HZ thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask_200HZ */
void StartTask_200HZ(void const * argument)
{
	/* USER CODE BEGIN StartTask_200HZ */

	vTaskSuspendAll() ;

	OLED_Init();
	OLED_CLS();
	OLED_ShowStr (1,1,"mpu init--",2);
	while(mpu_dmp_init()) {}
	
	OLED_ShowStr (1,3,"mpu ok",2);
	printf("200HZ start\n");	
	delay_ms(50);	
	OLED_ShowStr (1,3,"HF start",2);
	delay_ms(200);	
	OLED_CLS();
	OLED_ShowStr (1,1,"powered by",2);	
	OLED_ShowStr (35,3,"HF",2);
	delay_ms(500);
	task_flag = 1;
			
	xTaskResumeAll() ;
    
    /* Infinite loop */
    for(;;) {
		//vTaskSuspendAll() ;//�ٽ���뱣����ʼ
		Car_Task_200HZ();
		//xTaskResumeAll() ; //�ٽ���뱣������
 
      osDelay(5);
    }
  /* USER CODE END StartTask_200HZ */
}

/* USER CODE BEGIN Header_StartTask_100HZ */
/**
* @brief Function implementing the Task_100HZ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_100HZ */
void StartTask_100HZ(void const * argument)
{
  /* USER CODE BEGIN StartTask_100HZ */
	while(task_flag == 0 ){}//�Ȼ����ɼ�����ʼ
    printf("100HZ start\n");
    /* Infinite loop */
    for(;;)
    {
        Car_Task_100HZ();
        osDelay(10);
    }
  /* USER CODE END StartTask_100HZ */
}

/* USER CODE BEGIN Header_StartTask_5HZ */
/**
* @brief Function implementing the Task_5HZ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_5HZ */
void StartTask_5HZ(void const * argument)
{
  /* USER CODE BEGIN StartTask_5HZ */
    //printf("5HZ start\n");

    /* Infinite loop */
    for(;;)
    {

       Car_Task_5HZ();
        osDelay(100);//ʵ������Ƶ��10hz

			
    }
  /* USER CODE END StartTask_5HZ */
}

/* USER CODE BEGIN Header_StartTask_Interaction */
/**
* @brief Function implementing the Task_Interactio thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Interaction */
void StartTask_Interaction(void const * argument)
{
  /* USER CODE BEGIN StartTask_Interaction */
		vTaskSuspendAll() ;//�ٽ���뱣����ʼ
    printf("Interaction start\n");
		xTaskResumeAll() ; //�ٽ���뱣������
    //HAL_StatusTypeDef HAL_UART_Receive_DMA(&huart3, uint8_t *pData, 255);
    /* Infinite loop */
    for(;;)
    {
			vTaskSuspendAll() ;//�ٽ���뱣����ʼ
			Car_Task_Interaction();
			xTaskResumeAll() ; //�ٽ���뱣������
        osDelay(200);
    }
  /* USER CODE END StartTask_Interaction */
}

/* USER CODE BEGIN Header_Start_sw_1Task */
/**
* @brief Function implementing the sw1_pwm_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_sw_1Task */
void Start_sw_1Task(void const * argument)
{
  /* USER CODE BEGIN Start_sw_1Task */
    /* Infinite loop */
    for(;;)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
        osDelay(100);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
        osDelay(200);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
        osDelay(200);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
        osDelay(1000);
    }
  /* USER CODE END Start_sw_1Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/