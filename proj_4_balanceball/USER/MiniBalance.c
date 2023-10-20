#include "stm32f10x.h"
#include "sys.h"

  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
u8  Flag_Stop=1;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Position_X,Position_Y;
float Balance_Kp=55,Balance_Ki=0.000,Balance_Kd=58,Data_Show,Data_Show2;
float Zero_X=133,Zero_Y=100,Target_X,Target_Y;
u8 delay_50,delay_flag;         //  50ms��׼��ʾ��־λ
int PS2_LX=128,PS2_LY=128,PS2_RX=128,PS2_RY=128,PS2_KEY;     //PS2ң�����

int main(void)
{ 
    delay_init();	    	            //=====��ʱ������ʼ��	
    uart_init(128000);	            //=====���ڳ�ʼ��Ϊ
    JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
    LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
    KEY_Init();                     //=====������ʼ��
    MY_NVIC_PriorityGroupConfig(2); //=====�жϷ���
    delay_ms(100);                  //=====��ʱ�ȴ�
    //	uart3_init(9600);               //=====����3��ʼ��
    Adc_Init();                     //=====adc��ʼ��
    delay_ms(100);                  //=====��ʱ�ȴ�
    OLED_Init();                    //=====OLED��ʼ��	
    TIM1_Int_Init(99,7199);         //=====��ʱ��10ms�ж�
    TIM4_PWM_Init(9999,143);        //=====PWM��ʼ��
    Screen_Init();
    PS2_Init();                     //=====PS2�ֱ���ʼ��
    PS2_SetInit();		 							//=====ps2���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
    
    while(1) {
        PS2_KEY=PS2_DataKey();	         //�ɼ�PS2����
        PS2_LX=PS2_AnologData(PSS_LX);    //�ɼ�PS2ҡ��
        PS2_LY=PS2_AnologData(PSS_LY);
        PS2_RX=PS2_AnologData(PSS_RX);
        PS2_RY=PS2_AnologData(PSS_RY);

        oled_show();          //===��ʾ����
        DataScope();          //����MiniBalance��λ��
        delay_flag=1;	
        delay_50=0;
        while(delay_flag);	     //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ				
    } 
}

