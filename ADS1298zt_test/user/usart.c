#include "usart.h"
#include "ads1298.h"
#include "schedule.h"

u16 state=STATE_HEAD;

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
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   //��USART1��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 			//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //ʹ��
	NVIC_Init(&NVIC_InitStructure);

	//--- �����ж� ---
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ�ܴ��ڽ����ж�
	
	USART_ClearFlag(USART1,USART_FLAG_TC);	//��շ�����ɱ�־λ
	USART_ClearFlag(USART1,USART_FLAG_RXNE);	//��շ�����ɱ�λ
	USART_Cmd(USART1, ENABLE); //����ʹ��
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint16_t Res;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Res=USART_ReceiveData(USART1);
    //parse(Res);
		if(Res=='a')
			stopReadDataC();
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

void usart1_sendByte(u8 dat)
{
	USART_SendData(USART1, dat);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); // �ȴ��������
}


// ���������ַ�
void parse(u8 c)
{
    static u8 index=0,length=0,command,addr,value,sum;
    u8 deviceID;
    int retval;
    if (state == STATE_HEAD && c==0xff)
    {
        state=STATE_LENGTH;
        return ;
    }
	if (state == STATE_LENGTH)
	{
        state=STATE_COMMAND;
        length = c;
        index=0;
        sum=0;
        return;
    }
    if (state == STATE_COMMAND)
    {
        command=c;
        sum+=c;
        index++;
        if (index<length)
            state=STATE_ADDR;
        else
            state=STATE_CHECK;
        return;
    }
    if (state==STATE_ADDR)
    {
        addr=c;
        sum+=c;
        index++;
        if (index<length)
            state=STATE_VALUE;
        else
            state=STATE_CHECK;
        return ;
    }
    if (state==STATE_VALUE)
    {
        value=c;
        sum+=c;
        index++;
        state=STATE_CHECK;
        return;
    }
    if (state==STATE_CHECK)
    {
        if (sum==c)
        {  
            if (command==0x00)
            {
                addr=shakeHands();
            }
            if (command==0x01)
            {
                GPIO_SetBits(GPIOA,GPIO_Pin_15);
            }
            if (command == 0x02)
            {
                //motorshake(addr,80,20,value);
            }
            if (command ==0x03)
            {
                retval = configRLD(addr, value);
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                if (retval==0)
                {
                    usart1_sendByte(0xff);
                    usart1_sendByte(0xff);
                }else
                {
                    usart1_sendByte(0xee);
                    usart1_sendByte(0xee);
                }                
            }
            if (command ==0x04)
            {
                retval=configForNoiseTest();
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                if (retval==0)
                {
                    usart1_sendByte(0xff);
                    usart1_sendByte(0xff);
                }else
                {
                    usart1_sendByte(0xee);
                    usart1_sendByte(0xee);
                }
            }
            if (command ==0x05)
            {
                retval=configForSquarewaveTest();
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                if (retval==0)
                {
                    usart1_sendByte(0xff);
                    usart1_sendByte(0xff);
                }else
                {
                    usart1_sendByte(0xee);
                    usart1_sendByte(0xee);
                }
            }
            if ( command == 0x06)
            {
                retval=configForNormalMeasurement();
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                if (retval==0)
                {
                    usart1_sendByte(0xff);
                    usart1_sendByte(0xff);
                }else
                {
                    usart1_sendByte(0xee);
                    usart1_sendByte(0xee);
                }
            }
            if (command==0x10)
                beginReadDataC();   //begin continuously conversion
            if (command==0x11)
                stopReadDataC();    //stop ..
            if (command==0x20)
            {
                // read register and return
                deviceID=readRegister(addr);
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                usart1_sendByte(deviceID);
                usart1_sendByte(deviceID);
            }
                
            if (command==0x40)
            {
                // write register and return the new value
                writeRegister(addr,value);
                delayMs(10);
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                deviceID=readRegister(addr);
                usart1_sendByte(deviceID);
                usart1_sendByte(deviceID);
            }
            if (command==0x80)
            {
                // restart
                resetADS1298();
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);
                usart1_sendByte(0x03);
                usart1_sendByte(0xff);
                usart1_sendByte(0xff);                
            }
        }
        state=STATE_HEAD;
        return;
    }
}
