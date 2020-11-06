#ifndef __USART_H
#define __USART_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 

#define MAX_RXBUF_SIZ 1024*10

#ifndef _USRAT_C_
extern u32 Uart1RxLen;
extern vu8 Uart1RxBuf[MAX_RXBUF_SIZ];
#endif

#define USART1_RX_END        (Uart1RxLen |= (1 << 15))
#define IS_USART1_RX_END     (Uart1RxLen & (1 << 15) ? 1 : 0)
#define USART1_RX_IDLE       (Uart1RxLen = 0)   
#define IS_USART1_IDLE       (!IS_USART1_RX_END)
#define USART1_RX_LEN     (Uart1RxLen & ~(1 << 15))

#define USART_n		USART1  //����ʹ��printf�����Ĵ��ڣ���������Ҫʹ��USART_printfר�ú�������

#define USART1_REC_LEN  			200  	//����USART1�������ֽ���
#define USART2_REC_LEN  			200  	//����USART2�������ֽ���
#define USART3_REC_LEN  			200  	//����USART3�������ֽ���

//��ʹ��ĳ������ʱҪ��ֹ�˴��ڣ��Լ��ٱ�����
#define EN_USART1 			1		//ʹ�ܣ�1��/��ֹ��0������1
#define EN_USART2 			0		//ʹ�ܣ�1��/��ֹ��0������2
#define EN_USART3 			0		//ʹ�ܣ�1��/��ֹ��0������3
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
 
extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         		//����״̬���	
extern u16 USART3_RX_STA;         		//����״̬���	

//��������
void USART1_Init(u32 bound);//����1��ʼ��������
void USART2_Init(u32 bound);//����2��ʼ��������
void USART3_Init(u32 bound);//����3��ʼ��������
void USART1_printf(char* fmt,...); //����1��ר��printf����
void USART2_printf(char* fmt,...); //����2��ר��printf����
void USART3_printf(char* fmt,...); //����3��ר��printf����

#endif


