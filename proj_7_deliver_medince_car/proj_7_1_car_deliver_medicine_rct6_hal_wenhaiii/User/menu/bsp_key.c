/*��Ƶ�̳����� https://www.bilibili.com/video/BV1UL411V7XK?p=2&share_source=copy_web   �ǳƣ���ιιι*/
#include "bsp_key.h"
#include "menu.h"



int iButtonCount;
int iButtonFlag;
char g_nButton;     //�����ⲿ���������������ط����á���ʹ�ö�ʱ��������ʱ�������,�����¼����Ч������ʲô��


u8 KEY=0;

void KEY_GPIO_Init(void)    //�Լ��Ŷ�������PCB���ϵ����������������ⲿ�жϵķ�ʽŪ��
{
	GPIO_InitTypeDef KEY_Init;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	KEY_Init.Mode = GPIO_MODE_INPUT;
	KEY_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	KEY_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 ;  //PE1��ʱ�����г�ʼ��
	KEY_Init.Pull =  GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &KEY_Init);
	
}

void ButtonScan(void)   //���ö�ʱ������������������Ҳ���ڶ�ʱ���ĸ����жϺ�����
{                       //�ж����������Ƿ���Ч
	switch(KEY_Scan())   
	{
		case(1):  //KEY1
			iButtonCount++;        //sysTick��5ms����һ��ɨ�裬������5ms��һ�Σ�����2����10ms
			if(iButtonCount >= 10)   //�����Ӧ�����жϵĶ�ʱ���ĸ�������Ϊ3ms����ô������ǵñ�������30ms�İ������µĵ�ƽ
			{
				if(iButtonFlag == 0)       //�ж���û���ذ���1Ϊ�У�0Ϊû��
				{
					iButtonCount = 0;
					iButtonFlag = 1;
					g_nButton = 1;
				}
				else iButtonCount = 0; //����ذ������������¼���
			}
			else g_nButton = 0;  //���û���ȶ�����30ms�����ʾû�а��°���
		break;
		
		case(2):  //KEY2
			iButtonCount++;
			if(iButtonCount >= 10)   
			{
				if(iButtonFlag == 0)      
				{
					iButtonCount = 0;
					iButtonFlag = 1;
					
					g_nButton = 2;
				}
				else iButtonCount = 0; 
			}
			else g_nButton = 0;
		break;
		
		case(3):  //K3
			iButtonCount++;
			if(iButtonCount >= 10)   
			{
				if(iButtonFlag == 0)     
				{
					iButtonCount = 0;
					iButtonFlag = 1;
					
					g_nButton = 3;
				}
				else iButtonCount = 0; 
			}
			else g_nButton = 0;  
		break;
	
		default:                   
			iButtonCount = 0;
			iButtonFlag = 0;
		   g_nButton = 0;
		break;
	}
	
}

uint8_t KEY_Scan(void)   //������ް������� 
{
	if(KEY1==0)       return 1;      //�������if  else if  ��Խǰ������ȼ�Խ��
  else if(KEY2==0)  return 2;
	else if(KEY3==0)  return 3;
	
	return 0;   //�ް�������
}


/**
  * ��������: ��ȡ����K1��״̬
  * �����������
  * �� �� ֵ: KEY_DOWN�����������£�
  *           KEY_UP  ������û������
  * ˵    �����ޡ�
  */
KEYState_TypeDef K1_StateRead(void)
{
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==K1_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(10);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==K1_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==K1_DOWN_LEVEL);      
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return KEY0_DOWN;
    }
  }
  /* ����û�����£�����û������״̬ */
  return KEY0_UP;
}

