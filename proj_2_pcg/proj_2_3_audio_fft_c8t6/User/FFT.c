/***********************************************
	经典快速傅里叶计算程序
************************************************/

#include "FFT.H"

#define FFT_Size 256			/*输入序列的大小，在本程序中仅限2的次幂*/
float FFT_Input[FFT_Size];	/*输入序列*/
float FFT_Output[FFT_Size/2];	/*输出序列*/

typedef struct	/*复数结构体*/
{
	float Real;
	float Imag;
}Complex;

Complex x[FFT_Size];
Complex W[FFT_Size];
int size_x=FFT_Size;
float PI=3.1415926535;


void change(void);	/*变址*/
void add(Complex ,Complex ,Complex *);	/*复数加法*/
void sub(Complex ,Complex ,Complex *);	/*复数减法*/
void mul(Complex ,Complex ,Complex *);	/*复数乘法*/

void FFT_Init(void)	/*初始化变换核，定义一个变换核，相当于旋转因子WAP*/
{
	int i;
	for(i=0;i<size_x;i++)
	{
		W[i].Real=cos(2*PI/size_x*i);	/*用欧拉公式计算旋转因子*/
		W[i].Imag=-1*sin(2*PI/size_x*i);
	}
}

//输入大小为FFT_Size,输出大小为FFT_Size/2
void FFT(void)	/*快速傅里叶变换*/
{
	Complex up,down,product;
	int i=0,j=0,k=0,l=0;
	for(i=0;i<size_x;i++)
	{
		x[i].Real=FFT_Input[i];
		x[i].Imag=0;
	}
	change();	/*调用变址函数*/
	for(i=0;i< log(size_x)/log(2) ;i++)	/*一级蝶形运算 stage */
	{   
		l=1<<i;
		for(j=0;j<size_x;j+= 2*l )	/*一组蝶形运算 group,每个group的蝶形因子乘数不同*/
		{            
			for(k=0;k<l;k++)	/*一个蝶形运算 每个group内的蝶形运算*/
			{       
				mul(x[j+k+l],W[size_x*k/2/l],&product);
				add(x[j+k],product,&up);
				sub(x[j+k],product,&down);
				x[j+k]=up;
				x[j+k+l]=down;
			}
		}
	}
	for(i=0;i<size_x/2;i++)
	{
		FFT_Output[i]=sqrt(x[i].Real*x[i].Real+x[i].Imag*x[i].Imag);
	}
}

void change(void)	/*变址计算，将x(n)码位倒置*/
{
	Complex temp;
	unsigned short i=0,j=0,k=0;
	float t=0;
	for(i=0;i<size_x;i++)
	{
		k=i;j=0;
		t=(log(size_x)/log(2));
		while( (t--)>0 )	//利用按位与以及循环实现码位颠倒
		{
			j=j<<1;
			j|=(k & 1);
			k=k>>1;
		}
		if(j>i)	//将x(n)的码位互换
		{
			temp=x[i];
			x[i]=x[j];
			x[j]=temp;
		}
	}
}

void add(Complex a,Complex b,Complex *c)	/*复数加法的定义*/
{
	c->Real=a.Real+b.Real;
	c->Imag=a.Imag+b.Imag;
}

void sub(Complex a,Complex b,Complex *c)	/*复数减法的定义*/
{
	c->Real=a.Real-b.Real;
	c->Imag=a.Imag-b.Imag;
}

void mul(Complex a,Complex b,Complex *c)	/*复数乘法的定义*/
{
	c->Real=a.Real*b.Real - a.Imag*b.Imag;
	c->Imag=a.Real*b.Imag + a.Imag*b.Real;
}
