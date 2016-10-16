#include "HXHD_SZTH.H"
void ESOUT(void)
{
	ES_1=1,k=0,iiii=0;
	SendDat1[0]='$';//±¨Í·
 
	for(i=1;i<=200&&UART_Buffer1[i]!=0x0A;i++)
	{
		SendDat1[ES_1]=UART_Buffer1[i];
		ES_1++;
	}
	for(i=0;i<=ES_1;i++)
		OutChar0(SendDat1[i]);
}