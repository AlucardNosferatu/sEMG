#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x_spi.h"

void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 SPI1_ReadByte(u8 TxData);
void SPI1_WriteByte(u8 TxData);
void SPI1_IRQHandler(void);

#define _MASTER_DEVICE
#define SlaveDeviceID 0x00

#endif
