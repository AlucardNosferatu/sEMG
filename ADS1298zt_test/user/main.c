#include "ads1298.h"
#include "usart.h"
#include "stm32f10x.h"
#include "schedule.h"
#include "wifi.h"
//#include "led.h"
int r;

int main(void)
{
	u8 r1, r2, c1, c2;

  SystemInit();
	
  usart1_Init(921600); 
	atk_8266_send_cmd("+++",NULL,200); // +++£ºÍË³öÍ¸´«
	if(atk_8266_send_cmd("AT\r\n","OK",100)) //ËµÃ÷²¨ÌØÂÊ²»¶Ô
	{
			usart1_Init(115200);
			atk_8266_send_cmd("AT+UART_DEF=921600,8,1,0,0\r\n","OK",500);//±£´æ²¨ÌØÂÊÉèÖÃ
			atk_8266_send_cmd("AT+RST\r\n","ready",1000);//ÖØÆôÄ£¿é
			usart1_Init(921600);
	}
	
	//Õâ¼¸¾äATÖ¸Áî¶¼ÊÇÊ²Ã´ÒâË¼
	atk_8266_send_cmd("AT+RST\r\n","ready",500); //ÖØÆôÄ£¿é
	atk_8266_send_cmd("AT+CWMODE=1\r\n","OK",500); //ÉèÖÃÎªSTAÄ£Ê½
	//atk_8266_send_cmd("AT+RST\r\n","ready",500); //ÖØÆôÄ£¿é
	
	atk_8266_send_cmd("AT+CWJAP_CUR=\"HARLab604_2G\",\"HARLab604!\"\r\n","OK",2000);//ÁÙÊ±Á¬½ÓAPÍøÂç£¬²»±£´æµ½Flash£¬µÚÒ»¸ö²ÎÊýÎªÄ¿±êAPµÄSSID£¨ÎÞÏßÍøÂçÃû³Æ£©£¬µÚ¶þ¸ö²ÎÊýÎªÎÞÏßÍøÂçÃÜÂë£
	//ÕâÀï¼ÓÊÇ·ñÁ¬½ÓµÄµÈ´ýÌõ¼þ
	
	
	atk_8266_send_cmd("AT+CIPMUX=0\r\n","OK",500);//Ê¹ÄÜµ¥Á¬½Ó
	//¶Ë¿ÚºÅÊÇ×Ô¼º¶¨ÒåµÄ£¬Ö»Òª¿Í»§¶ËºÍ·þÎñ¶ËÒ»ÖÂ¾ÍÐÐ
	
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.1.131\",8084\r\n","OK",2000); //´´½¨TCPÁ¬½Ó£¬Á¬½Óserver¶Ë£¬µÚÒ»¸ö²ÎÊýÎªÔ¶¶ËIPµØÖ·£¬µÚ¶þ¸ö²ÎÊýÎªÔ¶¶Ë¶Ë¿ÚºÅ
	//ÕâÀï¼ÓÅÐ¶ÏÌõ¼þ
	
	
	atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//½øÈëÍ¸´«Ä£Ê½
	atk_8266_send_cmd("AT+CIPSEND\r\n",">",500); //·¢ËÍÊý¾Ý
	
	Rx1_enable = 1; // ¿ªÆôU1½ÓÊÕ
	ads1298_init();
	r1 = resetADS1298(0,CS1_Port,CS1_Pin);
	r2 = resetADS1298(1,CS2_Port,CS2_Pin);
	GPIO_ResetBits(CLKSEL2_Port,CLKSEL2_Pin); 
	delayMs(100);
//	c1 = configForSquarewaveTest(CS1_Port,CS1_Pin);		//¿ªÊ¼·½²¨	
//	c2 = configForSquarewaveTest(CS2_Port,CS2_Pin);	
	c1 = configForNormalMeasurement(CS1_Port,CS1_Pin);		//¿ªÊ¼¼¡µç	
	c2 = configForNormalMeasurement(CS2_Port,CS2_Pin);
	
	beginReadDataC(); 
	
  while (1)                                     //Ö´ÐÐ³ÌÐò
  {
	}
	
}    

