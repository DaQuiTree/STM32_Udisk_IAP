#include "sys.h"
#include "flash.h"
#include "iap.h"
#include "delay.h"
#include <stdio.h>

#include "ch376.h"
#include "filesys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IAP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	

iapfun jump2app; 
u16 iapbuf[512];   
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{	
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];
		dfu+=2;//偏移2个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0;	
            FLASH_WriteNWord(iapbuf, fwaddr, 512);
			fwaddr+=1024;//偏1024  16=2*8.所以要乘以2.
		}
	}
    if(i)FLASH_WriteNWord(iapbuf, fwaddr, i);//将最后的一些内容字节写进去. 
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
    
    while(1);
}		 

//CH376相关

u8 UdiskWaitingConnect(void)
{
    u8 s,i;
    
    while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) 
        delay_ms(100);  // 检查U盘是否连接,等待U盘拔出
    delay_ms(200); //每次操作后必要的延时
    for (i = 0; i < 100; i ++ ){ 
        delay_ms( 50 );
        s = CH376DiskMount( );  //初始化磁盘并测试磁盘是否就绪.   
        if ( s == USB_INT_SUCCESS ) /* 准备好 */
            break;                                          
        else if ( s == ERR_DISK_DISCON )/* 检测到断开,重新检测并计时 */
            return USB_INT_DISK_ERR;  
        if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
            break; 
    }
    printf(" U DISK INIT!\r\n"); //显示字符串
    delay_ms(200); //每次操作后必要的延时
    
    return 0;
}

u8 UdiskWaitingDisconnect(void)
{
    printf("Bin file has been updated! Please remove the Udisk.\r\n"); //显示字符串
    while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) 
        delay_ms(500);  // 检查U盘是否连接,等待U盘拔出
    delay_ms(200); //每次操作后必要的延时
    printf(" U DISK REMOVED!\r\n"); //显示字符串
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












