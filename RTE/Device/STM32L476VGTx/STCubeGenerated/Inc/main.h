/********************************************************************************
  * file name		:	main.h
  * author			:
  * date		    :    
  * description	    :         
********************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************Include************************************/
	 
#include "stm32l4xx_hal.h"	  
	 	 
#include "stdio.h"
#include "string.h"	 
	 
#include "arm_math.h"
#include "arm_const_structs.h"	  
	 
#include "bsp_led.h"
#include "bsp_uart.h" 
	 
#include "bsp_flash.h"
#include "bsp_acceler.h"
#include "bsp_gyro.h"	
	 
//#include "bsp_button.h"	  
//#include "bsp_iwdg.h"
//#include "bsp_timer.h"
//#include "bsp_oled.h"
//#include "bsp_fir.h"


/**************************************MACRO*************************************/


#define BUFFER_SIZE					0x100

/*************************************FUNCTION***********************************/		 
	
extern UART_HandleTypeDef T_UartHandle;

void Error_Handler (void);

void SystemClock_Config(void);

int fputc(int ch, FILE *f);

int fgetc(FILE * f) ;





#ifdef __cplusplus
}
#endif

#endif /* _MAIN_H */







/***********************************END OF FILE**********************************/

