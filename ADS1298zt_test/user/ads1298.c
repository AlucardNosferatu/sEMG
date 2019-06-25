#include "ads1298.h"
#include "schedule.h"
#include "stm32f10x_exti.h"

void ads1298_init(void)      //ads1298��ʼ��//IO�ڳ�ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );//PORTA��PORTBʱ��ʹ�� 
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //ʹ��PB3 PB4 ���ֹJTAG
    
		//EMG_Ready   DRDY2  ��ʼ��   �˿�PB.2   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	  //EMG_Ready   DRDY1  ��ʼ��   �˿�PA.12   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    //EMG_SPI2 SCK  MISO  MOSI  ��ʼ��  PB.13/14/15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB.13/14/15����
		
		//����SPI2�����
    SPI_I2S_DeInit(SPI2); //��SPI2�Ĵ�������Ϊȱʡֵ
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    
    SPI_NSSInternalSoftwareConfig(SPI2,SPI_NSSInternalSoft_Set);//Ϊѡ����SPI��������ڲ�NSS�ܽ�
    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
    
    //EMG_RESET1/CS1/START/RESET2 PC.6/7/8/9 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�˿�ģʽ���������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		//EMG_CLKSEL2 PB.12 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�˿�ģʽ���������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		//EMG_CS2/CLKSEL1 PA.8/11 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�˿�ģʽ���������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
		
    GPIO_SetBits(RESET1_Port,RESET1_Pin); //Reset1�øߵ�ƽ
		GPIO_SetBits(RESET2_Port,RESET2_Pin); //Reset2�øߵ�ƽ
    GPIO_ResetBits(START_Port,START_Pin); //Start�õ͵�ƽ
		GPIO_SetBits(CLKSEL1_Port,CLKSEL1_Pin);//CLK_SEL1�øߵ�ƽ
		GPIO_SetBits(CLKSEL2_Port,CLKSEL2_Pin);//CLK_SEL2�øߵ�ƽ
		disableADS1298(CS1_Port,CS1_Pin);     //CSʧ��
		disableADS1298(CS2_Port,CS2_Pin);
}

// flag = 0 Master
// flag = 1 Slave
int resetADS1298(u8 flag, GPIO_TypeDef* port, u16 pin)                  //��ads1298��������
{
    int trytime ,r;
		u8 addr; 
	
    for (trytime=0;trytime<5;trytime++)                     
    {      
				writeCommand(SDATAC,port,pin);
				delayMs(10);
				writeCommand(ADS_RESET,port,pin);
				delayMs(500);
			
        writeCommand(SDATAC,port,pin);
        if (shakeHands(port,pin)==ADS1298_DEVICE_ID)
            break;
        delayMs(10);
    }
    if (trytime==5)
        return -1;
    
		r = tryWriteRegister(CONFIG3,0xc0,5,port,pin);   //  0xdc enable REFBUF, RLD_BUF
    if (r!=0)
        return -1;
	
		delayMs(100);
		
		if(flag==0)
			r = tryWriteRegister(CONFIG1,0x26,5,port,pin);   // LR Mode: 250 SPS 
		else
			r = tryWriteRegister(CONFIG1,0x06,5,port,pin);
		if (r!=0)
        return -1;
		
		r = tryWriteRegister(CONFIG2,0x00,5,port,pin);   // input short 
    if (r!=0)
        return -1;
		
		for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
		{
			r = tryWriteRegister(addr,0x01,5,port,pin);   // Ch to Test signal
			if(r)
				return -1;
		}    
		
		r = tryWriteRegister(RLD_SENSP,0xff,5,port,pin);   // RLD Positive 
    if (r!=0)
        return -1;
		
		r = tryWriteRegister(RLD_SENSN,0xff,5,port,pin);   // RLD Negative
    if (r!=0)
        return -1;

		return 0;
}

int configForSquarewaveTest(GPIO_TypeDef* port,u16 pin)       //���óɷ�������ģʽ
{ 
	int r=-1,n=1;
	u8 addr;
	
	r = tryWriteRegister(CONFIG2,0x10,5,port,pin);   // signal internal
	if(r)
		return -1;
  
	for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		r = tryWriteRegister(addr,0x05,5,port,pin);   // Ch to Test signal
		if(r)
			return -1;
	}    
	
	return 0;
}

int configForNoiseTest(GPIO_TypeDef* port, u16 pin)            //���ó���������ģʽ
{
	int r;
	u8 addr;
	r = tryWriteRegister(CONFIG2,0x00,5,port,pin);   // signal external
  if (r!=0) 
      return -1;
	
	for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
			r = tryWriteRegister(addr,0x01,5,port,pin);   // Ch to shorted
			if (r!=0)
				return -1;
  }    
	return 0;
}


int configForNormalMeasurement(GPIO_TypeDef* port, u16 pin)    //���ó���������ģʽ
{
	int r;
	u8 addr;
	r = tryWriteRegister(CONFIG2,0x00,5,port,pin);   // signal external
  if (r!=0)
      return -1;
	
  for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
			r = tryWriteRegister(addr,0x00,5,port,pin);   // 
			if (r!=0)
				return -1;
  }    
	return 0; 
}

void beginReadDataC()               //�����ж�              
	{
	  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
		
		/* Enable SYSCFG clock */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //����AFIOʱ��
 
    //EXTI4 NVIC ����
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;       		//ʹ���ⲿ�ж�ͨ��
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//    
//    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);   //��ʼ���ж��ߺ���
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;       		//ʹ���ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   //��ʼ���ж��ߺ���
	
	
		writeCommand(RDATAC,CS2_Port,CS2_Pin);
		writeCommand(RDATAC,CS1_Port,CS1_Pin);
		
		delayMs(5);
    GPIO_SetBits(START_Port,START_Pin); // START HIGH
    delayMs(1);
		
		enableADS1298(CS1_Port,CS1_Pin);
} 


void EXTI15_10_IRQHandler(void)             //�жϷ�����
{       
    int i;
    u8 tmp[54],sum;
	
		if(EXTI_GetITStatus(EXTI_Line12)==SET)
		{
				for (i=0;i<27;i++)
				{
					tmp[i] = EMG_SendByte(0xff);
				}
				
				disableADS1298(CS1_Port,CS1_Pin);
				enableADS1298(CS2_Port,CS2_Pin);
				
				for (i=0;i<27;i++)
				{
					tmp[i+27] = EMG_SendByte(0xff);
				}
				
				disableADS1298(CS2_Port,CS2_Pin);
				enableADS1298(CS1_Port,CS1_Pin);
				
				sum=0;
				usart1_sendByte(0xff);   		 	//��ͷ����0xff 
				usart1_sendByte(0xff);
				usart1_sendByte(0x01);    		//EMG��������0x01
				for (i=0;i<54;i++)            //����EMG����
				{
						usart1_sendByte(tmp[i]);
						sum += tmp[i];
				}
				usart1_sendByte(sum);//send checksum
		}				
		EXTI_ClearITPendingBit(EXTI_Line12);
}

u8 shakeHands(GPIO_TypeDef* port, u16 pin)                     //����
{
    u8 deviceID;
    deviceID = readRegister(0x00,port,pin);
    return deviceID;
}


int stopReadDataC()                
{
	GPIO_ResetBits(START_Port,START_Pin);    //START = 0
	disableADS1298(CS1_Port,CS1_Pin);
	
	writeCommand(SDATAC,CS2_Port,CS2_Pin);  
	writeCommand(SDATAC,CS1_Port,CS1_Pin); 
		
	return 0;
}


int configRLD(u8 rldp, u8 rldn, GPIO_TypeDef* port, u16 pin)         //����RLD�Ĵ���������configRLD����������ֵΪ���ͣ���ʽ����rldp��rldnΪ�޷����ַ���
{
		int r;
		r = tryWriteRegister(RLD_SENSP,rldp,5,port,pin);   // RLD Positive 
    if (r!=0)
        return -1;
		
		r = tryWriteRegister(RLD_SENSN,rldn,5,port,pin);   // RLD Negative
    if (r!=0)
        return -1;
    return 0;
}

//-------------------------------- �ײ㺯��----------------------------------------


u8 EMG_SendByte(u8 TxData)    //����\��������
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  //��鷢���Ƿ���ɣ�����Ժ��ٷ�������
    SPI_I2S_SendData(SPI2,TxData); //ͨ������SPIx����һ������
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  //�������ܱ�־λ���Ƿ���Խ��� 
	  return SPI_I2S_ReceiveData(SPI2);   //����ͨ��SPIx������յ�����	
}

//���Ĵ���
u8 readRegister(u8 addr, GPIO_TypeDef* port, u16 pin)
{
    u8 r;
    enableADS1298(port,pin);
    EMG_SendByte(RREG|addr);
    EMG_SendByte(0x00); //��ȡ1��
    r=EMG_SendByte(0xff); 
    disableADS1298(port,pin);
    return r;
}

//д�Ĵ���
int writeRegister(u8 addr,u8 value, GPIO_TypeDef* port, u16 pin)         
{
    enableADS1298(port,pin);
    EMG_SendByte(WREG|addr);
    EMG_SendByte(0x00); //д1��
    EMG_SendByte(value);
    disableADS1298(port,pin);
    delayMs(1);
    if (readRegister(addr,port,pin)==value)
        return 0;
    else
        return -1;
}

//������Ĵ������������ regs ��
void readNRegister(u8 start,u8 count, u8* regs, GPIO_TypeDef* port, u16 pin)
{
	u8 i;
	enableADS1298(port,pin);
	EMG_SendByte(RREG|start);
	EMG_SendByte(count-1);
	for(i=0;i<count;i++)
		regs[i] = EMG_SendByte(0x00);
	disableADS1298(port,pin);
}

//д����Ĵ���
int writeNRegister(u8 start, u8 count, u8* regs, GPIO_TypeDef* port, u16 pin)
{
	u8 i;
	u8 val;
	enableADS1298(port,pin);
	EMG_SendByte(WREG|start);
	EMG_SendByte(count-1);
	for(i=0;i<count;i++)
		EMG_SendByte(regs[i]);
	
	//check
	EMG_SendByte(RREG|start);
	EMG_SendByte(count-1);
	for(i=0;i<count;i++)
	{
		val = EMG_SendByte(0x00);
		if(val!=regs[i])
			break;
	}
	disableADS1298(port,pin);
	
	if(i<count)
		return -1;
	else
		return 0;
}



void writeCommand(u8 cmd, GPIO_TypeDef* port, u16 pin)
{
		enableADS1298(port,pin);
    EMG_SendByte(cmd);
    EMG_SendByte(0x00); //�ȴ�����4������
    disableADS1298(port,pin);
}


//----------------------------------------------
// ����: д�Ĵ�������ೢ�� tryTimes ��
// ���أ�0  - �ɹ�
//       -1 - ʧ��
int tryWriteRegister(u8 addr, u8 value, int tryTimes, GPIO_TypeDef* port, u16 pin)
{
	int i,r;
	for(i=0;i<tryTimes;i++)
	{
		r = writeRegister(addr,value,port,pin);
		if(r==0)
			break;
		delayMs(10);
	}
	if(i==tryTimes)
		return -1;
	else
		return 0;
}
