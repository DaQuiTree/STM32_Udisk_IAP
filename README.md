# STM32_Udisk_IAP
基于洋桃1号开发板 CH376芯片实现的U盘IAP升级

## 描述
按住KEY1并reset，LED1亮起，说明进入IAP模式。当U盘插入开发板USB接口时，程序扫描U盘根目录UPDATE.BIN文件，自动完成升级。升级完成LED1熄灭，此时拔出U盘，程序跳转至APP运行。

## 洋桃1号开发板发现的问题 
软件：

1、修改 ch376.h #define CH376_INT GPIO_Pin_8

2、修改 ch376.c Query376Interrupt() i = 1-GPIO_ReadInputDataBit(CH376_INTPORT,CH376_INT);  因为手册上说:CH376 芯片 INT#引脚输出的中断请求默认是低电平有效

3、部分对SPI的封装延时时间过长，例如 xWriteCH376Cmd(u8 mCmd)的delay_us(1700)，demo中延迟时间是大于1.5us即可。

4、SPI驱动不是很好用。建议每一次读写时前后set/reset CS，而非set CS直到批量操作结束后再reset。

硬件：

将PA8用跳线帽与TF IN/USB INT短接，方可使用CH376 Interrupt

## 程序缺陷

1、文件名必须为全大写，否则读写可能失败，暂时没有去排查原因。

2、IAP模块对filesys模块提供的接口封装的不好。

3、从U盘读出的数据未进行校验。
