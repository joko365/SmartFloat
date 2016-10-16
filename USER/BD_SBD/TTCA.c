#include "HXHD_SZTH.H"
void TTCA(void)
{
	TTCA_1=6,k=0,iiii=0;
	SendDat0[0]='$';//报头
	SendDat0[1]='S';
	SendDat0[2]='D';
	SendDat0[3]=',';

	for(i=0;i<=200&&UART_Buffer0[i]!=0x0A;i++)
	{
		if(UART_Buffer0[i]==',')
			k++;
		switch(k)
		{
			case 0:
			break;
			case 1://收信方ID，不处理
			break;
			case 2://收信方地址，从“,”开始，末位接传输方式
				SendDat0[TTCA_1]=UART_Buffer0[i];
				SendDat0[TTCA_1+1]=',';
				SendDat0[TTCA_1+2]='1';
				SendDat0[TTCA_1+3]=',';
				TTCA_1++;
				TTCA_2=TTCA_1+5;//跳到正文位置
			break;
			case 3://保密要求，正文第一字节
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//13->18
			break;
			case 4://回执标志，正文第二个字节
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//19->20
			break;
			case 5://电文长度，后处理
			break;
			case 6://电文内容
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//19->20
			break;
		}
	}
	SendDat0[TTCA_2]=',';
	SendDat0[4]=((TTCA_2-TTCA_1+8)/10)+0x30;//总长度十位
	SendDat0[5]=((TTCA_2-TTCA_1+8)%10)+0x30;//总长度个位

	SendDat0[16]=((TTCA_2-TTCA_1-6)/10)+0x30;//字报长度十位
	SendDat0[17]=((TTCA_2-TTCA_1-6)%10)+0x30;//报文长度个位

	for(iiii=1;iiii<=TTCA_2;iiii++)
	{
		CRCtemp[iiii-1]=SendDat0[iiii];
	}
	CRCVALUE=BDCRC(CRCtemp,TTCA_2,C);
	
	SendDat0[TTCA_2+1]=C[0];	//校验值高四位
	SendDat0[TTCA_2+2]=C[1];	//校验值低四位
	SendDat0[TTCA_2+3]=0x0D;	//结束位
	SendDat0[TTCA_2+4]=0x0A;  //结束位

	for(i=0;i<=TTCA_2+4;i++)
		OutChar1(SendDat0[i]);
}