/********************************************************************************
  * file name		:	bsp_uart.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/

#include "bsp_uart.h"

UART_HandleTypeDef T_UartHandle;

static void GPIO_Uart_Init(void);
static void NVIC_Uart_Init(void);


void BSP_Uart_Init(void)
{	
	/* Enable USARTx clock */
	USARTx_CLK_ENABLE();	
	
	/* USART Format configuration */ 	
	T_UartHandle.Instance        = USARTx;
	
	T_UartHandle.Init.BaudRate   = 115200;
	T_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	T_UartHandle.Init.StopBits   = UART_STOPBITS_1;
	T_UartHandle.Init.Parity     = UART_PARITY_NONE;
	T_UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	T_UartHandle.Init.Mode       = UART_MODE_TX_RX; 
	
	if(HAL_UART_Init(&T_UartHandle) != HAL_OK)
	{
		Error_Handler();
	}	
	
	NVIC_Uart_Init();	
	
}

/**
  * @brief UART MSP Initialization 
  *          - Enable the Peripheral's clock
  *          - Initialize GPIO,DMA,NVIC
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USARTx)
	{
		GPIO_Uart_Init();
					
	}		
	
}


/**
  * @brief  UART GPIO Configuration
  * @param  None
  * @note   
  *         
  * @retval None
  */
static void GPIO_Uart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	/* Enable GPIO TX/RX clock */	
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();	
	
	/* UART  TX&RX GPIO pin configuration  */
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;	
	GPIO_InitStruct.Alternate = USARTx_TX_AF;	
	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;
	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}


static void NVIC_Uart_Init(void)
{	
	/* NVIC for USART */
	HAL_NVIC_SetPriority(USARTx_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USARTx_IRQn);
}



/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	USARTx_FORCE_RESET();
	USARTx_RELEASE_RESET();

	HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);

	HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

	HAL_NVIC_DisableIRQ(USARTx_IRQn);		
}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set transmission flag: trasfer complete*/
	
//	HAL_UART_Transmit(&T_UartHandle,aRxBuffer,1,0); 
//	HAL_UART_Receive_IT(&T_UartHandle,aRxBuffer,1); 
	
}


/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Set transmission flag: trasfer complete*/

}


/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();
}









/***********************************END OF FILE**********************************/


