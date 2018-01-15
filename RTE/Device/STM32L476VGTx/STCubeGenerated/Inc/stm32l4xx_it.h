/********************************************************************************
  * file name		:	stm32l4xx_it.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef __STM32L4xx_IT_H
#define __STM32L4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/*************************************INCLUDE************************************/
	 
#include "stm32l4xx_hal.h"

/*************************************FUNCTION***********************************/	
	
extern TIM_HandleTypeDef TIM3_Handler;
	 
extern UART_HandleTypeDef T_UartHandle;	 

extern DMA_HandleTypeDef DMA_TIM_IC;	 

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
	 

void USART1_IRQHandler(void);
void TIM3_IRQHandler(void);

void DMA1_Channel6_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
