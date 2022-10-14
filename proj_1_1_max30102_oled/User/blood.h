#ifndef _BLOOD_H
#define _BLOOD_H
#include "stm32f10x.h"                  // Device header
#include "MAX30102.h"
#include "algorithm.h"
#include "math.h"
#include "OLED_IIC.h"

typedef enum {
	BLD_NORMAL,		//����
	BLD_ERROR,		//������
	
} BloodState;//ѪҺ״̬

typedef struct {
	int 		heart;		//��������
	float 			SpO2;			//Ѫ������
} BloodData;


void BLOOD_Data_Translate(void);
void BLOOD_Data_Update(void);
void BLOOD_Loop(void);

#endif


