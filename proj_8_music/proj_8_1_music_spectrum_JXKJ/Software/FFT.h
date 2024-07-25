#ifndef __FFT_H
#define __FFT_H
#include <math.h>

extern float FFT_Input[];	/*输入序列*/
extern float FFT_Output[];	/*输出序列*/

//#define FFT_NUM 128
#define FFT_Size 128			/*输入序列的大小，在本程序中仅限2的次幂*/


void FFT_Init(void); /*初始化变换核*/
void FFT(void);     /*快速傅里叶变换*/


#endif
