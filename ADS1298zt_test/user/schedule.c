#include "schedule.h"

void delayMs(u32 i)	//??1864
{
	u32 temp;
	SysTick->LOAD=9000*i;	  //??????, 72MHZ?
	SysTick->CTRL=0X01;		//??,???????,???????
	SysTick->VAL=0;			//?????
	do
	{
		temp=SysTick->CTRL;	   //????????
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	//??????
	SysTick->CTRL=0;	//?????
	SysTick->VAL=0;		//?????
}

void delay_ms(u16 i)
{	 		  	  
	u32 temp;
	SysTick->LOAD=9000*i;	  //??????, 72MHZ?
	SysTick->CTRL=0X01;		//??,???????,???????
	SysTick->VAL=0;			//?????
	do
	{
		temp=SysTick->CTRL;	   //????????
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	//??????
	SysTick->CTRL=0;	//?????
	SysTick->VAL=0;		//????? 	    
} 

void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

void delayL()
{
	for (int i=0;i<=3;i++)
	{
		delayMs(1000);
	}

}