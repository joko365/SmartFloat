//#include <STC12C5A60S2.H>
//#include <stdio.h>
//#include <string.h>
//#include <intrins.h>
#include "HXHD_SZTH.h"

//#define S2RI 0x01	//串口2接收中断请求标志位
//#define S2TI 0x02	//串口2发送中断请求标志位

#define CONVERT_HEX_CHAR(c, h, l) l = 0x0F&c;h=0x0F&(c>>4);l =(l<=9)?('0'+l):('A'+l-10);h=(h<=9)?('0'+h):('A'+h-10)//16进制转换

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

//*************************************CRC校验***********************************//
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
//sbit power=P1^7;//P1口：P1口是一个内部提供上拉电阻的8位双向I/O口，P1口缓冲器能接收输出4TTL门电流。
								//P1口管脚写入“1”后，电位被内部上拉为高，可用作输入，P1口被外部下拉为低电平时，将输出电流，这是由于内部上拉的缘故。
								//在FLASH编程和校验时，P1口作为第八位地址接收;
//sbit mm=P2^0;//P2口：P2口为一个内部上拉电阻的8位双向I/O口，P2口缓冲器可接收，输出4个TTL门电流，当P2口被写“1”时，其管脚电位被内部上拉电阻拉高，且作为输入。
						 //作为输入时，P2口的管脚电位被外部拉低，将输出电流，这是由于内部上拉的缘故。
						 //P2口当用于外部程序存储器或16位地址外部数据存储器进行存取时，P2口输出地址的高八位。
						 //在给出地址“1”时，它利用内部上拉的优势，当对外部八位地址数据存储器进行读写时，P2口输出其特殊功能寄存器的内容。
						 //P2口在FLASH编程和校验时接收高八位地址信号和控制信号;

void HXHD_SZTH(void)
{
//	int timetep0=0,timetep1=0;
	char BDINIT[15]={"$DM,03,ALL,67\r\n"};//北斗初始化
	i=0,ii=0,iii=0,iiii=0;
	UART_Buffer_Size0=0,UART_Buffer_Size1=0;
	count0=0,count1=0;
	Flag0=0,Flag1=0;
	//power =1;//对应上面的sbit power=P1^7;
	Delay(200);
	//power=0;
	//UartInit();//串口初始化在这里不需要

	for(i=0;i<19600;i++)
		{	
			Delay(65535);
			Delay(65535);
			Delay(65535);
//			OutChar0(i+30);
//			OutChar0('\r');
//			OutChar0('\n');
		//mm=i%2;//对应sbit mm=P2^0;
		}
	for(i=0;i<15;i++)
		OutChar1(BDINIT[i]);

	while(1)
	{
/*******************************************串口1接收中断处理*****************************************/
//		OutChar0('\n');
//		Delay(100);
		if(Flag0==1)
		{
			Flag0=0;
			k=0;
			switch(UART_Buffer0[1])
			{
				case 'Q': //查询终端状态$QSTA，查询申请$QAPP
					switch(UART_Buffer0[2])
					{
						case 'S'://************************查询终端状态$QSTA
						break;
						case 'A'://************************查询申请$QAPP
						break;
					}					
				break;
				case 'S'://********************************设置终端状态$STST
					_nop_();
				break;
				case 'I'://********************************查询报文顺序号$IQMS
				break;
				case 'P'://定位申请$PAPP；结束定位PEND
					switch(UART_Buffer0[2])
					{
						case 'A'://************************定位申请$PAPP
						break;
						case 'E'://************************结束定位PEND
						break;
					}
				break;
				case 'D'://********************************遇险通信申请$DCAP
				break;
				case 'T'://终端到终端通信申请$TTCA；终端到固定通信申请$TNCA；终端查询网管最新报文$TINC；授时申请$TAPP
					switch(UART_Buffer0[2])
					{
//*********************************************************终端到终端通信申请$TTCA*********************************************************//
						case 'T':
							TTCA();
   						break;
//*********************************************************终端到终端通信申请$TTCA*********************************************************//
						case 'N'://************************终端到固定通信申请$TNCA
						break;
						case 'I'://************************终端查询网管最新报文$TINC
						break;
						case 'A'://************************授时申请$TAPP
						break;
					}
					break;
				case 'C'://通信回执申请$CACA；通信输出成功状态$COSS
					switch(UART_Buffer0[2])
					{
						case 'A'://************************通信回执申请$CACA
						break;
						case 'O'://************************通信输出成功状态$COSS
						break;
					}
				break;
				case 'L'://********************************超长报文通信申请$LTCA
				break;
				case 'G':
					switch(UART_Buffer0[4])
					{
						case 'A'://************************GPS定位申请$GPSA
						break;
						case 'E'://************************结束GPS定位$GPSE
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
/**********************************************串口2中断处理开始****************************************/
		if(Flag1==1)
		{
			Flag1=0;
			switch (UART_Buffer1[1])
			{
				case 'E'://********************************短信回复$ES发送成功OR失败
					ESOUT();
				break;
				case 'R'://********************************接收到数据$RD
					switch(UART_Buffer1[2])
					{
						case 'D'://************************位置速度$PG转$PINF输出
							RD();
						break;
						default:
						break;
					}
				break;
				case 'P'://********************************位置速度$PG，定位状态$PS
					switch(UART_Buffer1[2])
					{
						case 'G'://************************位置速度$PG转$PINF输出
							if (UART_Buffer1[7]=='T')
								break;
							else
								PG();
						break;
						case 'S'://************************定位状态$PS
							PSOUT();
						break;
						default:
						break;
					}
				break;
				case 'S'://位置信息$PINF；定位查询结果$PINQ
					switch(UART_Buffer1[2])
					{
						case 'I'://************************位置信息	$PINF
						break;
						case 'S'://************************定位查询结果$PINQ
						break;
						default:
						break;
					}
 				break;
				case 'I'://********************************超长报文通信输出$LTCO
					ICOUT();
				break;
				case 'N'://********************************最终固定用户接受回执确认输出$NAOP
				break;
				case 'Q'://********************************回执查询结果$QOUT
				break;
				default:
				break;
			}
 
 			Flag1=0;
			count1=0;
			UART_Buffer_Size1=0;
		}
/**********************************************串口中断处理结束****************************************/
	}
}


/***********************串口初始化,串口1波特率9600，串口2波特率9600*********************************/
/*
void UartInit(void)
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xDC;			//设定定时初值
	TH1 = 0xDC;			//设定定时器重装值
	ET1 = 0;			//禁止定时器1中断
	TR1 = 1;			//启动定时器1
	EA=1;				//开中断
	ES=1;				//端口1中断允许

	AUXR &= 0xF7;		//波特率不倍速
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xDC;			//设定独立波特率发生器重装值
	AUXR |= 0x10;		//启动独立波特率发生器
	IE2=0x03;			//端口2中断允许
}
*/
/*串口1和2两个串口实现了stm32里面的一个收发端口的功能，1对应RX电脑收；2对应TX通过串口向外发*/
/********************************************串口1发送数据**********************************************/
//stm32里这里直接用重定向的printf来实现串口1和2的发送。
//这里的变量message获得值，是通过串口1函数OutChar0，串口2函数OutChar1
void OutChar0(unsigned char message) //反馈给电脑，说明串口1代表了是stm32中的收TX端口功能
{
	SBUF=message;
	while(!TI);
	TI=0;
}

/************************************************串口2发送数据****************************************/
//串口1和2发送数据有什么不同？
void OutChar1(unsigned char message) //发送给报文板，转到stm32后，使用重定向的printf来发送，相当于stm32的发送端口RX端口
{
	S2BUF=message;//S2BUF为该C51芯片的串口2的缓冲，Serial2 Buffer为特殊功能寄存器
   	while(!(S2CON&S2TI)); //等待发送结束
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
		IE2=0x02;			//端口2中断禁止
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

void Delay(int iii) //delay 1*i ms延时程序(1.5s,i=1500)
{
	int j,k;
	for (j=0;j<iii;j++)
	{
		for (k=0;k<125;k++);
	}
}