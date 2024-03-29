#include "MAX30102.h"
#include "systick.h"
uint16_t g_fifoRed;
uint16_t g_fifoIr;
	
void MAX30102_GPIO(void)
{	
	RCC_APB2PeriphClockCmd(MAX30102_INTPin_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin		=	MAX30102_INTPin_Pin;
	GPIO_Init(MAX30102_INTPin_PORT,&GPIO_InitStruct);
	
}

uint8_t MAX30102_Reset(void)
{
	if(IIC_Write_Byte(MAX30102_Device_address,REG_MODE_CONFIG, 0x40))
        return 1;
    else
        return 0;    
}

void MAX30102_Config(void)
{
	IIC_Write_Byte(MAX30102_Device_address,REG_INTR_ENABLE_1,0xc0);//// INTR setting
	IIC_Write_Byte(MAX30102_Device_address,REG_INTR_ENABLE_2,0x00);//
	IIC_Write_Byte(MAX30102_Device_address,REG_FIFO_WR_PTR,0x00);//FIFO_WR_PTR[4:0]
	IIC_Write_Byte(MAX30102_Device_address,REG_OVF_COUNTER,0x00);//OVF_COUNTER[4:0]
	IIC_Write_Byte(MAX30102_Device_address,REG_FIFO_RD_PTR,0x00);//FIFO_RD_PTR[4:0]
	
	IIC_Write_Byte(MAX30102_Device_address,REG_FIFO_CONFIG,0x0f);//sample avg = 1, fifo rollover=false, fifo almost full = 17
	IIC_Write_Byte(MAX30102_Device_address,REG_MODE_CONFIG,0x03);//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	IIC_Write_Byte(MAX30102_Device_address,REG_SPO2_CONFIG,0x27);	// SPO2_ADC range = 4096nA, SPO2 sample rate (50 Hz), LED pulseWidth (400uS)  
	IIC_Write_Byte(MAX30102_Device_address,REG_LED1_PA,0x32);//Choose value for ~ 10mA for LED1
	IIC_Write_Byte(MAX30102_Device_address,REG_LED2_PA,0x32);// Choose value for ~ 10mA for LED2
	IIC_Write_Byte(MAX30102_Device_address,REG_PILOT_PA,0x7f);// Choose value for ~ 25mA for Pilot LED
}

void MAX30102_Read_Fifo(void)
{
	uint16_t un_temp;
	g_fifoRed = 0;
	g_fifoIr = 0;
	uint8_t ach_i2c_data[6];

	//read and clear status register
	IIC_Read_Byte(MAX30102_Device_address, REG_INTR_STATUS_1);
	IIC_Read_Byte(MAX30102_Device_address, REG_INTR_STATUS_2);

	ach_i2c_data[0] = REG_FIFO_DATA;

	IIC_Read_Array(MAX30102_Device_address, REG_FIFO_DATA,ach_i2c_data,6);

	// ach_i2c_data[0-2]��װg_fifoRed
	un_temp = ach_i2c_data[0];
	un_temp <<= 14;
	g_fifoRed += un_temp;
	un_temp = ach_i2c_data[1];
	un_temp <<= 6;
	g_fifoRed += un_temp;
	un_temp = ach_i2c_data[2];
	un_temp >>= 2;
	g_fifoRed+=un_temp;
	if (g_fifoRed <= 10000) {
		g_fifoRed = 0;
	}	

	// ach_i2c_data[3-5]��װg_fifoIr
	un_temp = ach_i2c_data[3];
	un_temp <<= 14;
	g_fifoIr += un_temp;
	un_temp = ach_i2c_data[4];
	un_temp <<= 6;
	g_fifoIr += un_temp;
	un_temp = ach_i2c_data[5];
	un_temp >>= 2;
	g_fifoIr += un_temp;
	
	if (g_fifoIr< = 10000) {
		g_fifoIr = 0;
	}

}


