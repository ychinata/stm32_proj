
PCG数据流向：
ADC->DMA->USART


g_Usart2RxBuf：
USART2接收数据存储于 g_Usart2RxBuf -USART2_IRQHandler
（USART2_RX_BUF改名g_Usart2RxBuf）

UART_Info->sendbuf：
1.在main中malloc内存
2.在DMA_Config中设置DMA的内存基地址


疑问：
1.g_Usart2RxBuf 如何与 UART_Info->sendbuf 关联？
2.g_Usart2RxBuf 是上位机发送过来的数据？
3.DMA1_Channel1/DMA1_Channel7是如何决定的?-2023.1.30
// USART发送+DMA: DMA1_Channel7,外设(串口)是目的
// ADC+DMA:DMA1_Channel1,外设(传感器)是源
4.为什么要使用2个串口


TODO:
1.DMA原理
两个DMA控制器有12个通道(DMA1有7个通道，DMA2有5个通道)
2.myquene.c待整理

更名记录：
UART_Info->g_UART_Info
_UART_Info->UartInfoStru

编译前修改：
QueueInfo
UartInfoStru
