
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



TODO:
1.DMA原理
2.myquene.c待整理
