#ifndef __FFT_H__
#define __FFT_H__
#include <math.h>

extern float FFT_Input[];	/*��������*/
extern float FFT_Output[];	/*�������*/

void FFT_Init(void); /*��ʼ���任��*/
void FFT(void);     /*���ٸ���Ҷ�任*/


#endif
