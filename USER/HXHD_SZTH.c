//#include <STC12C5A60S2.H>
//#include <stdio.h>
//#include <string.h>
//#include <intrins.h>
#include "HXHD_SZTH.h"

//#define S2RI 0x01	//����2�����ж������־λ
//#define S2TI 0x02	//����2�����ж������־λ

#define CONVERT_HEX_CHAR(c, h, l) l = 0x0F&c;h=0x0F&(c>>4);l =(l<=9)?('0'+l):('A'+l-10);h=(h<=9)?('0'+h):('A'+h-10)//16����ת��

void Delay1ms(int);
void UartInit(void);
void Delay(int);
void OutChar0(unsigned char);
void OutChar1(unsigned char);
void TTCA (void);
void RD(void);
void PG(void);
void ESOUT(void);
void PSOUT(void);
void ICOUT(void);

//*************************************CRCУ��***********************************//
unsigned char BDCRC(char *buf, int size, char h[2])
{
	unsigned char crc = 0;
	unsigned int i = 0;
	for (i = 0; i<size; ++i)
	{
		crc^=buf[i];		
	}
	CONVERT_HEX_CHAR(crc, h[0], h[1]);			
	return crc;
}
int i,ii,iii,iiii;
char SendDat0[101];
char UART_Buffer_Size0;
char UART_Buffer0[200];//={"$TTCA,1,136001,1,0,14,abcdefghijklmn,EA\r\n"};
char SendDat1[101];
char UART_Buffer_Size1;
char UART_Buffer1[200];//={"$RD,28,424956,1,14,ABCDEFGHIJKLMN,EA\r\n"};
char CRCtemp[99];
char C[2];
unsigned char count0,count1,k;
unsigned char TTCA_1,TTCA_2;
unsigned char PG_1;
unsigned char RD_1,RD_2;
unsigned char ES_1;
unsigned char PS_1;
unsigned char IC_1;
unsigned char CRCVALUE;
unsigned char Flag0,Flag1;
//sbit power=P1^7;//P1�ڣ�P1����һ���ڲ��ṩ���������8λ˫��I/O�ڣ�P1�ڻ������ܽ������4TTL�ŵ�����
								//P1�ڹܽ�д�롰1���󣬵�λ���ڲ�����Ϊ�ߣ����������룬P1�ڱ��ⲿ����Ϊ�͵�ƽʱ����������������������ڲ�������Ե�ʡ�
								//��FLASH��̺�У��ʱ��P1����Ϊ�ڰ�λ��ַ����;
//sbit mm=P2^0;//P2�ڣ�P2��Ϊһ���ڲ����������8λ˫��I/O�ڣ�P2�ڻ������ɽ��գ����4��TTL�ŵ�������P2�ڱ�д��1��ʱ����ܽŵ�λ���ڲ������������ߣ�����Ϊ���롣
						 //��Ϊ����ʱ��P2�ڵĹܽŵ�λ���ⲿ���ͣ���������������������ڲ�������Ե�ʡ�
						 //P2�ڵ������ⲿ����洢����16λ��ַ�ⲿ���ݴ洢�����д�ȡʱ��P2�������ַ�ĸ߰�λ��
						 //�ڸ�����ַ��1��ʱ���������ڲ����������ƣ������ⲿ��λ��ַ���ݴ洢�����ж�дʱ��P2����������⹦�ܼĴ��������ݡ�
						 //P2����FLASH��̺�У��ʱ���ո߰�λ��ַ�źźͿ����ź�;

void HXHD_SZTH(void)
{
//	int timetep0=0,timetep1=0;
	char BDINIT[15]={"$DM,03,ALL,67\r\n"};//������ʼ��
	i=0,ii=0,iii=0,iiii=0;
	UART_Buffer_Size0=0,UART_Buffer_Size1=0;
	count0=0,count1=0;
	Flag0=0,Flag1=0;
	//power =1;//��Ӧ�����sbit power=P1^7;
	Delay(200);
	//power=0;
	//UartInit();//���ڳ�ʼ�������ﲻ��Ҫ

	for(i=0;i<19600;i++)
		{	
			Delay(65535);
			Delay(65535);
			Delay(65535);
//			OutChar0(i+30);
//			OutChar0('\r');
//			OutChar0('\n');
		//mm=i%2;//��Ӧsbit mm=P2^0;
		}
	for(i=0;i<15;i++)
		OutChar1(BDINIT[i]);

	while(1)
	{
/*******************************************����1�����жϴ���*****************************************/
//		OutChar0('\n');
//		Delay(100);
		if(Flag0==1)
		{
			Flag0=0;
			k=0;
			switch(UART_Buffer0[1])
			{
				case 'Q': //��ѯ�ն�״̬$QSTA����ѯ����$QAPP
					switch(UART_Buffer0[2])
					{
						case 'S'://************************��ѯ�ն�״̬$QSTA
						break;
						case 'A'://************************��ѯ����$QAPP
						break;
					}					
				break;
				case 'S'://********************************�����ն�״̬$STST
					_nop_();
				break;
				case 'I'://********************************��ѯ����˳���$IQMS
				break;
				case 'P'://��λ����$PAPP��������λPEND
					switch(UART_Buffer0[2])
					{
						case 'A'://************************��λ����$PAPP
						break;
						case 'E'://************************������λPEND
						break;
					}
				break;
				case 'D'://********************************����ͨ������$DCAP
				break;
				case 'T'://�ն˵��ն�ͨ������$TTCA���ն˵��̶�ͨ������$TNCA���ն˲�ѯ�������±���$TINC����ʱ����$TAPP
					switch(UART_Buffer0[2])
					{
//*********************************************************�ն˵��ն�ͨ������$TTCA*********************************************************//
						case 'T':
							TTCA();
   						break;
//*********************************************************�ն˵��ն�ͨ������$TTCA*********************************************************//
						case 'N'://************************�ն˵��̶�ͨ������$TNCA
						break;
						case 'I'://************************�ն˲�ѯ�������±���$TINC
						break;
						case 'A'://************************��ʱ����$TAPP
						break;
					}
					break;
				case 'C'://ͨ�Ż�ִ����$CACA��ͨ������ɹ�״̬$COSS
					switch(UART_Buffer0[2])
					{
						case 'A'://************************ͨ�Ż�ִ����$CACA
						break;
						case 'O'://************************ͨ������ɹ�״̬$COSS
						break;
					}
				break;
				case 'L'://********************************��������ͨ������$LTCA
				break;
				case 'G':
					switch(UART_Buffer0[4])
					{
						case 'A'://************************GPS��λ����$GPSA
						break;
						case 'E'://************************����GPS��λ$GPSE
						break;
					}
				break;
				default:
				break;
 			}
  
			Flag0=0;
			count0=0;
			UART_Buffer_Size0=0;
		}
/**********************************************����2�жϴ���ʼ****************************************/
		if(Flag1==1)
		{
			Flag1=0;
			switch (UART_Buffer1[1])
			{
				case 'E'://********************************���Żظ�$ES���ͳɹ�ORʧ��
					ESOUT();
				break;
				case 'R'://********************************���յ�����$RD
					switch(UART_Buffer1[2])
					{
						case 'D'://************************λ���ٶ�$PGת$PINF���
							RD();
						break;
						default:
						break;
					}
				break;
				case 'P'://********************************λ���ٶ�$PG����λ״̬$PS
					switch(UART_Buffer1[2])
					{
						case 'G'://************************λ���ٶ�$PGת$PINF���
							if (UART_Buffer1[7]=='T')
								break;
							else
								PG();
						break;
						case 'S'://************************��λ״̬$PS
							PSOUT();
						break;
						default:
						break;
					}
				break;
				case 'S'://λ����Ϣ$PINF����λ��ѯ���$PINQ
					switch(UART_Buffer1[2])
					{
						case 'I'://************************λ����Ϣ	$PINF
						break;
						case 'S'://************************��λ��ѯ���$PINQ
						break;
						default:
						break;
					}
 				break;
				case 'I'://********************************��������ͨ�����$LTCO
					ICOUT();
				break;
				case 'N'://********************************���չ̶��û����ܻ�ִȷ�����$NAOP
				break;
				case 'Q'://********************************��ִ��ѯ���$QOUT
				break;
				default:
				break;
			}
 
 			Flag1=0;
			count1=0;
			UART_Buffer_Size1=0;
		}
/**********************************************�����жϴ������****************************************/
	}
}


/***********************���ڳ�ʼ��,����1������9600������2������9600*********************************/
/*
void UartInit(void)
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;			//�趨��ʱ��ֵ
	TH1 = 0xDC;			//�趨��ʱ����װֵ
	ET1 = 0;			//��ֹ��ʱ��1�ж�
	TR1 = 1;			//������ʱ��1
	EA=1;				//���ж�
	ES=1;				//�˿�1�ж�����

	AUXR &= 0xF7;		//�����ʲ�����
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	BRT = 0xDC;			//�趨���������ʷ�������װֵ
	AUXR |= 0x10;		//�������������ʷ�����
	IE2=0x03;			//�˿�2�ж�����
}
*/
/*����1��2��������ʵ����stm32�����һ���շ��˿ڵĹ��ܣ�1��ӦRX�����գ�2��ӦTXͨ���������ⷢ*/
/********************************************����1��������**********************************************/
//stm32������ֱ�����ض����printf��ʵ�ִ���1��2�ķ��͡�
//����ı���message���ֵ����ͨ������1����OutChar0������2����OutChar1
void OutChar0(unsigned char message) //���������ԣ�˵������1��������stm32�е���TX�˿ڹ���
{
	SBUF=message;
	while(!TI);
	TI=0;
}

/************************************************����2��������****************************************/
//����1��2����������ʲô��ͬ��
void OutChar1(unsigned char message) //���͸����İ壬ת��stm32��ʹ���ض����printf�����ͣ��൱��stm32�ķ��Ͷ˿�RX�˿�
{
	S2BUF=message;//S2BUFΪ��C51оƬ�Ĵ���2�Ļ��壬Serial2 BufferΪ���⹦�ܼĴ���
   	while(!(S2CON&S2TI)); //�ȴ����ͽ���
   	S2CON&=~S2TI;
}

void UART0_receive(void) interrupt 4
{
	unsigned char temp0;
	if(RI==1)
	{
		ES=0;
		do
		{	
			while(RI==0);
			RI=0;
			temp0=SBUF;
			switch(temp0)
			{
				case '$':
					count0++;
					UART_Buffer_Size0=0;
					UART_Buffer0[UART_Buffer_Size0]=temp0;
				break;
				default:
					count0++;
					UART_Buffer_Size0++;
					UART_Buffer0[UART_Buffer_Size0]=temp0;
				break;
			}
		}
		while(temp0!=0x0A);
		ES=1;
		Flag0=1;
	}
}

void UART1_receive(void) interrupt 8
{
	unsigned temp1;
	if(S2CON&S2RI)
	{
		IE2=0x02;			//�˿�2�жϽ�ֹ
		do
		{
			while ((S2CON&S2RI)==0);
			S2CON&=~S2RI;
			temp1=S2BUF;
			UART_Buffer1[UART_Buffer_Size1]=temp1;
			UART_Buffer_Size1++;
			count1++;
		}
		while(temp1!=0x0A);
		IE2=0x03;
		Flag1=1;
	}
}

void Delay(int iii) //delay 1*i ms��ʱ����(1.5s,i=1500)
{
	int j,k;
	for (j=0;j<iii;j++)
	{
		for (k=0;k<125;k++);
	}
}