#include "HXHD_SZTH.H"
void ICOUT(void)
{
	IC_1=1,k=0,iiii=0;
	SendDat1[0]='$';//��ͷ
 
	for(i=1;i<=200&&UART_Buffer1[i]!=0x0A;i++)
	{
		SendDat1[IC_1]=UART_Buffer1[i];
		IC_1++;
	}
	for(i=0;i<=PS_1;i++)
		OutChar0(SendDat1[i]);
}