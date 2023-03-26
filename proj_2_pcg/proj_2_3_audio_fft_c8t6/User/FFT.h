#ifndef __FFT_H__
#define __FFT_H__
#include <math.h>

extern float FFT_Input[];	/*输入序列*/
extern float FFT_Output[];	/*输出序列*/

void FFT_Init(void); /*初始化变换核*/
void FFT(void);     /*快速傅里叶变换*/


#endif
