#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"



//STM32F103C8T6
//	FLASH 64K ������
//	SRAM	20K
//STM32F103RCT6
//	FLASH 256K ������
//	SRAM	48K
//STM32F103ZET6
//	FLASH 512K ������
//	SRAM	64K
//STM32L452CEU6
//	FLASH 512K ������
//	SRAM	160K 
//	80MHz

//STM32F4 �ڲ��ڴ��Ϊ����飺 
//1����ͨ�ڴ棨�ַ�Ϊ��Ҫ�ڴ�͸����ڴ棬��ַ�ӣ�0X20000000��ʼ����128KB�����ⲿ���ڴ��κ����趼���Է��ʡ� 
//2��CCM �ڴ棨��ַ�ӣ�0X10000000��ʼ����64KB�����ⲿ���ڴ�� CPU ���Է��ʣ�DMA֮��Ĳ�����ֱ�ӷ��ʣ�ʹ��ʱ���ر�ע�⣡��
//STM32F405RGT6
//	FLASH 1024K 
//	SRAM	192K 
//	168MHz
////////////////////////////////////////////////////////////////////////////////////////////////
#define SRAMBANK 	1	//����֧�ֵ�SRAM����.	
////////////////////////////////////////////////////////////////////////////////////////////////
//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE		32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			17*1024  						//�������ڴ� 10K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С
////mem2�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!) STM32F4����
//#define MEM2_BLOCK_SIZE		32  	  						//�ڴ���СΪ32�ֽ�
//#define MEM2_MAX_SIZE			60 *1024  						//�������ڴ�60K
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
////////////////////////////////////////////////////////////////////////////////////////////////	
#ifndef NULL
#define NULL 0
#endif

//�����ڴ��
#define SRAMIN			0	//�ڲ��ڴ��
//#define SRAMCCM  		1	//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)
//#define SRAMEX  		2	//�ⲿ�ڴ��

//////////////////////////////////////////////////////////////////////////////////////////////// 
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);					//��ʼ��
	u8 (*perused)(u8);		  	    	//�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��
////////////////////////////////////////////////////////////////////////////////////////////////
void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
u32 my_mem_malloc(u8 memx,u32 size);	//�ڴ����(�ڲ�����)
u8 my_mem_free(u8 memx,u32 offset);		//�ڴ��ͷ�(�ڲ�����)
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
u8 mem_perused(u8 memx);				//����ڴ�ʹ����(��/�ڲ�����)  
void mem_init(u8 memx);				//�ڴ�����ʼ������(��/�ڲ�����)

//void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)



#endif






