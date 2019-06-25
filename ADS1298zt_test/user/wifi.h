#ifndef _WIFI_H
#define _WIFI_H

#include "stm32f10x.h"

u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* atk_8266_check_cmd(u8 *str);

#endif

