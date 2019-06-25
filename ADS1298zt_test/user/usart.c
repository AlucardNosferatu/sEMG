#include "usart.h"
#include "ads1298.h"
#include "schedule.h"

u8 Rx1_enable = 0;
 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART1_RX_STA=0; 

void usart1_Init(int baudRate)	//USART1 初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //打开复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //打开串口时钟

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//配置串口 
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	
	//--- 设置NVIC参数 ---
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   				//打开USART1的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; 	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 						//响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						 //使能
	NVIC_Init(&NVIC_InitStructure);

	//--- 设置中断 ---
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能串口接收中断
	
	USART_ClearFlag(USART1,USART_FLAG_TC);	//清空发送完成标志位
	USART_ClearFlag(USART1,USART_FLAG_RXNE);	//清空发送完成标位
	USART_Cmd(USART1, ENABLE); //串口使能
	
	TIM4_Init(999,7199);		//10ms中断
	TIM4_Set(0);			//关闭定时器4
}

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Res=USART_ReceiveData(USART1);
		if(Rx1_enable) // 透传
		{
				parse(Res);
		}
		else //Wifi
		{
				if(USART1_RX_STA<USART1_MAX_RECV_LEN)		//还可以接收数据
				{
					TIM_SetCounter(TIM4,0);//计数器清空        				 
					if(USART1_RX_STA==0) TIM4_Set(1);	 	//使能定时器4的中断 
					USART1_RX_BUF[USART1_RX_STA++]=Res;		//记录接收到的值	 
				}else 
				{
					USART1_RX_STA|=1<<15;					//强制标记接收完成
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
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); // 等待发送完成
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
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); // 等待发送完成
}


// 解析接收字符
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

//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART1_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志    
		TIM4_Set(0);			//关闭TIM4  
	}	    
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM4,0);//计数器清空
		TIM_Cmd(TIM4, ENABLE);  //使能TIMx	
	}else TIM_Cmd(TIM4, DISABLE);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能//TIM4时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}