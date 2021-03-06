#include "ads1298.h"
#include "usart.h"
#include "stm32f10x.h"
#include "schedule.h"
#include "wifi.h"
//#include "led.h"
#define emg_mode
int r;

void switch_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
	
void switch_to_mcu()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void switch_to_imu()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void wifi_init(int br)
{
	atk_8266_send_cmd("+++",NULL,200); // +++：退出透传
	if(atk_8266_send_cmd("AT\r\n","OK",100)) //说明波特率不对
	{
			usart1_Init(1036800-br);
			char BaudConf[200];
			sprintf(BaudConf,"AT+UART_DEF=%d,8,1,0,0\r\n",br);
			
			atk_8266_send_cmd(BaudConf,"OK",500);//保存波特率设置
			atk_8266_send_cmd("AT+RST\r\n","ready",1000);//重启模块
			usart1_Init(br);
	}

	atk_8266_send_cmd("AT+RST\r\n","ready",500); //重启模块
	atk_8266_send_cmd("AT+CWMODE=1\r\n","OK",500); //设置为STA模式
	//atk_8266_send_cmd("AT+RST\r\n","ready",500); //重启模块
	
	atk_8266_send_cmd("AT+CWJAP_CUR=\"HARLab604_2G\",\"HARLab604!\"\r\n","OK",2000);//临时连接AP网络，不保存到Flash，第一个参数为目标AP的SSID（无线网络名称），第二个参数为无线网络密码�
	//这里加是否连接的等待条件
	
	atk_8266_send_cmd("AT+CIPMUX=0\r\n","OK",500);//使能单连接
	//端口号是自己定义的，只要客户端和服务端一致就行
	
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.1.80\",8084\r\n","OK",2000); //创建TCP连接，连接server端，第一个参数为远端IP地址，第二个参数为远端端口号
	//这里加判断条件
	
	atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//进入透传模式
	atk_8266_send_cmd("AT+CIPSEND\r\n",">",500); //发送数据
	
}

void ads1298_loop()
{
	u8 r1, r2, c1, c2;
	Rx1_enable = 1; // 开启U1接收
	ads1298_init();
	r1 = resetADS1298(0,CS1_Port,CS1_Pin);
	r2 = resetADS1298(1,CS2_Port,CS2_Pin);
	GPIO_ResetBits(CLKSEL2_Port,CLKSEL2_Pin); 
	delayMs(100);
//	c1 = configForSquarewaveTest(CS1_Port,CS1_Pin);		//开始方波	
//	c2 = configForSquarewaveTest(CS2_Port,CS2_Pin);	
	c1 = configForNormalMeasurement(CS1_Port,CS1_Pin);		//开始肌电	
	c2 = configForNormalMeasurement(CS2_Port,CS2_Pin);
	
	beginReadDataC(); 
	
  while (1)                                     //执行程序
  {
	}
}

int main()
{
	int baud_rate;
#ifdef emg_mode
	baud_rate = 921600;
#else
	baud_rate = 115200;
#endif
	
	SystemInit();
  usart1_Init(baud_rate);
	switch_init();
	switch_to_mcu();
	wifi_init(baud_rate);
	
#ifdef emg_mode
	switch_to_mcu();
	ads1298_loop();
#else
	switch_to_imu();
	while(1)
		{
		}
#endif
}    


