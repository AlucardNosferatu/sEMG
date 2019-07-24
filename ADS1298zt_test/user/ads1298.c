#include "ads1298.h"
#include "schedule.h"
#include "stm32f10x_exti.h"

void ads1298_init(void)      //ads1298初始化//IO口初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );//PORTA、PORTB时钟使能 
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //使用PB3 PB4 需禁止JTAG
    
		//EMG_Ready   DRDY2  初始化   端口PB.2   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	  //EMG_Ready   DRDY1  初始化   端口PA.12   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    //EMG_SPI2 SCK  MISO  MOSI  初始化  PB.13/14/15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB.13/14/15上拉
		
		//配置SPI2的输出
    SPI_I2S_DeInit(SPI2); //将SPI2寄存器重设为缺省值
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    
    SPI_NSSInternalSoftwareConfig(SPI2,SPI_NSSInternalSoft_Set);//为选定的SPI软件配置内部NSS管脚
    SPI_Cmd(SPI2, ENABLE); //使能SPI外设
    
    //EMG_RESET1/CS1/START/RESET2 PC.6/7/8/9 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//端口模式：推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		//EMG_CLKSEL2 PB.12 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//端口模式：推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		//EMG_CS2/CLKSEL1 PA.8/11 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//端口模式：推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
		
    GPIO_SetBits(RESET1_Port,RESET1_Pin); //Reset1置高点平
		GPIO_SetBits(RESET2_Port,RESET2_Pin); //Reset2置高点平
    GPIO_ResetBits(START_Port,START_Pin); //Start置低电平
		GPIO_SetBits(CLKSEL1_Port,CLKSEL1_Pin);//CLK_SEL1置高电平
		GPIO_SetBits(CLKSEL2_Port,CLKSEL2_Pin);//CLK_SEL2置高电平
		disableADS1298(CS1_Port,CS1_Pin);     //CS失能
		disableADS1298(CS2_Port,CS2_Pin);
}

// flag = 0 Master
// flag = 1 Slave
int resetADS1298(u8 flag, GPIO_TypeDef* port, u16 pin)                  //对ads1298进行配置
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
			//r = tryWriteRegister(CONFIG1,0x26,5,port,pin);   // LR Mode: 250 SPS 
			r = tryWriteRegister(CONFIG1,0xA5,5,port,pin);   // HS Mode: 1k SPS 
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

int configForSquarewaveTest(GPIO_TypeDef* port,u16 pin)       //配置成方波测量模式
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

int configForNoiseTest(GPIO_TypeDef* port, u16 pin)            //配置成噪声测量模式
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


int configForNormalMeasurement(GPIO_TypeDef* port, u16 pin)    //配置成正常测量模式
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

void beginReadDataC()               //设置中断              
	{
	  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
		
		/* Enable SYSCFG clock */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //开启AFIO时钟
 
    //EXTI4 NVIC 配置
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;       		//使能外部中断通道
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
//    NVIC_Init(&NVIC_InitStructure);														//根据指定的参数初始化VIC寄存器
//    
//    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);   //初始化中断线函数
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;       		//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);														//根据指定的参数初始化VIC寄存器
    
    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   //初始化中断线函数
	
	
		writeCommand(RDATAC,CS2_Port,CS2_Pin);
		writeCommand(RDATAC,CS1_Port,CS1_Pin);
		
		delayMs(5);
    GPIO_SetBits(START_Port,START_Pin); // START HIGH
    delayMs(1);
		
		enableADS1298(CS1_Port,CS1_Pin);
} 


void EXTI15_10_IRQHandler(void)             //中断服务函数
{
	int i,j;
	u8 tmp[4][54],sum;
	u8 temp[54];
//	for(j=0;j<4;j++){//4 packs
		if(EXTI_GetITStatus(EXTI_Line12)==SET){
			for (i=0;i<27;i++)
			{
				tmp[0][i] = EMG_SendByte(0xff);//send and read, 27 bytes for 1 module, 54 for 2 modules
			}
			disableADS1298(CS1_Port,CS1_Pin);
			enableADS1298(CS2_Port,CS2_Pin);
			
			for (i=0;i<27;i++)
			{
				tmp[0][i+27] = EMG_SendByte(0xff);
			}
			disableADS1298(CS2_Port,CS2_Pin);
			enableADS1298(CS1_Port,CS1_Pin);
		}
//	}
//	for(i=0;i<27;i++){
//		if(i<3){
//			temp[i]=tmp[0][i];
//		}
//		else{
//			temp[i]=tmp[0][i]&0xFC+((tmp[1][i]&0xC0)>>6);
//			i++;
//			temp[i]=((tmp[1][i-1]&0x3C)<<2)+((tmp[2][i-1]&0xF0)>>4);			
//			i++;
//			temp[i]=((tmp[2][i-2]&0x0C)<<4)+((tmp[3][i-2]&0xFC)>>2);	
//		}
//	}
	/*
	AAAA AABB
	BBBB CCCC
	CCDD DDDD
	
	AAAA AA00     AAAA AA00
	BB00 0000 >>6 0000 00BB
	
	00BB BB00 <<2 BBBB 0000
	CCCC 0000 >>4 0000 CCCC
	
	0000 CC00 <<4 CC00 0000
	DDDD DD00 >>2 00DD DDDD
	*/
	
//	for(i=0;i<27;i++){
//		if(i<3){
//			temp[i+27]=tmp[0][i+27];
//		}
//		else{
//			temp[i+27]=tmp[0][i+27]&0xFC+((tmp[1][i+27]&0xC0)>>6);
//			i++;
//			temp[i+27]=((tmp[1][i+26]&0x3C)<<2)+((tmp[2][i+26]&0xF0)>>4);			
//			i++;
//			temp[i+27]=((tmp[2][i+25]&0x0C)<<4)+((tmp[3][i+25]&0xFC)>>2);	
//		}
//	}
	
	sum=0;
	usart1_sendByte(0xff);   		 	//包头两个0xff
	usart1_sendByte(0xff);
	usart1_sendByte(0x01);    		//EMG数据命令0x01
	for (i=0;i<54;i++)            //发送EMG数据
	{
		usart1_sendByte(tmp[0][i]);
		sum += tmp[0][i];
	}
	usart1_sendByte(sum);//send checksum
	EXTI_ClearITPendingBit(EXTI_Line12);
}

u8 shakeHands(GPIO_TypeDef* port, u16 pin)                     //握手
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


int configRLD(u8 rldp, u8 rldn, GPIO_TypeDef* port, u16 pin)         //设置RLD寄存器，定义configRLD函数，函数值为整型，形式参数rldp，rldn为无符号字符型
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

//-------------------------------- 底层函数----------------------------------------


u8 EMG_SendByte(u8 TxData)    //发送\接收数据
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  //检查发送是否完成，完成以后再发送数据
    SPI_I2S_SendData(SPI2,TxData); //通过外设SPIx发送一个数据
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  //检查接收受标志位，是否可以接受 
	  return SPI_I2S_ReceiveData(SPI2);   //返回通过SPIx最近接收的数据	
}

//读寄存器
u8 readRegister(u8 addr, GPIO_TypeDef* port, u16 pin)
{
    u8 r;
    enableADS1298(port,pin);
    EMG_SendByte(RREG|addr);
    EMG_SendByte(0x00); //读取1个
    r=EMG_SendByte(0xff); 
    disableADS1298(port,pin);
    return r;
}

//写寄存器
int writeRegister(u8 addr,u8 value, GPIO_TypeDef* port, u16 pin)         
{
    enableADS1298(port,pin);
    EMG_SendByte(WREG|addr);
    EMG_SendByte(0x00); //写1个
    EMG_SendByte(value);
    disableADS1298(port,pin);
    delayMs(1);
    if (readRegister(addr,port,pin)==value)
        return 0;
    else
        return -1;
}

//读多个寄存器，结果放在 regs 内
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

//写多个寄存器
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
    EMG_SendByte(0x00); //等待至少4个周期
    disableADS1298(port,pin);
}


//----------------------------------------------
// 功能: 写寄存器，最多尝试 tryTimes 次
// 返回：0  - 成功
//       -1 - 失败
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
