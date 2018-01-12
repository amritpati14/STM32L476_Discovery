/********************************************************************************
  * file name		:	bsp_common.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_COMMON_H
#define _BSP_COMMON_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	

#include "bsp_led.h"

/*************************************REGISTER***********************************/	


	 
	 
/**************************************DEFINE************************************/
	

//#define WRITE_READ_ADDR     ((uint32_t)0x0000)

//#define FLASH_SECTOR ((uint8_t)0x03)
//#define FLASH_SECTOR_SIZE   0x10000	 


//	 
//#define Probe_Data_Size	                      2400	 

	 
#define BUFFER_SIZE         ((uint32_t)0x100)	 
/*************************************FUNCTION***********************************/		 

//extern UART_HandleTypeDef T_UartHandle;
//extern TIM_HandleTypeDef TIM3_Handler;
//extern TIM_HandleTypeDef TIM2_Handler;

extern uint8_t aTxBuffer[BUFFER_SIZE];
extern uint8_t aRxBuffer[BUFFER_SIZE];

//extern uint32_t ProbeData[Probe_Data_Size];	

extern void Error_Handler(void);


//extern TIM_HandleTypeDef TIM7_Handler;
//extern DMA_HandleTypeDef DMA_TIM_IC;


//extern uint8_t const Bmp_Log[];
//extern uint8_t OLED_Display_ECG_Data[] ;

#ifdef __cplusplus
}
#endif

#endif /* _BSP_COMMON_H */




/***********************************END OF FILE**********************************/

