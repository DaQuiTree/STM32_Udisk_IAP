#ifndef __IAP_H__
#define __IAP_H__

#include "sys.h"  

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IAP ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.

#define BinUpdateName "\\UPDATE.BIN"
#define ApplicationAddress 0x08008000 	    //��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
											//����0X08000000~0X0800FFFF�Ŀռ�ΪIAPʹ��

void iap_load_app(u32 appxaddr);			//ִ��flash�����app����
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//��ָ����ַ��ʼ,д��bin

#define UDISKREADLEN 1024

u8 UdiskWaitingConnect(void);
u8 UdiskWaitingDisconnect(void);
s8 UdiskReadFile(u8 *SrcFileName, u8 *buf, u16 *ThisLen);

#endif







































