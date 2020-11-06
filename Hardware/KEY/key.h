#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"

#define KEYPORT	GPIOA	//定义IO接口组
#define KEY1	GPIO_Pin_0	//定义IO接口

void KEY_Init(void);//初始化
uint8_t KEY_Read(void);
		 				    
#endif
