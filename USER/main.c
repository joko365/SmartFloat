/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   对GPS模块传输的数据进行解码，获取定位信息。
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./Bsp/led/bsp_led.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./gps/gps_config.h"

void Delay(__IO u32 nCount); 


// WIFI和SD卡公用SDIO总线，当使用SD卡时，避免WIFI的冲突
// 需要把WIFI的使能引脚PDN拉低。
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
  LED_BLUE;
  
	/*串口初始化*/
	Debug_USART_Config();
  
  GPS_Config();

	// 失能WIFI
	WIFI_PDNPIN_SET();
	
	
		printf("\r\n秉火WF-NEO-6M GPS模块测试例程\r\n"); 
  
	
	/*   测试SIM800模块 */

				printf("AT\r\n");//先发AT，让GSM模块自动获取波特率为9600
				Delay(10);
				printf("ATD17863860111;\r\n");//电话号码修改为自己需要拨打的电话
				Delay(100);
				//printf("ATH\r\n");//ATH挂断电话
				//Delay(1000);
				//while(1)
				//{
				// 
				//}
				
  /* GPS解码测试 */
  nmea_decode_test();
  
	
	
  while(1);


}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

