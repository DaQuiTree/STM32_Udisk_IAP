/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� flash.h
  * @����       �� strongerHuang
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��19��
  * @ժҪ       �� �ں�Flash���ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _FLASH_H
#define _FLASH_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f10x.h"


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* �궨�� --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)        //ҳ��С   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)        //���ʹ�С - 2�ֽ�
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01

/* �������� ------------------------------------------------------------------*/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);


#endif /* _FLASH_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
