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
	SysTick->LOAD=nus; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}

void delayL()
{
	for (int i=0;i<=3;i++)
	{
		delayMs(1000);
	}

}