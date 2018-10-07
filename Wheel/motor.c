#include<reg52.h> 
#include<intrins.h>  
#define uchar unsigned char 
#define uint unsigned int 
sbit Left_P=P1^0; 
sbit Left_D=P1^1;
sbit Right_P=P1^2;  
sbit Right_D=P1^3;
uchar pwm,speed=10,SerialValue=0;   //���ڽ��յ���ֵ; 
void delaynus(uchar n)
{
	uchar i;
 	for(i=0;i<n;i++);
}
void Init()
{
   	TMOD=0x21; 	//�趨��ʱ��Ϊ��ʽ2 ���Զ���װ��Ķ�ʱ��  
	SM0=0;  	//�趨���ڹ�����ʽ1 10λ�첽�շ���  8λ����  
	SM1=1;  	//�趨���ڹ�����ʽ1 10λ�첽�շ���  8λ����    
	REN=1;  	//�����н���λ��REN=1�������������ݣ�REN=0����ֹ���ա�  
	IP=0x10;  
	TH0=(65536-100)/256;  
	TL0=(65536-100)%256;
	TH1=0xfd;	//װ���ֵ  
	TL1=0xfd; 	//װ���ֵ
	TR0=1;  	//����T0��ʱ��/������  
	ET0=1;  	//T0��ʱ���жϿ��أ�(��)   
	TR1=1;  	//����T1��ʱ��/������  
	ET1=1;  	//T1��ʱ���жϿ��أ�(��)  
	ES=1;  		//�����жϿ��أ�(��)    
}
void main() 
{  
	P1=0x00;
	Init();
	EA=1;
	while(1)  
	{ 
		if(pwm<speed)
		{
			switch(SerialValue)
			{
			case 0:{P1=0x00;break;} //ֹͣ0000
			case 1:{P1=0x05;break;} //ǰ��0101
			case 2:{P1=0x0a;break;} //����1010
			case 3:{P1=0x09;break;} //��ת1001
			case 4:{P1=0x06;break;} //��ת0110
			default:break;
			}
			delaynus(2);
			speed--;
			if(speed<10)speed=10;
		}
		else P1=0x00;
				
	} 
}

void serial() interrupt 4 
{  
	if(RI)
	{
	SerialValue=SBUF;
	speed=30;    
	RI=0;
	}  
}   
void timer0() interrupt 1 
{   
	TH0=(65536-100)/256;  
	TL0=(65536-100)%256;  
	pwm++; 
 	if(pwm>100)pwm=0;     
}