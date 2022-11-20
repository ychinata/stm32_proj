#include "myQueue.h"
#include "malloc.h"
#include <string.h>
#include "usart.h"	 


/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2022/02/08
//�汾��V1.3
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//QueueInfo�洢��ǰ���е�һЩ��Ϣ��
//dataΪ��̬����������Ķ��пռ䣬
//frontָ�����ͷ��
//rearΪ����β����
//capacityΪ���п����ɵĴ�С��
/////////////////////////////////////////////////////////////////////////////////////////////
//����ʹ��QueueInfo�洢��ǰ���е�һЩ��Ϣ��dataΪ��̬����������Ķ��пռ䣬
//frontָ�����ͷ��rearΪ����β����capacityΪ���п����ɵĴ�С����ʼ��ʱ��
//��front��rear����Ϊ0��������ѭ��ʹ�ö��пռ䣬�������capacity��Ԫ��ʱ��
//��ʱfront�����˶�����������Ҫ�������õ�0λ�ã��������޷��жϵ�ǰ�����������ǿա�
//һ�ֽ���취�ǣ���ʹ��capacity-1���ռ���д洢��ʼ�ձ���front��rear֮�����
//��С��1�����ÿռ䣬�˷����������ͷ�ڵ�������ͬ��֮�

/* Recyle Queue Operator Push and Pop Sketch
 * Queue Size: 4
 * Capacity  : 7
 * front       rear          front       rear       rear   front
 *   |          |              |          |          |       |
 *   1  2  3  4[ ][ ][ ]    [ ]1  2  3  4[ ][ ]    4[ ][ ][ ]1  2  3
 *          (1)                    (2)                   (3)
 *
 * PUSH : 5  ����5
 * front          rear       front          rear      rear front
 *   |             |           |             |          |    |
 *   1  2  3  4  5[ ][ ]    [ ]1  2  3  4  5[ ]    4  5[ ][ ]1  2  3
 *          (1+)                   (2+)                  (3+)
 *
 * POP  : ȡ��
 *    front    rear             front    rear       rear      front
 *      |       |                 |       |          |          |
 *   [ ]2  3  4[ ][ ][ ]    [ ][ ]2  3  4[ ][ ]    4[ ][ ][ ][ ]2  3
 *          (1-)                    (2-)                  (3-)
 * */
 
 //����ͬ������Ϊ7����СΪ4��ѭ�����У�������������������Ե��ж϶����Ƿ�Ϊ�ա�
 //�����Ƿ��п��ÿռ�ʱ������ֱ���ж�front��rear�Ĵ�С����ˣ���������Ӻͳ���ʱ��
 //ҲҪ��Բ�ͬ������д���ÿ�����ʱ����Ԫ�ظ�����rear��������rear����һλ��
 //ע���ж϶���Ϊ�ջ����������ұ�֤�䲻����capacity��������Ӷ�ͷɾ����ֻ�轫front����ƶ����ɡ�
///////////////////////////////////////////////////////////////////////////////////////////// 
 


//ѭ�����г�ʼ�� ���� ���ȣ��˲��ַ����ڲ�SARM
QueueInfo *queue_init(u32 capacity,u32 length)//ѭ�����г�ʼ�� 
{
    u32 i;
    QueueInfo *q;
    u8 res=0;
    if (length == 0 ||capacity==0) return NULL;	//��������
	
    q = (QueueInfo*)mymalloc(SRAMIN,sizeof(QueueInfo)); //���нṹ
    if(q == NULL)	return NULL; //�ڴ�����ʧ��
	
    q->Queue_data_state=0; //����״̬
    q->carry_num=0;		//���˼���
    q->front = q->rear = 0; //��ʼ��ʱ����front��rear����Ϊ0��
    q->length = length; 		//���г���/��С ����������ݳ��ȣ�  
    q->capacity = capacity; //capacity  �������� �����л�����������
    q->Queue_Full_flag=0; //���пձ�־

    q->databuf = (u8 **)mymalloc(SRAMIN,q->capacity *4);//���붯̬�ڴ�ռ䣬�������� 32λ��Ƭ����ָ��ռ��4�ֽڿռ�
    if(q->databuf == NULL) {	
            myfree(SRAMIN,q);//�ͷ��ڴ�
            return NULL; //�ڴ�����ʧ��
    }
		
    for(i=0;i<q->capacity;i++) { //��������    	
        *(q->databuf+i) = (u8*)mymalloc(SRAMIN,q->length); //���� n ����СΪ length ������ 
        if(*(q->databuf+i) == NULL) {        		
            res=1;
            break;
        }
        memset( *(q->databuf + i),0,length);//��0
        //Main_printf(" (q->databuf)+%d  %x ",i, ((q->databuf)+i)   );	 //ָ������  ָ����32λ��
        //Main_printf("*((q->databuf)+%d)  %x \r\n",i, *((q->databuf)+i)   );	//����ָ��ĵ�ַ
        
        //���л���������֡ͷ�ȹ̶�λ��������ݺ�ֱ��У�鼴��
        //Wifi_QUEUEbuf_Init(*(q->databuf + i),length);//��ʼ�����л�����
    }
		
    if(res) {//�ڴ�����ʧ��    
        while(i) {        
                myfree(SRAMIN,*(q->databuf+i));
                i--;
        }
        myfree(SRAMIN,q->databuf);//�ͷ��ڴ�
        myfree(SRAMIN,q);//�ͷ��ڴ�
        return NULL;
    }
    
    return q;//���س�ʼ���Ķ���ָ��		
}

u8 queue_Deinit(QueueInfo *q)//ѭ������ע��
{
    u16 i;
    if(q==NULL)		return 1; //ָ����Ч
    for(i=0;i<q->capacity;i++)
    {
        myfree(SRAMIN,*(q->databuf+i));//�ͷ��ڴ�
    }
    myfree(SRAMIN,q->databuf);//�ͷ��ڴ�
    myfree(SRAMIN,q);//�ͷ��ڴ�
    return 0;
}



//�����׸�λ�����һ���ֽ�
//ע���ж϶���Ϊ�ջ����������ұ�֤�䲻����capacity��
s8 queue_push_byte(QueueInfo *q, u8 byte) 
{	
    u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //��������
    {
        q->front = (q->front+1) % q->capacity;//��ͷ������ɾ�����Ƚ��������
        //return 0;//����
    }	
		
    *(*(q->databuf + q->rear)+0)=byte;

    return 1;    // return push count 		
}
//��������׷��һ������
//ע���ж϶���Ϊ�ջ����������ұ�֤�䲻����capacity��
s8 queue_add(QueueInfo *q, u8 *cache,u8 add,u8 length) 
{	
    u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //��������
    {
        //q->front = (q->front+1) % q->capacity;//��ͷ������ɾ�����Ƚ��������
        return 0;//����
    }
    memcpy( *(q->databuf + q->rear)+add,cache,length);//����  N ���ֽ�			

    q->rear = (q->rear+1) % q->capacity; //ȡ���ݣ���ͷ�����������ݣ���β����	
    return 1;    // return push count 		
}

//�����ǰ�������
//ÿ�����ʱ����Ԫ�ظ�����rear��������rear����һλ��
//ע���ж϶���Ϊ�ջ����������ұ�֤�䲻����capacity��
s8 queue_push(QueueInfo *q, u8 *cache,u16 length) 
{	
    u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //��������
    {
        q->front = (q->front+1) % q->capacity;//��ͷ������ɾ�����Ƚ��������
        q->Queue_Full_flag=1; //���пձ�־
        //return 0;//����
    }
    else
    {
        q->Queue_Full_flag=0; //���пձ�־
    }
    memcpy( *(q->databuf + q->rear),cache,length);//����  N ���ֽ�			

    q->rear = (q->rear+1) % q->capacity; //ȡ���ݣ���ͷ�����������ݣ���β����	
    return 1;    // return push count 		
}
////ע������û�����Ӷ�ͷ����Ҫ�ⲿ���ӣ��ص������ʹ��
//s8 queue_push2(QueueInfo *q, u8 *cache,u16 length) 
//{	
//		u16 i;
//    if (q==NULL) return -1; // need queue
//    if ((q->rear+1) % q->capacity == q->front) //��������
//		{
//				q->front = (q->front+1) % q->capacity;//��ͷ������ɾ�����Ƚ��������	
//				q->Queue_Full_flag=1; //���пձ�־
//				//return 0;//����
//		}
//		else
//		{
//				q->Queue_Full_flag=0; //���пձ�־
//		}
//		memcpy( *(q->databuf + q->rear),cache,length);//����  N ���ֽ�			

//    //q->rear = (q->rear+1) % q->capacity; //ȡ���ݣ���ͷ�����������ݣ���β����	
//    return 1;    // return push count 		
//}


//������ĺ���Ƴ���
//����ȡ����
s8 queue_pop(QueueInfo *q,u8 *sendbuf,u16 length)//�Ӷ��а��� n���ֽ� �����ͻ�����
{	
    if (q==NULL) return -1; // need queue
    if (q->front==q->rear) 		return 0;//����Ϊ��

    memcpy(sendbuf,*(q->databuf + q->front),length);//copy n ���ֽ�

    q->front = (q->front+1) % q->capacity;//ȡ���ݣ���ͷ�����������ݣ���β����
    return 1; //ȡ������
}
//////���е�һ���ֽ������ݳ��ȣ����������˵�һ���ֽڣ��ص������ʹ��
////s8 queue_pop(QueueInfo *q,u8 *sendbuf,u16 length)//�Ӷ��а��� n���ֽ� �����ͻ�����
////{	
////	  if (q==NULL) return -1; // need queue
////	  if (q->front==q->rear) return 0;//����Ϊ��
////	
////		memcpy(sendbuf,*(q->databuf + q->front)+1,length);//copy n ���ֽ�
////	
////    q->front = (q->front+1) % q->capacity;//ȡ���ݣ���ͷ�����������ݣ���β����
////    return 1; //ȡ������
////}


//�������Ƿ�������
//1 ������ 0 û������
s8 serch_queue_data(QueueInfo *q)
{
    if (q==NULL) return -1; //need queue
    if (q->front==q->rear) return 0;//����Ϊ��
    else return 1;//������
}


//�������
s8 queue_clear(QueueInfo *q)
{
    u16 i;

    if(q == NULL)	return -1; //need queue
    q->carry_num=0;
    q->front = q->rear = 0; //��ʼ��ʱ����front��rear����Ϊ0��
    q->Queue_Full_flag=0; //���пձ�־

    for(i=0;i<q->capacity;i++) //��������
    {		
        memset( *(q->databuf + i),0,q->length);//��0	
    }
    return 1;
}


//���еĲ��룬˳������л������������� 
//����������ָ�룬������Դ��Ҫ���˵����ݳ��ȣ�Ҫ���˵Ĵ���
s8 queue_data_push(QueueInfo *q, u8 *cache,u8 length,u16 times) 
{	
    u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //��������
    {
        q->front = (q->front+1) % q->capacity;//��ͷ������ɾ�����Ƚ��������
        q->Queue_Full_flag=1;
        //Main_printf("m ");
        //return 0;//����
    }
    else
    {
        q->Queue_Full_flag=0;
    }		
    memcpy( *(q->databuf + q->rear)+ q->carry_num *length,cache,length);//����  length ���ֽ�

    q->carry_num++;//���ݰ��˴�������			
    if(q->carry_num == times) //����N������
    {
        q->carry_num=0;
        q->rear = (q->rear+1) % q->capacity; //ȡ���ݣ���ͷ�����������ݣ���β����	
    
        return 1; //�������һ֡����
    }
    else
        return 0; 
}
