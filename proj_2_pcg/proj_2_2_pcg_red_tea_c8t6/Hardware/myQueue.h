#ifndef __myQueue_H
#define __myQueue_H	 
#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2022/02/08
//�汾��V1.3
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////

//push()���������������ĩ�����һ������Ԫ��
//shift()�����������еĵ�һ��Ԫ��ɾ��
//unshift()���������������ǰ�����һ������Ԫ��
//pop()�����������е����һ��Ԫ��ɾ��


//ѭ������ �ṹ������	
//__align(4) typedef struct {	
typedef struct {	
    u8 Queue_Full_flag;		//���д�����־
    u8 Queue_data_state;	//����״̬
    u32 front, rear;		//frontָ�����ͷ��rearΪ����β��
    u32 capacity,length;	//���д�С������
    //capacity  �������� �����л����������� 
    //length	���г���/��С ����������ݳ��ȣ�
    u16 carry_num;			//���ݰ��˴�����n�δ��
    u8 **databuf;			//����ָ�� ��̬�ڴ�����
} QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//�ⲿ����
QueueInfo *QUEUE_Init(u32 capacity,u32 length);//ѭ�����г�ʼ�� 
u8 QUEUE_Deinit(QueueInfo *q);//ѭ������ע��

s8 QUEUE_PushByte(QueueInfo *q, u8 byte) ;
s8 QUEUE_Add(QueueInfo *q, u8 *cache,u8 add,u8 length) ;
s8 QUEUE_Push(QueueInfo *q, u8 *cache,u16 length) ;//���ݷ������

s8 QUEUE_Pop(QueueInfo *q,u8 *sendbuf,u16 length);//�Ӷ��а��� n���ֽ� �����ͻ�����
s8 QUEUE_Clear(QueueInfo *q);//�Ӷ��а��� n���ֽ� �����ͻ�����

s8 QUEUE_SearchData(QueueInfo *q);//�������Ƿ�������

s8 QUEUE_DataPush(QueueInfo *q, u8 *cache,u8 length,u16 times);//���в�������


#endif

