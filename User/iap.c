#include "sys.h"
#include "flash.h"
#include "iap.h"
#include "delay.h"
#include <stdio.h>

#include "ch376.h"
#include "filesys.h"

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

iapfun jump2app; 
u16 iapbuf[512];   
//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{	
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];
		dfu+=2;//ƫ��2���ֽ�
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0;	
            FLASH_WriteNWord(iapbuf, fwaddr, 512);
			fwaddr+=1024;//ƫ1024  16=2*8.����Ҫ����2.
		}
	}
    if(i)FLASH_WriteNWord(iapbuf, fwaddr, i);//������һЩ�����ֽ�д��ȥ. 
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
    
    while(1);
}		 

//CH376���

u8 UdiskWaitingConnect(void)
{
    u8 s,i;
    
    while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) 
        delay_ms(100);  // ���U���Ƿ�����,�ȴ�U�̰γ�
    delay_ms(200); //ÿ�β������Ҫ����ʱ
    for (i = 0; i < 100; i ++ ){ 
        delay_ms( 50 );
        s = CH376DiskMount( );  //��ʼ�����̲����Դ����Ƿ����.   
        if ( s == USB_INT_SUCCESS ) /* ׼���� */
            break;                                          
        else if ( s == ERR_DISK_DISCON )/* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
            return USB_INT_DISK_ERR;  
        if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
            break; 
    }
    printf(" U DISK INIT!\r\n"); //��ʾ�ַ���
    delay_ms(200); //ÿ�β������Ҫ����ʱ
    
    return 0;
}

u8 UdiskWaitingDisconnect(void)
{
    printf("Bin file has been updated! Please remove the Udisk.\r\n"); //��ʾ�ַ���
    while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) 
        delay_ms(500);  // ���U���Ƿ�����,�ȴ�U�̰γ�
    delay_ms(200); //ÿ�β������Ҫ����ʱ
    printf(" U DISK REMOVED!\r\n"); //��ʾ�ַ���
    return(0);
}
      
s8 UdiskReadFile(u8 *SrcFileName, u8 *buf, u16 *ThisLen)
{ 
	UINT8	s;
    static UINT32 FileSize = 0, ByteCount = 0;
    
    if ( ByteCount == 0 ) { 
        printf( "OpenSrc\n" );  
        s = CH376FileOpenPath( SrcFileName );  
        if ( s != USB_INT_SUCCESS ) 
            return( s );
        FileSize = CH376GetFileSize( ); 
        printf( "SrcFileSz=%ld\n", FileSize );
    }else {  
        s = CH376ByteLocate( ByteCount ); 
        if ( s != USB_INT_SUCCESS ) 
            return( s );
    }
    
    s = CH376ByteRead( buf, UDISKREADLEN, ThisLen ); 
    if ( s != USB_INT_SUCCESS ) 
        return( s );
    
    ByteCount += *ThisLen;
    if (*ThisLen < UDISKREADLEN ) { //finished
        if ( ByteCount != FileSize ){
            printf( "Error on SourceFile reading" );//size not equal,failed
            FileSize = ByteCount = 0;
            return(2);
        }
        printf( "Read binfile finished, Total read=%ld\n", ByteCount);
        FileSize = ByteCount = 0;
        CH376FileClose( TRUE );  
        
        return(1); //finished
    }
    
	return(0); //should continue
}












