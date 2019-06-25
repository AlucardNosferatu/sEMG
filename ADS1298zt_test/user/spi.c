#include "spi.h"


void SPI1_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;//����ṹ�����
    SPI_InitTypeDef  SPI_InitStructure;//����ṹ�����
	  NVIC_InitTypeDef NVIC_InitStructure;//�����жϽṹ��
	  
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
	  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;//�ڵڶ��������زɼ�����
  	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;//����Ϊ�ߵ�ƽ
	  SPI_InitStructure.SPI_CRCPolynomial=7;
	  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//8λ����
	  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
	  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//�������ݴ�MSB��ʼ
  #ifdef _MASTER_DEVICE
		SPI_InitStructure.SPI_Mode=SPI_Mode_Master; //����Ϊ��ģʽ
	#else
	  SPI_InitStructure.SPI_Mode=SPI_Mode_Slave; //����Ϊ��ģʽ
	#endif
	  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft; //������� ssiλ
	  SPI_Init(SPI1,&SPI_InitStructure);
		
	#ifdef _MASTER_DEVICE	
	  SPI1->CR1|=1<<8;  //SSI����Ϊ�ߵ�ƽ
	#else
		SPI1->CR1|=0<<8;
	#endif
		
	  SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);//�����ж�
	  //SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
		SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����  	 
}
   

u8 SPI1_ReadWriteByte(u8 TxData)
{ 
	u8 count;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		count++;
		if(count>200)
		return 0;	//���200��������û��ɷ��ͣ��ͷ���0�����������	
	}
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	count=0;
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		count++;
		if(count>200)
		return 0;	//���200��������û��ɷ��ͣ��ͷ���0�����������	
	}
	
	return SPI_I2S_ReceiveData(SPI1); //���ؽ��յ�����
	//����һ�����ݵ�ͬʱ��������ν��յ�������
}

u8 SPI1_ReadByte(u8 TxData)
{
	 u8 count;
   
	 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		count++;
		if(count>200)
		return 0;	//���200��������û��ɷ��ͣ��ͷ���0�����������	
	}
	
	return SPI_I2S_ReceiveData(SPI1); //���ؽ��յ�����
	 
}

void SPI1_WriteByte(u8 TxData)
{
	u8 count;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
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
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); 
} 
	
























	
	
	