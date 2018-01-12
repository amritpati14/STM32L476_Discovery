/********************************************************************************
  * file name		:	bsp_led.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/

#include "bsp_led.h"


void BSP_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
		
	LED_RED_GPIO_CLK_ENABLE();
	LED_GREEN_GPIO_CLK_ENABLE();
		
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
	GPIO_InitStruct.Pin = LED_RED_PIN;		
	HAL_GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStruct);
		
	GPIO_InitStruct.Pin = LED_GREEN_PIN;
	HAL_GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStruct);		
	
}








/***********************************END OF FILE**********************************/


