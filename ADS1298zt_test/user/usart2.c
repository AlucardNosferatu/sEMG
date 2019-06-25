#include "usart.h"
#include "ads1298.h"
#include "schedule.h"

u8 parseEnable;

void usart1_Init(int baudRate)	//USART1 ��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //�򿪸���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //�򿪴���ʱ��

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//���ô��� 
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	
	//--- ����NVIC���� ---
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   				//��USART1��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 						//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						 //ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//--- �����ж� ---
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ�ܴ��ڽ����ж�
	
	USART_ClearFlag(USART1,USART_FLAG_TC);	//��շ�����ɱ�־λ
	USART_ClearFlag(USART1,USART_FLAG_RXNE);	//��շ�����ɱ�λ
	USART_Cmd(USART1, ENABLE); //����ʹ��
	
	parseEnable = 0;
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint16_t Res;
  if( (parseEnable!=0) && (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) )
	{
		Res=USART_ReceiveData(USART1);
    parse(Res);
  } 
}

void usart1_send(char* dat, u8 len)
{
	u8 i;
	for(i=0; i<len; i++)
	{
		USART_SendData(USART1, dat[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); // �ȴ��������
	}
	
}

void usart1_sendString(char* str)
{
	u16 i=0;
	while(str[i])
	{
		usart1_sendByte(str[i++]);
	}
}

void usart1_sendByte(u8 dat)
{
	USART_SendData(USART1, dat);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); // �ȴ��������
}

