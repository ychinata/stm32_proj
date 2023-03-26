/***********************************************
	������ٸ���Ҷ�������
************************************************/

#include "FFT.H"

#define FFT_Size 256			/*�������еĴ�С���ڱ������н���2�Ĵ���*/
float FFT_Input[FFT_Size];	/*��������*/
float FFT_Output[FFT_Size/2];	/*�������*/

typedef struct	/*�����ṹ��*/
{
	float Real;
	float Imag;
}Complex;

Complex x[FFT_Size];
Complex W[FFT_Size];
int size_x=FFT_Size;
float PI=3.1415926535;


void change(void);	/*��ַ*/
void add(Complex ,Complex ,Complex *);	/*�����ӷ�*/
void sub(Complex ,Complex ,Complex *);	/*��������*/
void mul(Complex ,Complex ,Complex *);	/*�����˷�*/

void FFT_Init(void)	/*��ʼ���任�ˣ�����һ���任�ˣ��൱����ת����WAP*/
{
	int i;
	for(i=0;i<size_x;i++)
	{
		W[i].Real=cos(2*PI/size_x*i);	/*��ŷ����ʽ������ת����*/
		W[i].Imag=-1*sin(2*PI/size_x*i);
	}
}

//�����СΪFFT_Size,�����СΪFFT_Size/2
void FFT(void)	/*���ٸ���Ҷ�任*/
{
	Complex up,down,product;
	int i=0,j=0,k=0,l=0;
	for(i=0;i<size_x;i++)
	{
		x[i].Real=FFT_Input[i];
		x[i].Imag=0;
	}
	change();	/*���ñ�ַ����*/
	for(i=0;i< log(size_x)/log(2) ;i++)	/*һ���������� stage */
	{   
		l=1<<i;
		for(j=0;j<size_x;j+= 2*l )	/*һ��������� group,ÿ��group�ĵ������ӳ�����ͬ*/
		{            
			for(k=0;k<l;k++)	/*һ���������� ÿ��group�ڵĵ�������*/
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

void change(void)	/*��ַ���㣬��x(n)��λ����*/
{
	Complex temp;
	unsigned short i=0,j=0,k=0;
	float t=0;
	for(i=0;i<size_x;i++)
	{
		k=i;j=0;
		t=(log(size_x)/log(2));
		while( (t--)>0 )	//���ð�λ���Լ�ѭ��ʵ����λ�ߵ�
		{
			j=j<<1;
			j|=(k & 1);
			k=k>>1;
		}
		if(j>i)	//��x(n)����λ����
		{
			temp=x[i];
			x[i]=x[j];
			x[j]=temp;
		}
	}
}

void add(Complex a,Complex b,Complex *c)	/*�����ӷ��Ķ���*/
{
	c->Real=a.Real+b.Real;
	c->Imag=a.Imag+b.Imag;
}

void sub(Complex a,Complex b,Complex *c)	/*���������Ķ���*/
{
	c->Real=a.Real-b.Real;
	c->Imag=a.Imag-b.Imag;
}

void mul(Complex a,Complex b,Complex *c)	/*�����˷��Ķ���*/
{
	c->Real=a.Real*b.Real - a.Imag*b.Imag;
	c->Imag=a.Real*b.Imag + a.Imag*b.Real;
}
