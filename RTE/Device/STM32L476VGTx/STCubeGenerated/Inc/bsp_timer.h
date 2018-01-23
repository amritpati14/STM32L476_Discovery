/********************************************************************************
  * file name		:	bsp_timer.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"		 
 	 	 

/*************************************REGISTER***********************************/	 
	 
	 
	 
/**************************************MACROS************************************/	 
#define Timer_Prescaler	                     (2000 - 1)	 
	 
#define Timer_Period	                     (80-1)	        	 

#define Probe_Data_Size                       4000	 
/*************************************FUNCTION***********************************/	
extern uint32_t timer_conter;

extern uint16_t ProbeData[Probe_Data_Size];

extern void Error_Handler (void);

void DMA_XferHalfCpltCallback(DMA_HandleTypeDef * hdma);
	 
void BSP_TIM3_IC_Init(void);
	
void BSP_TIM4_PWM_Init(void);

void BSP_TIM5_PWM_Init(void);

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim); 

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);


HAL_StatusTypeDef TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t *pData, uint16_t Length);


#ifdef __cplusplus
}
#endif

#endif /* _BSP_TIMER_H */




/***********************************END OF FILE**********************************/

