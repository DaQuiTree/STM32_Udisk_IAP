/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： flash.h
  * @作者       ： strongerHuang
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月19日
  * @摘要       ： 内核Flash编程头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _FLASH_H
#define _FLASH_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f10x.h"


/* 类型定义 ------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* 宏定义 --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)        //页大小   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)        //类型大小 - 2字节
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01

/* 函数申明 ------------------------------------------------------------------*/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);


#endif /* _FLASH_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
