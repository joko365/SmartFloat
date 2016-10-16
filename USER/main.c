/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ��GPSģ�鴫������ݽ��н��룬��ȡ��λ��Ϣ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./gps/gps_config.h"

void Delay(__IO u32 nCount); 


// WIFI��SD������SDIO���ߣ���ʹ��SD��ʱ������WIFI�ĳ�ͻ
// ��Ҫ��WIFI��ʹ������PDN���͡�
void WIFI_PDNPIN_SET(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE);															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
		GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
} 

extern void nmea_decode_test(void);
extern void HXHD_SZTH(void);


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
  
  LED_BLUE;
  
	/*���ڳ�ʼ��*/
	Debug_USART_Config();
  
  GPS_Config();

	// ʧ��WIFI
	WIFI_PDNPIN_SET();
	
	
		printf("\r\n����WF-NEO-6M GPSģ���������\r\n"); 
  
	
	/*   ����SIM800ģ�� */

				printf("AT\r\n");//�ȷ�AT����GSMģ���Զ���ȡ������Ϊ9600
				Delay(10);
				printf("ATD17863860111;\r\n");//�绰�����޸�Ϊ�Լ���Ҫ����ĵ绰
				Delay(100);
				//printf("ATH\r\n");//ATH�Ҷϵ绰
				//Delay(1000);
				//while(1)
				//{
				// 
				//}
				
  /* GPS������� */
  nmea_decode_test();
  
	
	
  while(1);


}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

