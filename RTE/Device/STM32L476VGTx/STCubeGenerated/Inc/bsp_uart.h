/********************************************************************************
  * file name		:	BSP_UART.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_UART_H
#define _BSP_UART_H


#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	 

#include "bsp_led.h"
	 
/**************************************MACRO*************************************/	 
	 
///* Definition for USARTx clock resources */
//#define USARTx                           USART1
//#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()

//#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
//#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

//#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
//#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()
//	 
//#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()	 

///* Definition for USARTx Pins */
//#define USARTx_TX_PIN                    GPIO_PIN_6
//#define USARTx_TX_GPIO_PORT              GPIOB
//#define USARTx_TX_AF                     GPIO_AF7_USART1

//#define USARTx_RX_PIN                    GPIO_PIN_7
//#define USARTx_RX_GPIO_PORT              GPIOB
//#define USARTx_RX_AF                     GPIO_AF7_USART1

///* Definition for USARTx's DMA */
//#define USARTx_TX_DMA_CHANNEL            DMA1_Channel4
//#define USARTx_RX_DMA_CHANNEL            DMA1_Channel5

///* Definition for USARTx's DMA Request */
//#define USARTx_TX_DMA_REQUEST            DMA_REQUEST_2
//#define USARTx_RX_DMA_REQUEST            DMA_REQUEST_2

///* Definition for USARTx's NVIC */
//#define USARTx_DMA_TX_IRQn               DMA1_Channel4_IRQn
//#define USARTx_DMA_RX_IRQn               DMA1_Channel5_IRQn
//#define USARTx_DMA_TX_IRQHandler         DMA1_Channel4_IRQHandler
//#define USARTx_DMA_RX_IRQHandler         DMA1_Channel5_IRQHandler

///* Definition for USARTx's NVIC */
//#define USARTx_IRQn                      USART1_IRQn
//#define USARTx_IRQHandler                USART1_IRQHandler	 



/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()

#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL            DMA1_Channel7
#define USARTx_RX_DMA_CHANNEL            DMA1_Channel6

/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST            DMA_REQUEST_2
#define USARTx_RX_DMA_REQUEST            DMA_REQUEST_2

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn               DMA1_Channel7_IRQn
#define USARTx_DMA_RX_IRQn               DMA1_Channel6_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA1_Channel7_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA1_Channel6_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler



	
/*************************************FUNCTION***********************************/	

extern void Error_Handler (void);

extern uint8_t aRxBuffer[];


void BSP_Uart_Init(void);

void HAL_UART_MspInit(UART_HandleTypeDef *huart);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle);







#ifdef __cplusplus
}
#endif

#endif /* _BSP_UART_H */

/***********************************END OF FILE**********************************/

