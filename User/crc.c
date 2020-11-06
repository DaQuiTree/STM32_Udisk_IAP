/**
  ******************************************************************************
  * @file    crc.c
  * @brief   crc16У���ʵ��
  *****************************************************************************
  * @note: 
  */
  
#include "stm32f10x.h"
#include "crc.h"

/* ����������&�������� */

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
  * @brief  CRC16У���Ӻ���
  * @param  datain:
  * @retval ��
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
  * @brief  CRC16У��ӿں���
  * @param  num: У������
  *        pbuf: ��ҪУ�������
  * @retval CRC16��У���(word)
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
