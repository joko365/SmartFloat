#include "HXHD_SZTH.H"
void RD(void)
{
	RD_1=11,k=0,iiii=0;
	SendDat1[0]='$';//��ͷ
	SendDat1[1]='C';
	SendDat1[2]='O';
	SendDat1[3]='U';
	SendDat1[4]='T';
	SendDat1[5]=',';
	SendDat1[6]='0';//CRCУ���־��0��ʾУ����ȷ��
	SendDat1[7]=',';
	SendDat1[8]='1';//ͨ�����
	SendDat1[9]=',';
	SendDat1[10]='0';//���ŷ�����
 
	for(i=0;i<=200&&UART_Buffer1[i]!=0x0A;i++)
	{
		if(UART_Buffer1[i]==',')
			k++;
		switch(k)
		{
			case 0:
			break;
			case 1://�����ܳ��ȣ�������
			break;
			case 2://���ŷ���ַ����11λ�ԡ�,����ʼ��ĩλ����0����ʾ����Ҫ��ִ
				SendDat1[RD_1]=UART_Buffer1[i];
				SendDat1[RD_1+1]=',';
				SendDat1[RD_1+2]='0';//��ִ��־��0��ʾ����Ҫ��ִ
				SendDat1[RD_1+3]=',';
				SendDat1[RD_1+4]='0';//����˳��ţ���0��β��
				RD_1++;
				RD_2=RD_1+4;
			break;
			case 3://���䷽ʽ��������
			break;
			case 4://�������ݳ���
				SendDat1[RD_2]=UART_Buffer1[i];
				RD_2++;
			break;
			case 5://��������
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

	SendDat1[RD_2+1]=C[0];	//У��ֵ����λ
	SendDat1[RD_2+2]=C[1];	//У��ֵ����λ
	SendDat1[RD_2+3]=0x0D;	//����λ
	SendDat1[RD_2+4]=0x0A;  //����λ

	for(i=0;i<=RD_2+4;i++)
		OutChar0(SendDat1[i]);
}