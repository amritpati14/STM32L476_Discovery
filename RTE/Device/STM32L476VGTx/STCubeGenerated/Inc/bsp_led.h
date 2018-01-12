/********************************************************************************
  * file name		:	bsp_led.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_LED_H
#define _BSP_LED_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	 	 
	 
/*************************************REGISTER***********************************/	 
	 
	 
/**************************************MACRO*************************************/	 
	 
#define LED_RED_GPIO_PORT                      GPIOB                       /* GPIOB */
#define LED_RED_PIN                            GPIO_PIN_2                  /* PB.02 */	
#define LED_RED_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_RED_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()
  

#define LED_GREEN_GPIO_PORT                    GPIOE                       /* GPIOE */
#define LED_GREEN_PIN                          GPIO_PIN_8                  /* PE.08 */	
#define LED_GREEN_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED_GREEN_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()

/*************************************FUNCTION***********************************/	
	 
#define LED_GREEN_ON()                         HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN,GPIO_PIN_SET)
#define LED_GREEN_OFF()                        HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN,GPIO_PIN_RESET)
#define LED_GREEN_Toggle()                     HAL_GPIO_TogglePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN) 

#define LED_RED_ON()                           HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN,GPIO_PIN_SET)
#define LED_RED_OFF()                          HAL_GPIO_WritePin(LED_RED_GPIO_PORT, LED_RED_PIN,GPIO_PIN_RESET)
#define LED_RED_Toggle()                       HAL_GPIO_TogglePin(LED_RED_GPIO_PORT, LED_RED_PIN) 


void BSP_LED_Init(void);

	 
	 
	 
	 
	 
	 

#ifdef __cplusplus
}
#endif

#endif /* _BSP_LED_H */

/***********************************END OF FILE**********************************/

