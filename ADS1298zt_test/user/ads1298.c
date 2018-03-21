#include "ads1298.h"
#include "schedule.h"
#include "stm32f10x_exti.h"

void ads1298_init(void)      //ads1298��ʼ��//IO�ڳ�ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE );//PORTA��PORTBʱ��ʹ�� 
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );//SPI1ʱ��ʹ�� 
    
    //EMG_Ready   DRDY  ��ʼ��   �˿�PA.4   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    //EMG_SPI_CLK  MISO  MOSI  ��ʼ��  PA5/6/7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA,SCK
    
		GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PA5/6/7����
		
		//����SPI1�����
    SPI_I2S_DeInit(SPI1); //��SPI1�Ĵ�������Ϊȱʡֵ
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    
    SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);//Ϊѡ����SPI��������ڲ�NSS�ܽ�
    SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
    
    //EMG_Reset/CS/START 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//�˿�ģʽ���������
    GPIO_InitStructure.GPIO_Pin = RESET_Pin | CS_Pin | START_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    
		
		
   //GPIO_SetBits(GPIOB,RESET_Pin); //Reset�øߵ�ƽ
   GPIO_ResetBits(GPIOB,START_Pin);//Start�õ͵�ƽ
	
		delayMs(500);
}


int resetADS1298(void)                  //��ads1298��������
{
    int trytime ,r;
		u8 addr; 
    GPIO_SetBits(GPIOB,RESET_Pin);             //Reset = 1
    GPIO_ResetBits(GPIOB,CS_Pin|START_Pin);    //CS, START = 0
    delayMs(10);
	
    for (trytime=0;trytime<5;trytime++)                     
    {      
        GPIO_ResetBits(GPIOB,RESET_Pin);                   //Reset = 0
        delayMs(500);
        GPIO_SetBits(GPIOB,RESET_Pin);                     //Reset = 1
        delayMs(10);
        
        writeCommand(SDATAC);
        if (shakeHands()==ADS1298_DEVICE_ID)
            break;
        delayMs(10);
    }
    if (trytime==5)
        return -1;
    
		r = tryWriteRegister(CONFIG3,0xc0,5);   //  0xdc enable REFBUF, RLD_BUF
    if (r!=0)
        return -1;
	
		delayMs(100);
		
		r = tryWriteRegister(CONFIG1,0x86,5);   // HR Mode: 500 SPS 
    if (r!=0)
        return -1;
		
		r = tryWriteRegister(CONFIG2,0x00,5);   // HR Mode: 500 SPS 
    if (r!=0)
        return -1;
		
		for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		while(r!=0)
		{
		r = tryWriteRegister(addr,0x01,5);   // Ch to Test signal
		}
		r=-1;
	}    
		/*
		r = tryWriteRegister(RLD_SENSP,0xff,5);   // RLD Positive 
    if (r!=0)
        return -1;
		
		r = tryWriteRegister(RLD_SENSN,0xff,5);   // RLD Negative
    if (r!=0)
        return -1;
    */
		return 0;
}

/*
int configForSquarewaveTest()       //���óɷ�������ģʽ
{ 
	int r;
	u8 addr;
	r = tryWriteRegister(CONFIG2,0x10,5);   // signal internal
  if (r!=0)
      return -1;
	
  for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		r = tryWriteRegister(addr,0x05,5);   // Ch to Test signal
		if (r!=0)
      return -1;
  }    
	return 0;
}
*/

int configForSquarewaveTest()       //���óɷ�������ģʽ
{ 
	int r=-1,n=1;
	u8 addr;
	
	while(r!=0)
	r = tryWriteRegister(CONFIG2,0x10,5);   // signal internal
	r=-1;
  
	for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		while(r!=0)
		{
		r = tryWriteRegister(addr,0x05,5);   // Ch to Test signal
		}
		r=-1;
	}    
	return 0;
}

void Read_CH()
{	
	u8 addr;
	int i,j;
for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		usart1_sendByte(addr);
		j=readRegister(addr);	
		usart1_sendByte(j);
  } 

}	

int configForNoiseTest(void)            //���ó���������ģʽ
{
	int r;
	u8 addr;
	r = tryWriteRegister(CONFIG2,0x00,5);   // signal external
  if (r!=0)
      return -1;
	
	for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		r = tryWriteRegister(addr,0x01,5);   // Ch to shorted
		if (r!=0)
      return -1;
  }    
	return 0;
}

int configForNormalMeasurement(void)    //���ó���������ģʽ
{
	int r;
	u8 addr;
	r = tryWriteRegister(CONFIG2,0x00,5);   // signal external
  if (r!=0)
      return -1;
	
  for(addr=CH1SET;addr<=CH8SET;addr++) // CH 1~8
  {
		r = tryWriteRegister(addr,0x00,5);   // 
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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //����AFIOʱ��
 
    //EXTI4 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;       		//ʹ���ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;	//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    /* Connect EXTI4 Line to PA4 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   //��ʼ���ж��ߺ���
	
	
		writeCommand(RDATAC);
		writeCommand(START);
		delayMs(5);
		// START HIGH
    GPIO_SetBits(GPIOB,GPIO_Pin_2);
    delayMs(10);
    enableADS1298();
} 
	

void EXTI4_IRQHandler(void)             //�жϷ�����
{       
    int i;
    uint8_t tmp,sum;
   
    sum=0;
    usart1_sendByte(0xff);   		 	//��ͷ����0xff 
    usart1_sendByte(0xff);
    usart1_sendByte(0x01);    		//EMG��������0x01
		
    for (i=0;i<27;i++)            //����EMG���ݣ���27���ֽ�
    {
        tmp = EMG_SendByte(0xff);
        usart1_sendByte(tmp);
        sum += tmp;
    }
    usart1_sendByte(sum);//send checksum

		EXTI_ClearITPendingBit(EXTI_Line4);
}

u8 shakeHands(void)                     //����
{
    u8 deviceID;
    deviceID=readRegister(0x00);
    return deviceID;
}

void stopReadDataC(void)                
{
	GPIO_ResetBits(GPIOB,CS_Pin|START_Pin);    //CS, START = 0
	
	writeCommand(STOP);
  delayMs(5);
	
	writeCommand(SDATAC);  
	delayMs(5);
}


int configRLD(u8 rldp, u8 rldn)         //����RLD�Ĵ���������configRLD����������ֵΪ���ͣ���ʽ����rldp��rldnΪ�޷����ַ���
{
		int r;
		r = tryWriteRegister(RLD_SENSP,rldp,5);   // RLD Positive 
    if (r!=0)
        return -1;
		
		r = tryWriteRegister(RLD_SENSN,rldn,5);   // RLD Negative
    if (r!=0)
        return -1;
    return 0;
}

//-------------------------------- �ײ㺯��----------------------------------------


uint8_t EMG_SendByte(uint8_t TxData)    //����\��������
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  //��鷢���Ƿ���ɣ�����Ժ��ٷ�������
    SPI_I2S_SendData(SPI1,TxData); //ͨ������SPIx����һ������
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  //�������ܱ�־λ���Ƿ���Խ��� 
	  return SPI_I2S_ReceiveData(SPI1);   //����ͨ��SPIx������յ�����	
}

u8 readRegister(u8 addr)    //���Ĵ���������ֵΪ�޷����ַ��ͣ���ʽ����startΪ�ַ��ͣ�countΪ�޷����ַ���
{
    char r;
    enableADS1298();
    EMG_SendByte(RREG|addr);
    EMG_SendByte(0x00); //��ȡ1��
    r=EMG_SendByte(0xff); 
    disableADS1298();
    return r;
}

int writeRegister(u8 addr,u8 value)     //д�Ĵ���         
{
    enableADS1298();
    EMG_SendByte(WREG|addr);
    EMG_SendByte(0x00); //д1��
    EMG_SendByte(value);
    disableADS1298();
    delayMs(1);
    if (readRegister(addr)==value)
        return 0;
    else
        return -1;
}

void readNRegister(u8 start,u8 count, u8* regs)
{
	u8 i;
	enableADS1298();
	EMG_SendByte(RREG|start);
	EMG_SendByte(count-1);
	for(i=0;i<count;i++)
		regs[i] = EMG_SendByte(0x00);
	disableADS1298();
}

int writeNRegister(u8 start, u8 count, u8* regs)
{
	u8 i;
	u8 val;
	enableADS1298();
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
	disableADS1298();
	
	if(i<count)
		return -1;
	else
		return 0;
}

void writeCommand(u8 cmd)
{
		enableADS1298();
    EMG_SendByte(cmd);
    EMG_SendByte(0x00); //�ȴ�����4������
    disableADS1298();
}

//----------------------------------------------
// ����: д�Ĵ�������ೢ�� tryTimes ��
// ���أ�0  - �ɹ�
//       -1 - ʧ��
int tryWriteRegister(u8 addr, u8 value, int tryTimes)
{
	int i,r;
	for(i=0;i<tryTimes;i++)
	{
		r = writeRegister(addr,value);
		if(r==0)
			break;
		delayMs(10);
	}
	if(i==tryTimes)
		return -1;
	else
		return 0;
}

