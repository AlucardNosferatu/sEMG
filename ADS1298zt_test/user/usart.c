#include "usart.h"
#include "ads1298.h"
#include "schedule.h"

u8 Rx1_enable = 0;
 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART1_RX_STA=0; 

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 						//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						 //ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//--- �����ж� ---
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ�ܴ��ڽ����ж�
	
	USART_ClearFlag(USART1,USART_FLAG_TC);	//��շ�����ɱ�־λ
	USART_ClearFlag(USART1,USART_FLAG_RXNE);	//��շ�����ɱ�λ
	USART_Cmd(USART1, ENABLE); //����ʹ��
	
	TIM4_Init(999,7199);		//10ms�ж�
	TIM4_Set(0);			//�رն�ʱ��4
}

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Res=USART_ReceiveData(USART1);
		if(Rx1_enable) // ͸��
		{
				parse(Res);
		}
		else //Wifi
		{
				if(USART1_RX_STA<USART1_MAX_RECV_LEN)		//�����Խ�������
				{
					TIM_SetCounter(TIM4,0);//���������        				 
					if(USART1_RX_STA==0) TIM4_Set(1);	 	//ʹ�ܶ�ʱ��4���ж� 
					USART1_RX_BUF[USART1_RX_STA++]=Res;		//��¼���յ���ֵ	 
				}else 
				{
					USART1_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
				} 
		}
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


// ���������ַ�
void parse(u8 cmd)
{  
	int retval2, retval1;
	switch(cmd)
	{
		case ADS_reset:
			// restart
			GPIO_SetBits(CLKSEL2_Port,CLKSEL2_Pin); 
			delayMs(500);
			retval1 = resetADS1298(0,CS1_Port,CS1_Pin);
			retval2 = resetADS1298(1,CS2_Port,CS2_Pin);
			GPIO_ResetBits(CLKSEL2_Port,CLKSEL2_Pin); 
			delayMs(500);
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
			if((retval1==0)&&(retval2==0))
			{
				usart1_sendByte(0xff);
				usart1_sendByte(0xff);
			}
			else 
			{
				usart1_sendByte(0xee);
				usart1_sendByte(0xee);
			}
			break;
			
		case ADS_gotoTestMode:
			retval1 = configForSquarewaveTest(CS1_Port,CS1_Pin);
			retval2 = configForSquarewaveTest(CS2_Port,CS2_Pin);
			
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
		
			if((retval1==0)&&(retval2==0))
			{
					usart1_sendByte(0xff);
					usart1_sendByte(0xff);
			}else
			{
					usart1_sendByte(0xee);
					usart1_sendByte(0xee);
			}
			break;
			
		case ADS_gotoNoiseMode:
			retval1 = configForNoiseTest(CS1_Port,CS1_Pin);
			retval2 = configForNoiseTest(CS2_Port,CS2_Pin);
		
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
			if ((retval1==0)&&(retval2==0))
			{
					usart1_sendByte(0xff);
					usart1_sendByte(0xff);
			}else
			{
					usart1_sendByte(0xee);
					usart1_sendByte(0xee);
			}
			break;
			
		case ADS_gotoMeasurementMode:
			retval1 = configForNormalMeasurement(CS1_Port,CS1_Pin);
			retval2 = configForNormalMeasurement(CS2_Port,CS2_Pin);
			
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
		
			if ((retval1==0)&&(retval2==0))
			{
					usart1_sendByte(0xff);
					usart1_sendByte(0xff);
			}else
			{
					usart1_sendByte(0xee);
					usart1_sendByte(0xee);
			}
			break;
			
		case ADS_beginRead:
			beginReadDataC();   //begin continuously conversion
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);	
			break;
		
		case ADS_stopRead:
			stopReadDataC();    //stop ..
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			usart1_sendByte(0x03);
			usart1_sendByte(0xff);
			usart1_sendByte(0xff);
			break;
		
			
	}
}

//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART1_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־    
		TIM4_Set(0);			//�ر�TIM4  
	}	    
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM4,0);//���������
		TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
	}else TIM_Cmd(TIM4, DISABLE);//�رն�ʱ��4	   
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
