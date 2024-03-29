#ifndef __myQueue_H
#define __myQueue_H	 
#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/02/08
//版本：V1.3
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////

//push()方法可以在数组的末属添加一个或多个元素
//shift()方法把数组中的第一个元素删除
//unshift()方法可以在数组的前端添加一个或多个元素
//pop()方法把数组中的最后一个元素删除


//循环队列 结构体声明	
//__align(4) typedef struct {	
typedef struct {	
    u8 Queue_Full_flag;		//队列存满标志
    u8 Queue_data_state;	//数据状态
    u32 front, rear;		//front指向队列头，rear为队列尾部
    u32 capacity,length;	//队列大小，容量
    //capacity  队列容量 （队列缓冲区数量） 
    //length	队列长度/大小 （缓存的数据长度）
    u16 carry_num;			//数据搬运次数，n次打包
    u8 **databuf;			//二级指针 动态内存申请
} QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//外部函数
QueueInfo *QUEUE_Init(u32 capacity,u32 length);//循环队列初始化 
u8 QUEUE_Deinit(QueueInfo *q);//循环队列注销

s8 QUEUE_PushByte(QueueInfo *q, u8 byte) ;
s8 QUEUE_Add(QueueInfo *q, u8 *cache,u8 add,u8 length) ;
s8 QUEUE_Push(QueueInfo *q, u8 *cache,u16 length) ;//数据放入队列

s8 QUEUE_Pop(QueueInfo *q,u8 *sendbuf,u16 length);//从队列搬运 n个字节 至发送缓冲区
s8 QUEUE_Clear(QueueInfo *q);//从队列搬运 n个字节 至发送缓冲区

s8 QUEUE_SearchData(QueueInfo *q);//检测队列是否有数据

s8 QUEUE_DataPush(QueueInfo *q, u8 *cache,u8 length,u16 times);//队列插入数据


#endif

