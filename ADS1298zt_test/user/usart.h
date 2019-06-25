#ifndef _usart_H
#define _usart_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"	

#define USART1_MAX_RECV_LEN 1024

#define	ADS_reset 0x01
#define	ADS_gotoTestMode 0x02
#define	ADS_gotoNoiseMode 0x03 
#define	ADS_gotoMeasurementMode 0x04
#define	ADS_beginRead 0x05
#define	ADS_stopRead 0x06
#define	ADS_setRLD 0x07
#define	ADS_resetRLD 0x08


void parse(u8 cmd);

void usart1_Init(int);	//USART1 ≥ı ºªØ
void usart1_send(char*, u8);
void usart1_sendByte(u8);
void usart1_sendString(char*);

extern u8 Rx1_enable;
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];
extern u16 USART1_RX_STA; 

void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);



#endif

