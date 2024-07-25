#include "stm32f10x.h"

/*
	STM32系列中容量设备的主存储器Flash
		地址：0x08000000~0x0801FFFF
		大小：128K
		分页：共128页，每页1K
		
	STM32F103C8T6的主存储器Flash
		地址：0x08000000~0x0800FFFF
		大小：64K
		分页：共64页，每页1K
		
	页地址的规律：0x08000000 0x08000400 0x08000800 0x08000C00
				  0x08001000 0x08001400 0x08001800 0x08001C00
				  ...
				  0x0800F000 0x0800F400 0x0800F800 0x0800FC00
*/

/**
  * @brief  Flash读取一个半字
  * @param  Address 指定的半字地址，必须为2的倍数
  * @retval 指定地址下的数据
  */
uint16_t Flash_Read(uint32_t Address)
{
	return *(__IO uint16_t *)Address;
}

/**
  * @brief  Flash写一个半字
  * @param  Address 指定的半字地址，必须为2的倍数
  * @param  Data 要写入的数据
  * @retval 无
  * @note   写入的地址必须是已经被擦除的地址
  */
void Flash_Write(uint32_t Address,uint16_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address,Data);
	FLASH_Lock();
}

/**
  * @brief  Flash 擦除一页
  * @param  PageAddress 指定的页地址，必须与页的首地址对其
  * @retval 无
  * @note   擦除后，所有数据均为0xFF
  */
void Flash_ErasePage(uint32_t PageAddress)
{
	FLASH_Unlock();
	FLASH_ErasePage(PageAddress);
	FLASH_Lock();
}

/**
  * @brief  Flash读取一页数据
  * @param  PageAddress 指定的页地址，必须与页的首地址对其
  * @param  Data 指定的页地址下的数据（输出参数）
  * @param  Count 读取数据的个数，目的是可以只读一页前面的部分数据而不必一次性读完所有数据
  * @retval 无
  */
void Flash_ReadPage(uint32_t PageAddress,uint16_t *Data,uint16_t Count)
{
	uint16_t i;
	for(i=0;i<Count;i++)
	{
		Data[i]=Flash_Read(PageAddress);
		PageAddress+=2;
	}
}

/**
  * @brief  Flash写一页数据
  * @param  PageAddress 指定的页地址，必须与页的首地址对其
  * @param  Data 要写入的数据
  * @param  Count 写入数据的个数，目的是可以只写一页前面的部分数据而不必一次性写完所有数据
  * @retval 无
  * @note   写入前必须擦除指定页
  */
void Flash_WritePage(uint32_t PageAddress,uint16_t *Data,uint16_t Count)
{
	uint16_t i;
	FLASH_Unlock();
	for(i=0;i<Count;i++)
	{
		FLASH_ProgramHalfWord(PageAddress,Data[i]);
		PageAddress+=2;
	}
	FLASH_Lock();
}

