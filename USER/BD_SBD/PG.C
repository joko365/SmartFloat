#include "HXHD_SZTH.H"
void PG(void)
{
	PG_1=6,k=0,iiii=0;
	SendDat1[0]='$';//��ͷ
	SendDat0[1]='P';
	SendDat0[2]='I';
	SendDat0[3]='N';
	SendDat0[4]='F';
	SendDat0[5]=',';

	for(i=0;i<=200&&UART_Buffer1[i]!=0x0A;i++)
	{
		if(UART_Buffer0[i]==',')
			k++;
		switch(k)
		{
			case 0:
			break;
			case 1://�����ݳ��ȣ�������
			break;
			case 2://λ����Ч��Ϣ��������
			break;
			case 3://γ��
				SendDat1[PG_1]=UART_Buffer1[i];
				PG_1++;
			break;
			case 4://�ϱ�γ��־��������
			break;
			case 5://����
 				SendDat1[PG_1]=UART_Buffer1[i];
				PG_1++;
			break;
			case 6://��������־��������
			break;
			case 7://�߶�
 				SendDat1[PG_1]=UART_Buffer1[i];
				PG_1++;
			break;
			default:
			break;
		}
	}
 	SendDat1[PG_1]=',';
 
	SendDat1[4]=((TTCA_2-TTCA_1+8)/10)+0x30;//�ܳ���ʮλ
	SendDat1[5]=((TTCA_2-TTCA_1+8)%10)+0x30;//�ܳ��ȸ�λ

	SendDat1[16]=((TTCA_2-TTCA_1-6)/10)+0x30;//�ֱ�����ʮλ
	SendDat1[17]=((TTCA_2-TTCA_1-6)%10)+0x30;//���ĳ��ȸ�λ

	for(iiii=1;iiii<=TTCA_2;iiii++)
	{
		CRCtemp[iiii-1]=SendDat1[iiii];
	}
	CRCVALUE=BDCRC(CRCtemp,TTCA_2,C);

	SendDat1[TTCA_2+1]=C[0];	//У��ֵ����λ
	SendDat1[TTCA_2+2]=C[1];	//У��ֵ����λ
	SendDat1[TTCA_2+3]=0x0D;	//����λ
	SendDat1[TTCA_2+4]=0x0A;    //����λ
	for(i=0;i<=TTCA_2+4;i++)
		OutChar0(SendDat1[i]);
}