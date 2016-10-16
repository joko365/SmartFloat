#include "HXHD_SZTH.H"
void RD(void)
{
	RD_1=11,k=0,iiii=0;
	SendDat1[0]='$';//报头
	SendDat1[1]='C';
	SendDat1[2]='O';
	SendDat1[3]='U';
	SendDat1[4]='T';
	SendDat1[5]=',';
	SendDat1[6]='0';//CRC校验标志，0表示校验正确，
	SendDat1[7]=',';
	SendDat1[8]='1';//通信类别
	SendDat1[9]=',';
	SendDat1[10]='0';//发信方类型
 
	for(i=0;i<=200&&UART_Buffer1[i]!=0x0A;i++)
	{
		if(UART_Buffer1[i]==',')
			k++;
		switch(k)
		{
			case 0:
			break;
			case 1://报文总长度，不处理
			break;
			case 2://发信方地址，第11位以‘,’开始，末位“，0”表示不需要回执
				SendDat1[RD_1]=UART_Buffer1[i];
				SendDat1[RD_1+1]=',';
				SendDat1[RD_1+2]='0';//回执标志，0表示不需要回执
				SendDat1[RD_1+3]=',';
				SendDat1[RD_1+4]='0';//报文顺序号，以0结尾；
				RD_1++;
				RD_2=RD_1+4;
			break;
			case 3://传输方式，不处理
			break;
			case 4://报文数据长度
				SendDat1[RD_2]=UART_Buffer1[i];
				RD_2++;
			break;
			case 5://报文正文
				SendDat1[RD_2]=UART_Buffer1[i];
				RD_2++;
			break;
			default:
			break;
		}
	}
	SendDat1[RD_2]=',';

	for(iiii=0;iiii<=RD_2;iiii++)
	{
		CRCtemp[iiii]=SendDat1[iiii];
	}
	CRCVALUE=BDCRC(CRCtemp,RD_2+1,C);

	SendDat1[RD_2+1]=C[0];	//校验值高四位
	SendDat1[RD_2+2]=C[1];	//校验值低四位
	SendDat1[RD_2+3]=0x0D;	//结束位
	SendDat1[RD_2+4]=0x0A;  //结束位

	for(i=0;i<=RD_2+4;i++)
		OutChar0(SendDat1[i]);
}