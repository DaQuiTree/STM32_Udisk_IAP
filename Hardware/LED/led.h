#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LEDn1 PBout(0)// PB0
#define LEDn2 PBout(1)// PB1	
#define LEDPORT	GPIOB	//����IO�ӿ�
#define LED1	GPIO_Pin_0	//����IO�ӿ�
#define LED2	GPIO_Pin_1	//����IO�ӿ�
#define LED3	GPIO_Pin_2	//����IO�ӿ�
#define LED4	GPIO_Pin_3	//����IO�ӿ�
#define LED5	GPIO_Pin_4	//����IO�ӿ�

void LED_Init(void);//��ʼ��
		 				    
#endif
