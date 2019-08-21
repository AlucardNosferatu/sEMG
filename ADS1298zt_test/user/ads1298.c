#include "ads1298.h"
#include "schedule.h"
#include "stm32f10x_exti.h"
//#define HIGHSPEED
//#define COMPRESS

int count=0;

void ads1298_init(void)      //ads1298³õÊ¼»¯//IO¿Ú³õÊ¼»¯
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE );//PORTA¡¢PORTBÊ±ÖÓÊ¹ÄÜ 
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2Ê±ÖÓÊ¹ÄÜ 
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //Ê¹ÓÃPB3 PB4 Ğè½ûÖ¹JTAG
    
		//EMG_Ready   DRDY2  ³õÊ¼»¯   ¶Ë¿ÚPB.2   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ÉÏÀ­ÊäÈë
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	  //EMG_Ready   DRDY1  ³õÊ¼»¯   ¶Ë¿ÚPA.12   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ÉÏÀ­ÊäÈë
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    //EMG_SPI2 SCK  MISO  MOSI  ³õÊ¼»¯  PB.13/14/15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB.13/14/15ÉÏÀ­
		
		//ÅäÖÃSPI2µÄÊä³ö
    SPI_I2S_DeInit(SPI2); //½«SPI2¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊı¾İÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//ÉèÖÃSPIµÄÊı¾İ´óĞ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//´®ĞĞÍ¬²½Ê±ÖÓµÄ¿ÕÏĞ×´Ì¬Îª¸ßµçÆ½
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//´®ĞĞÍ¬²½Ê±ÖÓµÄµÚ¶ş¸öÌø±äÑØ£¨ÉÏÉı»òÏÂ½µ£©Êı¾İ±»²ÉÑù
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSĞÅºÅÓÉÓ²¼ş£¨NSS¹Ü½Å£©»¹ÊÇÈí¼ş£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSĞÅºÅÓĞSSIÎ»¿ØÖÆ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//Ö¸¶¨Êı¾İ´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êı¾İ´«Êä´ÓMSBÎ»¿ªÊ¼
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCÖµ¼ÆËãµÄ¶àÏîÊ½
    SPI_Init(SPI2, &SPI_InitStructure);  //¸ù¾İSPI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ÷
    
    SPI_NSSInternalSoftwareConfig(SPI2,SPI_NSSInternalSoft_Set);//ÎªÑ¡¶¨µÄSPIÈí¼şÅäÖÃÄÚ²¿NSS¹Ü½Å
    SPI_Cmd(SPI2, ENABLE); //Ê¹ÄÜSPIÍâÉè
    
    //EMG_RESET1/CS1/START/RESET2 PC.6/7/8/9 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//¶Ë¿ÚÄ£Ê½£ºÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		
		//EMG_CLKSEL2 PB.12 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//¶Ë¿ÚÄ£Ê½£ºÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		//EMG_CS2/CLKSEL1 PA.8/11 
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//¶Ë¿ÚÄ£Ê½£ºÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
		
    GPIO_SetBits(RESET1_Port,RESET1_Pin); //Reset1ÖÃ¸ßµãÆ½
		GPIO_SetBits(RESET2_Port,RESET2_Pin); //Reset2ÖÃ¸ßµãÆ½
    GPIO_ResetBits(START_Port,START_Pin); //StartÖÃµÍµçÆ½
		GPIO_SetBits(CLKSEL1_Port,CLKSEL1_Pin);//CLK_SEL1ÖÃ¸ßµçÆ½
		GPIO_SetBits(CLKSEL2_Port,CLKSEL2_Pin);//CLK_SEL2ÖÃ¸ßµçÆ½
		disableADS1298(CS1_Port,CS1_Pin);     //CSÊ§ÄÜ
		disableADS1298(CS2_Port,CS2_Pin);
}

// flag = 0 Master
// flag = 1 Slave
int resetADS1298(u8 flag, GPIO_TypeDef* port, u16 pin)                  //¶Ôads1298½øĞĞÅäÖÃ
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
		
#ifdef HIGHSPEED
		if(flag==0)r = tryWriteRegister(CONFIG1,0xA5,5,port,pin);     // HS Mode: 1k  SPS BIN: 1010 0101
		else r = tryWriteRegister(CONFIG1,0x85,5,port,pin);						// HS Mode: 1k  SPS BIN: 1000 0101	
#else
		if(flag==0)r = tryWriteRegister(CONFIG1,0x26,5,port,pin);			// LR Mode: 250 SPS BIN: 0010 0110
		else r = tryWriteRegister(CONFIG1,0x06,5,port,pin);						// LR Mode: 250 SPS BIN: 0000 0110
#endif
		if (r!=0)return -1;
		
		r = tryWriteRegister(CONFIG2,0x00,5,port,pin);   // input short 
    if (r!=0)return -1;
		
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

int configForSquarewaveTest(GPIO_TypeDef* port,u16 pin)       //ÅäÖÃ³É·½²¨²âÁ¿Ä£Ê½
{ 
	int r=-1;
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

int configForNoiseTest(GPIO_TypeDef* port, u16 pin)            //ÅäÖÃ³ÉÔëÉù²âÁ¿Ä£Ê½
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


int configForNormalMeasurement(GPIO_TypeDef* port, u16 pin)    //ÅäÖÃ³ÉÕı³£²âÁ¿Ä£Ê½
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

void beginReadDataC()               //ÉèÖÃÖĞ¶Ï              
	{
	  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
		
		/* Enable SYSCFG clock */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //¿ªÆôAFIOÊ±ÖÓ
 
    //EXTI4 NVIC ÅäÖÃ
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;       		//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//ÇÀÕ¼ÓÅÏÈ¼¶3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//×ÓÓÅÏÈ¼¶3
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQÍ¨µÀÊ¹ÄÜ
//    NVIC_Init(&NVIC_InitStructure);														//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷
//    
//    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØÖĞ¶Ï
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);   //³õÊ¼»¯ÖĞ¶ÏÏßº¯Êı
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;       		//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;	//ÇÀÕ¼ÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//×ÓÓÅÏÈ¼¶3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQÍ¨µÀÊ¹ÄÜ
    NVIC_Init(&NVIC_InitStructure);														//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷
    
    /* Connect EXTI2 Line to PB2 pin */												//PB2 DRDY2
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //ÏÂ½µÑØÖĞ¶Ï
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   //³õÊ¼»¯ÖĞ¶ÏÏßº¯Êı
	
	
		writeCommand(RDATAC,CS2_Port,CS2_Pin);
		writeCommand(RDATAC,CS1_Port,CS1_Pin);
		
		delayMs(5);
    GPIO_SetBits(START_Port,START_Pin); // START HIGH
    delayMs(1);
		
		enableADS1298(CS1_Port,CS1_Pin);
} 


void EXTI15_10_IRQHandler(void)             //ÖĞ¶Ï·şÎñº¯Êı
{
	int i;
	u8 tmp[4][54],sum;
#ifdef COMPRESS
	int j;
	u8 temp[54];
#endif
	if(EXTI_GetITStatus(EXTI_Line12)==SET){
#ifdef COMPRESS
		for (i=0;i<27;i++){
			tmp[count][i] = EMG_SendByte(0xff);//send and read, 27 bytes for 1 module, 54 for 2 modules
		}
		disableADS1298(CS1_Port,CS1_Pin);
		enableADS1298(CS2_Port,CS2_Pin);
		for (i=0;i<27;i++){
			tmp[count][i+27] = EMG_SendByte(0xff);
		}
		disableADS1298(CS2_Port,CS2_Pin);
		enableADS1298(CS1_Port,CS1_Pin);
		
		count++;
		if(count==3){
			for(i=0;i<27;i++){
				if(i<3)temp[i]=tmp[0][i];
				else{
					temp[i]=tmp[0][i]&0xFC+((tmp[1][i]&0xC0)>>6);
					i++;
					temp[i]=((tmp[1][i-1]&0x3C)<<2)+((tmp[2][i-1]&0xF0)>>4);
					i++;
					temp[i]=((tmp[2][i-2]&0x0C)<<4)+((tmp[3][i-2]&0xFC)>>2);
				}
			}
			for(i=0;i<27;i++){
				if(i<3)temp[i+27]=tmp[0][i+27];
				else{
					temp[i+27]=tmp[0][i+27]&0xFC+((tmp[1][i+27]&0xC0)>>6);
					i++;
					temp[i+27]=((tmp[1][i+26]&0x3C)<<2)+((tmp[2][i+26]&0xF0)>>4);
					i++;
					temp[i+27]=((tmp[2][i+25]&0x0C)<<4)+((tmp[3][i+25]&0xFC)>>2);
				}
			}
			sum=0;
			usart1_sendByte(0xff);   		 	//°üÍ·Á½¸ö0xff
			usart1_sendByte(0xff);
			usart1_sendByte(0x01);    		//EMGÊı¾İÃüÁî0x01
			for (i=0;i<54;i++){           //·¢ËÍEMGÊı¾
				usart1_sendByte(temp[i]);
				sum += temp[i];
			}
			usart1_sendByte(sum);//send checksum
		}
		
		

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
#else
		for (i=0;i<27;i++){
			tmp[0][i] = EMG_SendByte(0xff);//send and read, 27 bytes for 1 module, 54 for 2 modules
		}
		disableADS1298(CS1_Port,CS1_Pin);
		enableADS1298(CS2_Port,CS2_Pin);
		
		for (i=0;i<27;i++){
			tmp[0][i+27] = EMG_SendByte(0xff);
		}
		disableADS1298(CS2_Port,CS2_Pin);
		enableADS1298(CS1_Port,CS1_Pin);
		
		sum=0;
		usart1_sendByte(0xff);   		 	//°üÍ·Á½¸ö0xff
		usart1_sendByte(0xff);
		usart1_sendByte(0x01);    		//EMGÊı¾İÃüÁî0x01

		for (i=0;i<54;i++){           //·¢ËÍEMGÊı¾İ
			usart1_sendByte(tmp[0][i]);
			sum += tmp[0][i];
		}
		usart1_sendByte(sum);//send checksum
#endif
	}
	EXTI_ClearITPendingBit(EXTI_Line12);
}

u8 shakeHands(GPIO_TypeDef* port, u16 pin)                     //ÎÕÊÖ
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


int configRLD(u8 rldp, u8 rldn, GPIO_TypeDef* port, u16 pin)         //ÉèÖÃRLD¼Ä´æÆ÷£¬¶¨ÒåconfigRLDº¯Êı£¬º¯ÊıÖµÎªÕûĞÍ£¬ĞÎÊ½²ÎÊırldp£¬rldnÎªÎŞ·ûºÅ×Ö·ûĞÍ
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

//-------------------------------- µ×²ãº¯Êı----------------------------------------


u8 EMG_SendByte(u8 TxData)    //·¢ËÍ\½ÓÊÕÊı¾İ
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  //¼ì²é·¢ËÍÊÇ·ñÍê³É£¬Íê³ÉÒÔºóÔÙ·¢ËÍÊı¾İ
    SPI_I2S_SendData(SPI2,TxData); //Í¨¹ıÍâÉèSPIx·¢ËÍÒ»¸öÊı¾İ
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);  //¼ì²é½ÓÊÕÊÜ±êÖ¾Î»£¬ÊÇ·ñ¿ÉÒÔ½ÓÊÜ 
	  return SPI_I2S_ReceiveData(SPI2);   //·µ»ØÍ¨¹ıSPIx×î½ü½ÓÊÕµÄÊı¾İ	
}

//¶Á¼Ä´æÆ÷
u8 readRegister(u8 addr, GPIO_TypeDef* port, u16 pin)
{
    u8 r;
    enableADS1298(port,pin);
    EMG_SendByte(RREG|addr);
    EMG_SendByte(0x00); //¶ÁÈ¡1¸ö
    r=EMG_SendByte(0xff); 
    disableADS1298(port,pin);
    return r;
}

//Ğ´¼Ä´æÆ÷
int writeRegister(u8 addr,u8 value, GPIO_TypeDef* port, u16 pin)         
{
    enableADS1298(port,pin);
    EMG_SendByte(WREG|addr);
    EMG_SendByte(0x00); //Ğ´1¸ö
    EMG_SendByte(value);
    disableADS1298(port,pin);
    delayMs(1);
    if (readRegister(addr,port,pin)==value)
        return 0;
    else
        return -1;
}

//¶Á¶à¸ö¼Ä´æÆ÷£¬½á¹û·ÅÔÚ regs ÄÚ
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

//Ğ´¶à¸ö¼Ä´æÆ÷
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
    EMG_SendByte(0x00); //µÈ´ıÖÁÉÙ4¸öÖÜÆÚ
    disableADS1298(port,pin);
}


//----------------------------------------------
// ¹¦ÄÜ: Ğ´¼Ä´æÆ÷£¬×î¶à³¢ÊÔ tryTimes ´Î
// ·µ»Ø£º0  - ³É¹¦
//       -1 - Ê§°Ü
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
