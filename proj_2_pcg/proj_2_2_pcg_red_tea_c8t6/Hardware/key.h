#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"



#define KEY1  PAin(8)	



#define	KEYDOWNMINTIME		10000	//�̰���ʱ��1s
#define	KEYDOWNLONGTIME		30000	//������3s


#define		KEYUNKNOW			0 	//����״̬
#define		KEYSHORTDOWN	1
#define		KEYLONGDOWN		2

extern volatile u8 KeyMask; //�������
extern volatile u8 KeyDat;
extern u8 g_KeyType;


void KEY_IO_Init(void);	
void Key_Scan(void);



#endif
