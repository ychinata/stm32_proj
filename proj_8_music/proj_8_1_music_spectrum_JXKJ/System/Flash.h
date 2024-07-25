#ifndef __FLASH_H
#define __FLASH_H

uint16_t Flash_Read(uint32_t Address);
void Flash_Write(uint32_t Address,uint16_t Data);

void Flash_ErasePage(uint32_t PageAddress);
void Flash_ReadPage(uint32_t PageAddress,uint16_t *Data,uint16_t Count);
void Flash_WritePage(uint32_t PageAddress,uint16_t *Data,uint16_t Count);

#endif
