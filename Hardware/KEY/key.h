#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"

#define KEYPORT	GPIOA	//����IO�ӿ���
#define KEY1	GPIO_Pin_0	//����IO�ӿ�

void KEY_Init(void);//��ʼ��
uint8_t KEY_Read(void);
		 				    
#endif
