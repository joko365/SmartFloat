#include "HXHD_SZTH.H"
void TTCA(void)
{
	TTCA_1=6,k=0,iiii=0;
	SendDat0[0]='$';//��ͷ
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
			case 1://���ŷ�ID��������
			break;
			case 2://���ŷ���ַ���ӡ�,����ʼ��ĩλ�Ӵ��䷽ʽ
				SendDat0[TTCA_1]=UART_Buffer0[i];
				SendDat0[TTCA_1+1]=',';
				SendDat0[TTCA_1+2]='1';
				SendDat0[TTCA_1+3]=',';
				TTCA_1++;
				TTCA_2=TTCA_1+5;//��������λ��
			break;
			case 3://����Ҫ�����ĵ�һ�ֽ�
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//13->18
			break;
			case 4://��ִ��־�����ĵڶ����ֽ�
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//19->20
			break;
			case 5://���ĳ��ȣ�����
			break;
			case 6://��������
				SendDat0[TTCA_2]=UART_Buffer0[i];
				TTCA_2++;//19->20
			break;
		}
	}
	SendDat0[TTCA_2]=',';
	SendDat0[4]=((TTCA_2-TTCA_1+8)/10)+0x30;//�ܳ���ʮλ
	SendDat0[5]=((TTCA_2-TTCA_1+8)%10)+0x30;//�ܳ��ȸ�λ

	SendDat0[16]=((TTCA_2-TTCA_1-6)/10)+0x30;//�ֱ�����ʮλ
	SendDat0[17]=((TTCA_2-TTCA_1-6)%10)+0x30;//���ĳ��ȸ�λ

	for(iiii=1;iiii<=TTCA_2;iiii++)
	{
		CRCtemp[iiii-1]=SendDat0[iiii];
	}
	CRCVALUE=BDCRC(CRCtemp,TTCA_2,C);
	
	SendDat0[TTCA_2+1]=C[0];	//У��ֵ����λ
	SendDat0[TTCA_2+2]=C[1];	//У��ֵ����λ
	SendDat0[TTCA_2+3]=0x0D;	//����λ
	SendDat0[TTCA_2+4]=0x0A;  //����λ

	for(i=0;i<=TTCA_2+4;i++)
		OutChar1(SendDat0[i]);
}