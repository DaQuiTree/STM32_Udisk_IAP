#include <stdio.h>

#include "sys.h"
#include "delay.h"
#include "flash.h"
#include "usart.h"

#include "led.h"
#include "key.h"

#include "spi.h"
#include "CH376.h"
#include "iap.h"

int main(void)
{     
    //system init
    RCC_Configuration();
    USART1_Init(115200); 
    LED_Init();
    KEY_Init();
    
    //CH376 init
    SPI2_Init();
    if(mInitCH376Host()== USB_INT_SUCCESS)
		printf("CH376 OK!\n"); //显示字符串
    
	//开机没有按下则跳转到APP程序中执行
	if(KEY_Read() == 1)
	{
        iap_load_app(ApplicationAddress);
	}else{ 
        u8 buf[1024] = {0};
        u8 SrcName[64] = BinUpdateName;
        u8 ret = 0;
        u16 sz;
        u32 addr = ApplicationAddress;
        
        LEDn1 = 1;
        printf("Hello Software Compiled Time: %s, %s.\n",__DATE__, __TIME__);	//获取软件编译时间       
        if(!UdiskWaitingConnect()){
            while((ret = UdiskReadFile(SrcName, buf, &sz)) <= 1){
                iap_write_appbin(addr, buf, sz);
                if(ret == 1)break;
                addr += FLASH_PAGE_SIZE;
                delay_ms(100);
            }
            LEDn1 = 0;
            if(ret == 1)
                if(!UdiskWaitingDisconnect())
                    iap_load_app(ApplicationAddress);
        }
        
        while(1);
    }
}


