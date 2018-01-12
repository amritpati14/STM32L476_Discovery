/********************************************************************************
  * file name		:bsp_gyro.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_GYRO_H
#define _BSP_GYRO_H


#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/ 

#include "stm32l4xx_hal.h"	 

/*************************************REGISTER***********************************/
	 
#define L3GD20_WHO_AM_I_ADDR          0x0F  /* device identification register */
#define L3GD20_CTRL_REG1_ADDR         0x20  /* Control register 1 */
#define L3GD20_CTRL_REG2_ADDR         0x21  /* Control register 2 */
#define L3GD20_CTRL_REG3_ADDR         0x22  /* Control register 3 */
#define L3GD20_CTRL_REG4_ADDR         0x23  /* Control register 4 */
#define L3GD20_CTRL_REG5_ADDR         0x24  /* Control register 5 */
#define L3GD20_REFERENCE_REG_ADDR     0x25  /* Reference register */
#define L3GD20_OUT_TEMP_ADDR          0x26  /* Out temp register */
#define L3GD20_STATUS_REG_ADDR        0x27  /* Status register */
#define L3GD20_OUT_X_L_ADDR           0x28  /* Output Register X */
#define L3GD20_OUT_X_H_ADDR           0x29  /* Output Register X */
#define L3GD20_OUT_Y_L_ADDR           0x2A  /* Output Register Y */
#define L3GD20_OUT_Y_H_ADDR           0x2B  /* Output Register Y */
#define L3GD20_OUT_Z_L_ADDR           0x2C  /* Output Register Z */
#define L3GD20_OUT_Z_H_ADDR           0x2D  /* Output Register Z */ 
#define L3GD20_FIFO_CTRL_REG_ADDR     0x2E  /* Fifo control Register */
#define L3GD20_FIFO_SRC_REG_ADDR      0x2F  /* Fifo src Register */

#define L3GD20_INT1_CFG_ADDR          0x30  /* Interrupt 1 configuration Register */
#define L3GD20_INT1_SRC_ADDR          0x31  /* Interrupt 1 source Register */
#define L3GD20_INT1_TSH_XH_ADDR       0x32  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_XL_ADDR       0x33  /* Interrupt 1 Threshold X register */
#define L3GD20_INT1_TSH_YH_ADDR       0x34  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_YL_ADDR       0x35  /* Interrupt 1 Threshold Y register */
#define L3GD20_INT1_TSH_ZH_ADDR       0x36  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_TSH_ZL_ADDR       0x37  /* Interrupt 1 Threshold Z register */
#define L3GD20_INT1_DURATION_ADDR     0x38  /* Interrupt 1 DURATION register */


/**************************************MACRO*************************************/	

/* SPI SCLK,MOSI,MISO GPIO CONFIG */
#define GYRO_SPIx                               SPI2
#define GYRO_SPIx_GPIO_PORT                     GPIOD                      /* GPIOD */
#define GYRO_SPIx_SCK_PIN                       GPIO_PIN_1                 /* PD.01*/
#define GYRO_SPIx_MISO_PIN                      GPIO_PIN_3                 /* PD.03 */
#define GYRO_SPIx_MOSI_PIN                      GPIO_PIN_4                 /* PD.04 */
#define GYRO_SPIx_AF                            GPIO_AF5_SPI2
#define GYRO_SPIx_CLOCK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define GYRO_SPIx_CLOCK_DISABLE()               __HAL_RCC_SPI2_CLK_DISABLE()
#define GYRO_SPIx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define GYRO_SPIx_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()
#define GYRO_SPIx_GPIO_FORCE_RESET()            __HAL_RCC_SPI2_FORCE_RESET()
#define GYRO_SPIx_GPIO_RELEASE_RESET()          __HAL_RCC_SPI2_RELEASE_RESET()

/* CS */
#define GYRO_CS_GPIO_PORT                       GPIOD                       /* GPIOD */
#define GYRO_CS_PIN                             GPIO_PIN_7                  /* PD.07 */	 
#define GYRO_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define GYRO_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOD_CLK_DISABLE()

/* INT1 */
#define GYRO_INT1_GPIO_PORT                     GPIOD                       /* GPIOD */
#define GYRO_INT1_PIN                           GPIO_PIN_2                  /* PD.02 */
#define GYRO_INT1_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define GYRO_INT1_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()
#define GYRO_INT1_EXTI_IRQn                     EXTI2_IRQn

/* INT2 */
#define GYRO_INT2_GPIO_PORT                     GPIOB                       /* GPIOB */
#define GYRO_INT2_PIN                           GPIO_PIN_8                  /* PB.08 */
#define GYRO_INT2_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define GYRO_INT2_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define GYRO_INT2_EXTI_IRQn                     EXTI9_5_IRQn
	 
	 	 
#define GYRO_CS_LOW()                           HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_RESET)
#define GYRO_CS_HIGH()                          HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_SET)	 
	 

/* Read/Write command */
#define READWRITE_CMD                           ((uint8_t)0x80)

/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD                        ((uint8_t)0x40)

																				 
																						 
#define I_AM_L3GD20                 ((uint8_t)0xD4)
									
/*************************************FUNCTION***********************************/	

extern void Error_Handler (void);	


void BSP_GYRO_Init(void);	

void GYRO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

void GYRO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);


uint8_t L3GD20_ReadID(void); 

void L3GD20_FilterCmd(uint8_t HighPassFilterState);

 
	  
		 
	 

#ifdef __cplusplus
}
#endif

#endif /* _BSP_GYRO_H */


/***********************************END OF FILE**********************************/

