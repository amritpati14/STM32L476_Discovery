/********************************************************************************
  * file name		:	main.c
  * author			:
  * date			:    
  * description	    : Main program body        
********************************************************************************/

/*************************************Include***********************************/


#include "main.h"

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "Healson uart transmit data test!";

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE] = {0x00};

uint8_t GYRO_ID,ACCELER_ID,MAGNETIC_ID;

uint8_t Flash_Data[4028];

uint32_t timer_conter = 0x86;

uint32_t ProbeData[Probe_Data_Size] ={0x00};

int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();
	
	/* LED initialization */	
	BSP_LED_Init();
	
	/* UART initialization */	
	BSP_Uart_Init();
	
	/* GYRO initialization */	
	BSP_GYRO_Init();

	/* Flash initialization */	
	BSP_Flash_Init();
	
	/* ACCELER initialization */		
	BSP_ACCELER_Init();
	
	BSP_TIM3_IC_Init();
	
	BSP_TIM4_PWM_Init();

	BSP_TIM5_PWM_Init();
				
	printf("stm32 is start!\n");
	
	GYRO_ID = L3GD20_ReadID();
	printf("GYRO_ID is 0x%x\n",GYRO_ID);           /* 0xD4 */
	
	
	ACCELER_ID = LSM303C_ACCELERO_ReadID();
	printf("ACCELER_ID is 0x%x\n",ACCELER_ID);	    /* 0x41 */	
	

	MAGNETIC_ID = LSM303C_MAGNETIC_ReadID();
	printf("MAGNETIC_ID is 0x%x\n",MAGNETIC_ID);	/* 0x3D */		
	
	HAL_DMA_Start(&DMA_TIM_IC,timer_conter,ProbeData[0],1);
	
//	printf("\nFlash write data before!\n");
//	
//	
//	Flash_QSPI_Read(Flash_Data,0x00,256);
//	
//	for(uint32_t i=0;i<255;i++)
//	{
//		printf("0x%x\t",Flash_Data[i]);	
//		
//		if ((i+1)%15 == 0)
//		{
//			printf("\n");
//		}
//	}
//	
//	
//	for(uint32_t i=0;i<256;i++)
//	{	
//		Flash_Data[i] = (i+1)%15;		
//	}	
//	
//	
//	
//	Flash_QSPI_Write(Flash_Data,0x00,256);
//	


//	Flash_QSPI_Read(Flash_Data,0x00,256);
//	
//	
//	printf("\nFlash write data after!\n");
//	
//	for(uint32_t i=0;i<255;i++)
//	{
//		printf("0x%x\t",Flash_Data[i]);	
//		
//		if ((i+1)%15 == 0)
//		{
//			printf("\n");
//		}
//	}	
	
	while(1)
	{			
		LED_GREEN_Toggle() ;
		/* Insert delay 1000 ms */
		HAL_Delay(1000);			

		LED_RED_Toggle() ;
		/* Insert delay 1000 ms */
		HAL_Delay(1000);	
				
		for(uint32_t i=0;i<Probe_Data_Size;i++)
		{
					
			printf("%d    ",ProbeData[i]);
			
			if((i+1)% 16 == 0)
			{
				printf("\n");
			}
						
		}
		
//		if (timer_conter == 1000) 
//		{
//			timer_conter = 0;
//			LED_GREEN_Toggle();		
//			LED_RED_Toggle();	
//			
//			for(uint32_t i=0;i<Probe_Data_Size;i++)
//			{
//			
//				printf("%d\t",ProbeData[i]);
//				if((i+1)% 15 == 0)
//				{
//					printf("\n");
//				}
//				
//			}
//			
//		}					
		
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};

	/* MSI is enabled after System reset, activate PLL with MSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLP = 7;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}
  
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers  80MHz */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		/* Initialization Error */
		while(1);
	} 
}

/** Error Marking
*/
void Error_Handler (void)
{
	while(1)
	{
		LED_RED_Toggle() ;
		/* Insert delay 1000 ms */
		HAL_Delay(1000);		
	}
}


/** Output Redefine 
*/
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&T_UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

/** Input Redefine 
*/
 int fgetc(FILE * f) 
{ 
	uint8_t ch = 0; 
	
	HAL_UART_Receive(&T_UartHandle,&ch, 1, 0xFFFF); 
	
	return ch; 
} 




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
