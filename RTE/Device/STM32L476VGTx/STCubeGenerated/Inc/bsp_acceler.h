/********************************************************************************
  * file name		:	bsp_acceler.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_ACCELER_H
#define _BSP_ACCELER_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	 

/*************************************REGISTER***********************************/	 
	 
#define ACCEL_WHO_AM_I_ADDR          0x0F  /* device identification register */

#define CTRL_REG4_A  0x23



/*********************************************************************************
                        SPI GPIO CONFIG
*********************************************************************************/
//SCLK,MOSI,MISO
#define ACCEL_SPIx                               SPI2
#define ACCEL_SPIx_GPIO_PORT                     GPIOD                      /* GPIOD */
#define ACCEL_SPIx_SCK_PIN                       GPIO_PIN_1                 /* PD.01*/

#define ACCEL_SPIx_MOSI_PIN                      GPIO_PIN_4                 /* PD.04 */
#define ACCEL_SPIx_AF                            GPIO_AF5_SPI2
#define ACCEL_SPIx_CLOCK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define ACCEL_SPIx_CLOCK_DISABLE()               __HAL_RCC_SPI2_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define ACCEL_SPIx_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_FORCE_RESET()            __HAL_RCC_SPI2_FORCE_RESET()
#define ACCEL_SPIx_GPIO_RELEASE_RESET()          __HAL_RCC_SPI2_RELEASE_RESET()

//CS
#define ACCEL_CS_GPIO_PORT                       GPIOE                       /* GPIOE */
#define ACCEL_CS_PIN                             GPIO_PIN_0                  /* PD.00 */	 
#define ACCEL_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCEL_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOE_CLK_DISABLE()

	 
	 
	 
#define ACCEL_CS_LOW()                           HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_RESET)
#define ACCEL_CS_HIGH()                          HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_SET)	 






/*************************************FUNCTION***********************************/	



void ACCEL_IO_Init(void);

uint8_t LSM303C_ReadID(void); 





#ifdef __cplusplus
}
#endif

#endif /* _BSP_ACCELER_H */




/***********************************END OF FILE**********************************/

