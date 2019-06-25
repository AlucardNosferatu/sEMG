#include "spi.h"


void SPI1_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
    SPI_InitTypeDef  SPI_InitStructure;//定义结构体变量
	  NVIC_InitTypeDef NVIC_InitStructure;//定义中断结构体
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
	
	  NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&NVIC_InitStructure);	 
	  
		SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
	  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//在第二个跳边沿采集数据
  	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;//空闲为高电平
	  SPI_InitStructure.SPI_CRCPolynomial=7;
	  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//8位数据
	  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//双线双向全双工
	  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//传输数据从MSB开始
  #ifdef _MASTER_DEVICE
		SPI_InitStructure.SPI_Mode=SPI_Mode_Master; //设置为主模式
	#else
	  SPI_InitStructure.SPI_Mode=SPI_Mode_Slave; //设置为从模式
	#endif
	  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; //软件控制 ssi位
	  SPI_Init(SPI1,&SPI_InitStructure);
		
	#ifdef _MASTER_DEVICE	
	  SPI1->CR1|=1<<8;  //SSI设置为高电平
	#else
		SPI1->CR1|=0<<8;
	#endif
		
	  SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);//开启中断
	  //SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
		SPI_Cmd(SPI1, ENABLE); //使能SPI外设  	 
}
   

u8 SPI1_ReadWriteByte(u8 TxData)
{ 
	u8 count;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		count++;
		if(count>200)
		return 0;	//如果200个计数还没完成发送，就返回0跳出这个函数	
	}
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	count=0;
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		count++;
		if(count>200)
		return 0;	//如果200个计数还没完成发送，就返回0跳出这个函数	
	}
	
	return SPI_I2S_ReceiveData(SPI1); //返回接收的数据
	//发送一个数据的同时，返回这次接收到的数据
}

u8 SPI1_ReadByte(u8 TxData)
{
	 u8 count;
   
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		count++;
		if(count>200)
		return 0;	//如果200个计数还没完成发送，就返回0跳出这个函数	
	}
	
	return SPI_I2S_ReceiveData(SPI1); //返回接收的数据
	 
}

void SPI1_WriteByte(u8 TxData)
{
	u8 count;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		count++;
		if(count>200)
		return;
  }		
	
	 SPI_I2S_SendData(SPI1, TxData); 
	
	
}


void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); 
} 
	
























	
	
	