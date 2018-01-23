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

uint8_t GYRO_ID,ACCELER_ID,MAGNETIC_ID,AUDIO_ID;

uint8_t Flash_Data[4028];

uint32_t timer_conter = 0x00;

uint16_t ProbeData[Probe_Data_Size] ={0x00};


int32_t                     RecBuff[2048];
int16_t                     PlayBuff[4096];
uint8_t                     DmaRecHalfBuffCplt  = 0;
uint8_t                     DmaRecBuffCplt      = 0;
uint32_t                    PlaybackStarted         = 0;


#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))



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
	
//	/* GYRO initialization */	
//	BSP_GYRO_Init();

//	/* Flash initialization */	
//	BSP_Flash_Init();
//	
//	/* ACCELER initialization */		
//	BSP_ACCELER_Init();

	/* AUDIO initialization */
//	CS43L22_I2C_Init();
	DFSDM_Init();
	Playback_Init();
	
//	/* TIM3_IC initialization */	
//	BSP_TIM3_IC_Init();

//	/* TIM5_PWM initialization */	
//	BSP_TIM5_PWM_Init();
				
	printf("stm32 is start!\n");
	
//	GYRO_ID = L3GD20_ReadID();
//	printf("GYRO_ID is 0x%x\n",GYRO_ID);           /* 0xD4 */
//	
//	
//	ACCELER_ID = LSM303C_ACCELERO_ReadID();
//	printf("ACCELER_ID is 0x%x\n",ACCELER_ID);	    /* 0x41 */	
//	

//	MAGNETIC_ID = LSM303C_MAGNETIC_ReadID();
//	printf("MAGNETIC_ID is 0x%x\n",MAGNETIC_ID);	/* 0x3D */		
//	
	
//	AUDIO_ID = AUDIO_ReadID(AUDIO_I2C_ADDRESS);
//	printf("AUDIO_ID is 0x%x\n",AUDIO_ID);	        /* 0xE3 */		
		
	
//	if(HAL_UART_Transmit_DMA(&T_UartHandle, aTxBuffer, BUFFER_SIZE)!= HAL_OK)
//	{
//		
//		Error_Handler();
//	}	


	/* Start DFSDM conversions */
	if(HAL_DFSDM_FilterRegularStart_DMA(&DfsdmFilterHandle, RecBuff, 2048)!= HAL_OK)
	{
		
		Error_Handler();
		
	}
  
	while(1)
	{			
//		LED_GREEN_Toggle() ;
//		/* Insert delay 1000 ms */
//		HAL_Delay(1000);			

//		LED_RED_Toggle() ;
//		/* Insert delay 1000 ms */
//		HAL_Delay(1000);	
		
		
	if(DmaRecHalfBuffCplt == 1)
	{
		/* Store values on Play buff */
		for(uint32_t i = 0; i < 1024; i++)
		{
			PlayBuff[2*i]     = SaturaLH((RecBuff[i] >> 8), -32768, 32767);
			PlayBuff[(2*i)+1] = PlayBuff[2*i];
		}
		
		if(PlaybackStarted == 0)
		{
			if(CS43L22_Play(AUDIO_I2C_ADDRESS, (uint16_t *) &PlayBuff[0], 4096)!= 0)
			{
				Error_Handler();
			}
			
			if(HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *) &PlayBuff[0], 4096)!= HAL_OK)
			{
				Error_Handler();
			}
			PlaybackStarted = 1;
		}      
		DmaRecHalfBuffCplt  = 0;
	}
	if(DmaRecBuffCplt == 1)
	{
		/* Store values on Play buff */
		for(uint32_t i = 1024; i < 2048; i++)
		{
			PlayBuff[2*i]     = SaturaLH((RecBuff[i] >> 8), -32768, 32767);
			PlayBuff[(2*i)+1] = PlayBuff[2*i];
		}
		DmaRecBuffCplt  = 0;
	}		
		
		
	
	
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
