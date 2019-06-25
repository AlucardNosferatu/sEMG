#ifndef _usart2_H
#define _usart2_H

#include "stm32f10x.h"

void usart2_Init(int);	//USART2 ≥ı ºªØ
void usart2_send(char*, u8);
void usart2_sendByte(u8);
void usart2_sendString(char*);

#endif
