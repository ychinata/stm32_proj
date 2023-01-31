#include "dma.h"
#include "delay.h"       //延时
#include "myQueue.h"
#include "Timer.h"
#include "usart.h"       //串口 
#include "adc.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/02/08
//版本：V1.2
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////

// 全局变量
u8 g_Uart2DmaFinishFlag;

//DMA的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改 
/////////////////////////////////////////////////////////////////////////////////////////////

// USART发送+DMA: 使用DMA1_Channel7,外设(串口)是目的
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx,cpar:外设地址,cmar:内存地址
void DMA_UART_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
    DMA_InitTypeDef DMA_InitStructure;	

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输

    DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
    //DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
}


//开启一次DMA传输
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
    DMA_CHx -> CCR &= ~(1<<0);	//关闭当前的DMA传输（不管有没有传输完成）
    DMA_CHx -> CNDTR = len;			//设置数据传输量
    DMA_CHx -> CCR |= 1<<0;			//开始DMA传输  
}
 

// 初始化DMA中断 cmd 使能
// 2022.11.20 RENAME:UART2_DMA_TX_NVIC_Config->DMA_UART2_TX_NVIC_Config
void DMA_UART2_TX_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    if(cmd == ENABLE) {
        DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE); //传输完成中断
    } else {
        DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE); //传输完成中断
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ADC+DMA:DMA1_Channel1,外设(传感器)是源
//单路ADC-删除
//多路ADC
void DMA_ADC_Config(u16 *buffer, u16 len)
{	
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能时钟

    DMA_DeInit(DMA1_Channel1);    //将通道一寄存器设为默认值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//该参数用以定义DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;//该参数用以定义DMA内存基地址(转换结果保存的地址)
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//该参数规定了外设是作为数据传输的目的地还是来源，此处是作为来源
    DMA_InitStructure.DMA_BufferSize = len;//定义指定DMA通道的DMA缓存的大小,单位为数据单位。
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设定外设地址寄存器递增与否,此处设为不变 Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//用来设定内存地址寄存器递增与否,此处设为递增，Enable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道拥有高优先级 分别4个等级 低、中、高、非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA的通道
			
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

// DMA ADC 中断配置
void DMA_ADC_NVIC_Config(u8 cmd)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ClearITPendingBit(DMA_IT_TC); //清除更新中断请求位
    if(cmd == ENABLE) {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //传输完成中断
    } else {
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE); //传输完成中断
    }
}

///////////////////////////////////////以下删除废弃代码

