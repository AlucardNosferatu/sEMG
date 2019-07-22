#include "ads1298.h"
#include "usart.h"
#include "stm32f10x.h"
#include "schedule.h"
#include "wifi.h"
//#include "led.h"
int r;

int main()
{
	u8 r1, r2, c1, c2;

  SystemInit();
	
  usart1_Init(921600);
	atk_8266_send_cmd("+++",NULL,200); // +++���˳�͸��
	if(atk_8266_send_cmd("AT\r\n","OK",100)) //˵�������ʲ���
	{
			usart1_Init(115200);
			atk_8266_send_cmd("AT+UART_DEF=921600,8,1,0,0\r\n","OK",500);//���沨��������
			atk_8266_send_cmd("AT+RST\r\n","ready",1000);//����ģ��
			usart1_Init(921600);
	}
	
	//�⼸��ATָ���ʲô��˼
	atk_8266_send_cmd("AT+RST\r\n","ready",500); //����ģ��
	atk_8266_send_cmd("AT+CWMODE=1\r\n","OK",500); //����ΪSTAģʽ
	//atk_8266_send_cmd("AT+RST\r\n","ready",500); //����ģ��
	
	atk_8266_send_cmd("AT+CWJAP_CUR=\"HARLab604_2G\",\"HARLab604!\"\r\n","OK",2000);//��ʱ����AP���磬�����浽Flash����һ������ΪĿ��AP��SSID�������������ƣ����ڶ�������Ϊ������������
	//������Ƿ����ӵĵȴ�����
	
	
	atk_8266_send_cmd("AT+CIPMUX=0\r\n","OK",500);//ʹ�ܵ�����
	//�˿ں����Լ�����ģ�ֻҪ�ͻ��˺ͷ����һ�¾���
	
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.1.131\",8081\r\n","OK",2000); //����TCP���ӣ�����server�ˣ���һ������ΪԶ��IP��ַ���ڶ�������ΪԶ�˶˿ں�
	//������ж�����
	
	
	atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//����͸��ģʽ
	atk_8266_send_cmd("AT+CIPSEND\r\n",">",500); //��������
	
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

