#ifndef ADS1298_H
#define ADS1298_H

#include "stm32f10x_spi.h"
#include "usart.h"

// ADS1298 commands
#define WREG 0x40
#define RREG 0x20
#define START 0x08
#define STOP 0x0a
#define RDATAC 0x10
#define SDATAC 0x11
#define ADS_RESET 0x06

#define ADS1298_DEVICE_ID 0x92

// Register Address
#define CONFIG1 0x01
#define CONFIG2 0x02
#define CONFIG3 0x03

#define CH1SET 0x05
#define CH2SET 0x06
#define CH3SET 0x07
#define CH4SET 0x08
#define CH5SET 0x09
#define CH6SET 0x0a
#define CH7SET 0x0b
#define CH8SET 0x0c

#define RLD_SENSP 0x0d
#define RLD_SENSN 0x0e

// GPIO Pin
#define CLKSEL1_Port GPIOA  //PA11
#define CLKSEL1_Pin GPIO_Pin_11
#define CS2_Port GPIOA	//PA8
#define CS2_Pin GPIO_Pin_8
#define RESET2_Port GPIOC  //PC9
#define RESET2_Pin GPIO_Pin_9
#define START_Port GPIOC //PC8
#define START_Pin GPIO_Pin_8
#define CS1_Port GPIOC //PC7
#define CS1_Pin GPIO_Pin_7
#define RESET1_Port GPIOC //PC6
#define RESET1_Pin GPIO_Pin_6
#define CLKSEL2_Port GPIOB //PB12
#define CLKSEL2_Pin GPIO_Pin_12

#define enableADS1298(x,y) GPIO_ResetBits(x,y)
#define disableADS1298(x,y) GPIO_SetBits(x,y)


u8 EMG_SendByte(u8 TxData);
void writeCommand(u8 cmd, GPIO_TypeDef* port, u16 pin);
int writeRegister(u8 addr, u8 value, GPIO_TypeDef* port, u16 pin);
u8 readRegister(u8 add, GPIO_TypeDef* port, u16 pin);
void readNRegister(u8 start, u8 count, u8* regs, GPIO_TypeDef* port, u16 pin);
int writeNRegister(u8 start, u8 count, u8* regs, GPIO_TypeDef* port, u16 pin);
int tryWriteRegister(u8 addr, u8 value, int tryTimes, GPIO_TypeDef* port, u16 pin);

void ads1298_init(void);
void beginReadDataC(void);
int configForSquarewaveTest(GPIO_TypeDef* port, u16 pin);
int configForNormalMeasurement(GPIO_TypeDef* port, u16 pin);
int configForNoiseTest(GPIO_TypeDef* port, u16 pin);
int resetADS1298(u8 flag, GPIO_TypeDef* port, u16 pin);
u8 shakeHands(GPIO_TypeDef* port, u16 pin);
int stopReadDataC(void);
int configRLD(u8 rldp, u8 rldn, GPIO_TypeDef* port, u16 pin);

#endif
