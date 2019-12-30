#include "ads1298.h"
#include "usart.h"
#include "stm32f10x.h"
#include "schedule.h"
#include "wifi.h"
//#include "led.h"
//#define emg_mode
int r;

void switch_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�����������
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
	atk_8266_send_cmd("+++",NULL,200); // +++���˳�͸��
	if(atk_8266_send_cmd("AT\r\n","OK",100)) //˵�������ʲ���
	{
			usart1_Init(1036800-br);
			atk_8266_send_cmd("AT+UART_DEF=115200,8,1,0,0\r\n","OK",500);//���沨��������
			atk_8266_send_cmd("AT+RST\r\n","ready",1000);//����ģ��
			usart1_Init(br);
	}

	atk_8266_send_cmd("AT+RST\r\n","ready",500); //����ģ��
	atk_8266_send_cmd("AT+CWMODE=1\r\n","OK",500); //����ΪSTAģʽ
	//atk_8266_send_cmd("AT+RST\r\n","ready",500); //����ģ��
	
	atk_8266_send_cmd("AT+CWJAP_CUR=\"HARLab604_2G\",\"HARLab604!\"\r\n","OK",2000);//��ʱ����AP���磬�����浽Flash����һ������ΪĿ��AP��SSID�������������ƣ����ڶ�������Ϊ������������
	//������Ƿ����ӵĵȴ�����
	
	atk_8266_send_cmd("AT+CIPMUX=0\r\n","OK",500);//ʹ�ܵ�����
	//�˿ں����Լ�����ģ�ֻҪ�ͻ��˺ͷ����һ�¾���
	
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"10.20.96.39\",8081\r\n","OK",2000); //����TCP���ӣ�����server�ˣ���һ������ΪԶ��IP��ַ���ڶ�������ΪԶ�˶˿ں�
	//������ж�����
	
	atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//����͸��ģʽ
	atk_8266_send_cmd("AT+CIPSEND\r\n",">",500); //��������
	
}

void ads1298_loop()
{
	u8 r1, r2, c1, c2;
	Rx1_enable = 1; // ����U1����
	ads1298_init();
	r1 = resetADS1298(0,CS1_Port,CS1_Pin);
	r2 = resetADS1298(1,CS2_Port,CS2_Pin);
	GPIO_ResetBits(CLKSEL2_Port,CLKSEL2_Pin); 
	delayMs(100);
//	c1 = configForSquarewaveTest(CS1_Port,CS1_Pin);		//��ʼ����	
//	c2 = configForSquarewaveTest(CS2_Port,CS2_Pin);	
	c1 = configForNormalMeasurement(CS1_Port,CS1_Pin);		//��ʼ����	
	c2 = configForNormalMeasurement(CS2_Port,CS2_Pin);
	
	beginReadDataC(); 
	
  while (1)                                     //ִ�г���
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


