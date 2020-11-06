/**
  ******************************************************************************
  * @file    crc.c
  * @brief   crc16校验的实现
  *****************************************************************************
  * @note: 
  */
  
#include "stm32f10x.h"
#include "crc.h"

/* 联合体声明&变量声明 */

union crc
{
	u16 word;
	struct 
	{
		u8	lo;
		u8	hi;
	}bytes;
};

union crc crc_chec;

/**
  * @func   void crc_chec_fuc(u8 datain);
  * @brief  CRC16校验子函数
  * @param  datain:
  * @retval 无
  */

static void crc_chec_fuc(u8 datain)
{
  u8 i;
  crc_chec.bytes.lo=crc_chec.bytes.lo^datain;
  for(i=0;i<8;i++)
  {
    if((crc_chec.word & 0x0001)==1)
    {
      crc_chec.word=crc_chec.word>>1;
      crc_chec.word=crc_chec.word^0xa001;
    }
    else
      crc_chec.word=crc_chec.word>>1;
  }
}

/**
  * @func   u16 generic_crc(u16 num, u8 *pbuf);
  * @brief  CRC16校验接口函数
  * @param  num: 校验总数
  *        pbuf: 需要校验的数组
  * @retval CRC16的校验和(word)
  */

u16 generic_crc(u16 num, u8 *pbuf)
{
  u16 i;
  u16 crc_chec_retval = 0x0000;
	
  crc_chec.word=0xffff;
  for(i=0;i<num;i++)
  {
     crc_chec_fuc(pbuf[i]);
  }
  
  crc_chec_retval = (u16)crc_chec.bytes.hi;
  crc_chec_retval <<= 8;
  crc_chec_retval |= (u16)crc_chec.bytes.lo;
  
  return crc_chec_retval;
}
