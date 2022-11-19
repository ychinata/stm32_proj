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
__align(4) typedef struct {
	
		u8 Queue_Full_flag;
	
		u8 Queue_data_state;			//����״̬
    u32 front, rear;			//frontָ�����ͷ��rearΪ����β��
    u32 capacity,length;	//���д�С������
		//capacity  �������� �����л����������� 
		//length		���г���/��С ����������ݳ��ȣ�
	
		u16 carry_num;				//���ݰ��˴�����n�δ��
	
	  u8 **databuf;					//����ָ�� ��̬�ڴ�����
}QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//�ⲿ����
QueueInfo *queue_init(u32 capacity,u32 length);//ѭ�����г�ʼ�� 
u8 queue_Deinit(QueueInfo *q);//ѭ������ע��

s8 queue_push_byte(QueueInfo *q, u8 byte) ;
s8 queue_add(QueueInfo *q, u8 *cache,u8 add,u8 length) ;
s8 queue_push(QueueInfo *q, u8 *cache,u16 length) ;//���ݷ������

s8 queue_pop(QueueInfo *q,u8 *sendbuf,u16 length);//�Ӷ��а��� n���ֽ� �����ͻ�����
s8 queue_clear(QueueInfo *q);//�Ӷ��а��� n���ֽ� �����ͻ�����



s8 serch_queue_data(QueueInfo *q);//�������Ƿ�������

s8 queue_data_push(QueueInfo *q, u8 *cache,u8 length,u16 times);//���в�������


#endif


 
 
 
 
 
 
 
 