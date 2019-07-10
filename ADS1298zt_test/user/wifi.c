#include "wifi.h"
#include "usart.h"
#include "schedule.h"

//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	usart1_sendString(cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					break;//�õ���Ч���� 
				}
					USART1_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 

