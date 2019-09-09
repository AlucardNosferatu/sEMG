#include "ads1298.h"
#include "usart.h"
#include "stm32f10x.h"
#include "schedule.h"
#include "wifi.h"
//#include "led.h"
int r;

void ESP8266(void){
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
	
	atk_8266_send_cmd("AT+CWJAP_CUR=\"HARLab609_2G\",\"HARLab609!\"\r\n","OK",2000);//��ʱ����AP���磬�����浽Flash����һ������ΪĿ��AP��SSID�������������ƣ����ڶ�������Ϊ������������
	//������Ƿ����ӵĵȴ�����
	
	
	atk_8266_send_cmd("AT+CIPMUX=0\r\n","OK",500);//ʹ�ܵ�����
	//�˿ں����Լ�����ģ�ֻҪ�ͻ��˺ͷ����һ�¾���
	
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.0.166\",8081\r\n","OK",2000); //����TCP���ӣ�����server�ˣ���һ������ΪԶ��IP��ַ���ڶ�������ΪԶ�˶˿ں�
	//������ж�����
	
	
	atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//����͸��ģʽ
	atk_8266_send_cmd("AT+CIPSEND\r\n",">",500); //��������
}

void SIM900A(void){
	u8 ERROR=1;
	atk_8266_send_cmd("++++\r\n",NULL,200);
	delay_ms(5000);
	while(ERROR){
			ERROR=atk_8266_send_cmd("AT+CIPMODE=1\r\n","OK",500);//����͸��ģʽ
			delay_ms(5000);
	}
	ERROR=1;
	while(ERROR){
			ERROR=atk_8266_send_cmd("AT+CIPCSGP=1,\"CMNET\"\r\n","OK",500);
			delay_ms(5000);
	}
	ERROR=1;
	while(ERROR){
			ERROR=atk_8266_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n","OK",5000);
			delay_ms(5000);
	}
	ERROR=1;
	while(ERROR){
			ERROR=atk_8266_send_cmd("AT+CGATT=1\r\n","OK",10000);
			delay_ms(5000);
	}
	ERROR=1;
	while(ERROR){
			ERROR=atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"112.74.89.58\",\"46495\"\r\n",NULL,10000);
			delay_ms(5000);
	}
	delay_ms(25000);
}

int main()
{
	u8 r1, r2, c1, c2;

  SystemInit();
	
  usart1_Init(921600);
	ESP8266();
//	SIM900A();
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

