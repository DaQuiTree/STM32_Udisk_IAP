
/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708271933 ����������ʱ������


*/
#define _USART_C_

#include "sys.h"
#include "usart.h"

//global
u32 Uart1RxLen = 0;
vu8 Uart1RxBuf[MAX_RXBUF_SIZ] __attribute__((at(0x20001000))) = {0};
	  	 
//ʹUASRT���ڿ���printf��������
//��usart.h�ļ���ɸ���ʹ��printf�����Ĵ��ں�	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x){ 
	x = x; 
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f){      
	while((USART_n->SR&0X40)==0);//ѭ������,ֱ���������   
    USART_n->DR = (u8) ch;      
	return ch;
}
#endif 



/*
USART1������س���
*/
 
#if EN_USART1   //USART1ʹ��������ѡ��
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	  

/*
USART1ר�õ�printf����
��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
���÷�����USART1_printf("123"); //��USART2�����ַ�123
*/
void USART1_printf (char *fmt, ...){ 
	char buffer[USART1_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART1_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART1_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART1, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}

void USART1_Init(u32 bound){ //����1��ʼ��������
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
    USART_ClearFlag(USART1,USART_FLAG_TC);
}


void USART1_IRQHandler(void){
    u8 clear = clear;

    if(!IS_USART1_IDLE){
       clear = USART1->SR;  //���������ɱ�־λ
       clear = USART1->DR;
       return;//���ձ�־λδ��λ����ʾ����δ������ϣ�������������
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET){//��������
       if(Uart1RxLen < MAX_RXBUF_SIZ)
           Uart1RxBuf[Uart1RxLen++] = USART_ReceiveData(USART1);     
    }else if(USART_GetITStatus(USART1, USART_IT_IDLE)!= RESET){//�������
       clear = USART1->SR;  //���������ɱ�־λ
       clear = USART1->DR;
       
       USART1_RX_END;   //������ɱ�־λ��1
    }
}
#endif	

/*
USART2������س���
*/
#if EN_USART2   //USART2ʹ��������ѡ��
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	  

/*
USART2ר�õ�printf����
��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
���÷�����USART2_printf("123"); //��USART2�����ַ�123
*/
void USART2_printf (char *fmt, ...){ 
	char buffer[USART2_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART2_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART2_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


void USART2_Init(u32 bound){ //����1��ʼ��������
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //ʹ��UART2����GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ�ܴ��ڵ�RCCʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //����USART2��RX�ӿ���PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //����USART2��TX�ӿ���PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//����ENABLE/�ر�DISABLE�ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
   //Usart2 NVIC ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 
}

void USART2_IRQHandler(void){ //����2�жϷ�����򣨹̶��ĺ����������޸ģ�	

} 
#endif	


#if EN_USART3   //���ʹ���˽���
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  

/*
USART3ר�õ�printf����
��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
���÷�����USART3_printf("123"); //��USART3�����ַ�123
*/
void USART3_printf (char *fmt, ...){ 
	char buffer[USART3_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART3_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART3, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}

void USART3_Init(u32 BaudRate){ //USART3��ʼ��������
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure; 

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��UART3����GPIOB��ʱ��
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ�ܴ��ڵ�RCCʱ��

   //����ʹ�õ�GPIO������
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//����USART3��RX�ӿ���PB11
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�ӿ�ģʽ ��������
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//����USART3��TX�ӿ���PB10
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�50MHz
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�ӿ�ģʽ �����������
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   //���ô���
   USART_InitStructure.USART_BaudRate = BaudRate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
   USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
   USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

   USART_Init(USART3, &USART_InitStructure);//���ô���3
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܴ��ڽ����ж�  
   //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//���ڷ����ж��ڷ�������ʱ����
   USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3

   //�����ж�����
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����USART3�ж�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�жϵȼ�
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

//����3�жϷ�����򣨹̶��ĺ����������޸ģ�
void USART3_IRQHandler(void){ 	

} 
#endif	




/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/

/*
a���ŵ����ã�

%d ʮ�����з�������
%u ʮ�����޷�������
%f ������
%s �ַ���
%c �����ַ�
%p ָ���ֵ
%e ָ����ʽ�ĸ�����
%x, %X �޷�����ʮ�����Ʊ�ʾ������
%o �޷����԰˽��Ʊ�ʾ������
%g �Զ�ѡ����ʵı�ʾ��
%p �����ַ��

*/





