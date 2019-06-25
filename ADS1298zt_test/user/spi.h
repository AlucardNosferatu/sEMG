#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x_spi.h"

void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
u8 SPI1_ReadByte(u8 TxData);
void SPI1_WriteByte(u8 TxData);
void SPI1_IRQHandler(void);

#define _MASTER_DEVICE
#define SlaveDeviceID 0x00

#endif
