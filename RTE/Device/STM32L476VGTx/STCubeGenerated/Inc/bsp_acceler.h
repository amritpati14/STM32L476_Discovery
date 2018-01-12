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

#define ACCEL_CTRL_REG4_A            0x23
	 
	 

#define MAGNE_WHO_AM_I_ADDR          0x0F  /* device identification register */

#define MAGNE_CTRL_REG3_M            0x22
	 
/**************************************MACRO*************************************/	

/* SPI SCLK,MOSI,MISO GPIO CONFIG */
#define ACCEL_SPIx                               SPI2
	 
#define ACCEL_SPIx_GPIO_PORT                     GPIOD                      /* GPIOD */
#define ACCEL_SPIx_SCK_PIN                       GPIO_PIN_1                 /* PD.01*/
#define ACCEL_SPIx_MISO_PIN                      GPIO_PIN_3                 /* PD.03 */
#define ACCEL_SPIx_MOSI_PIN                      GPIO_PIN_4                 /* PD.04 */
#define ACCEL_SPIx_AF                            GPIO_AF5_SPI2
#define ACCEL_SPIx_CLOCK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define ACCEL_SPIx_CLOCK_DISABLE()               __HAL_RCC_SPI2_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define ACCEL_SPIx_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_FORCE_RESET()            __HAL_RCC_SPI2_FORCE_RESET()
#define ACCEL_SPIx_GPIO_RELEASE_RESET()          __HAL_RCC_SPI2_RELEASE_RESET()

/* ACCELER CS */
#define ACCEL_CS_GPIO_PORT                       GPIOE                       /* GPIOE */
#define ACCEL_CS_PIN                             GPIO_PIN_0                  /* PE.00 */	 
#define ACCEL_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCEL_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOE_CLK_DISABLE()

#define ACCEL_CS_LOW()                           HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_RESET)
#define ACCEL_CS_HIGH()                          HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_SET)	

	 
/* MAGNETIC CS */
#define MAGNE_CS_GPIO_PORT                       GPIOC                       /* GPIOC */
#define MAGNE_CS_PIN                             GPIO_PIN_0                  /* PC.00 */	 
#define MAGNE_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define MAGNE_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOC_CLK_DISABLE()

#define MAGNE_CS_LOW()                           HAL_GPIO_WritePin(MAGNE_CS_GPIO_PORT, MAGNE_CS_PIN, GPIO_PIN_RESET)
#define MAGNE_CS_HIGH()                          HAL_GPIO_WritePin(MAGNE_CS_GPIO_PORT, MAGNE_CS_PIN, GPIO_PIN_SET)		 
 



#define __SPI_DIRECTION_1LINE_TX(__HANDLE__) do{\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             }while(0);

#define __SPI_DIRECTION_1LINE_RX(__HANDLE__) do {\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE);\
                                             } while(0);



/*************************************FUNCTION***********************************/	

extern void Error_Handler (void);	

void BSP_ACCELER_Init(void);
											 
uint8_t ACCELERO_IO_Read(uint8_t RegisterAddr);
											 
void ACCELERO_IO_Write(uint8_t RegisterAddr, uint8_t Value);											 

uint8_t LSM303C_ACCELERO_ReadID(void); 
											 
											 
uint8_t MAGNETIC_IO_Read(uint8_t RegisterAddr);

void MAGNETIC_IO_Write(uint8_t RegisterAddr, uint8_t Value);											 
											 
uint8_t LSM303C_MAGNETIC_ReadID(void);




#ifdef __cplusplus
}
#endif

#endif /* _BSP_ACCELER_H */




/***********************************END OF FILE**********************************/

