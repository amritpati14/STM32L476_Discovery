/********************************************************************************
  * file name		:	bsp_timer.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/
#include "bsp_timer.h"

TIM_HandleTypeDef TIM3_Handler;
TIM_IC_InitTypeDef TIM3_sConfig;

TIM_HandleTypeDef TIM4_Handler;
TIM_OC_InitTypeDef  TIM4_sConfig;

TIM_HandleTypeDef TIM5_Handler;
TIM_OC_InitTypeDef  TIM5_sConfig;

DMA_HandleTypeDef DMA_TIM_IC;


/* IC interrupt */
//void BSP_TIM3_IC_Init(void)
//{  	
//	TIM3_Handler.Instance               = TIM3;  
//	
//	TIM3_Handler.Init.Prescaler         = 0;        
//	TIM3_Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
//	TIM3_Handler.Init.Period            = 0;         
//	TIM3_Handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1; 			
//	TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//	
//	if(HAL_TIM_IC_Init(&TIM3_Handler) != HAL_OK)
//	{
//		/* Initialization Error */
//		Error_Handler();
//	}	
//	
//	TIM3_sConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
//	TIM3_sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
//	TIM3_sConfig.ICPrescaler = TIM_ICPSC_DIV1;
//	TIM3_sConfig.ICFilter    = 0; 
//	
//	if(HAL_TIM_IC_ConfigChannel(&TIM3_Handler, &TIM3_sConfig, TIM_CHANNEL_1) != HAL_OK)
//	{
//		/* Configuration Error */
//		Error_Handler();
//	}	
//	
//	HAL_TIM_IC_Start_IT(&TIM3_Handler,TIM_CHANNEL_1); 
//		
//} 


/* IC DMA */
void BSP_TIM3_IC_Init(void)
{  	
	TIM3_Handler.Instance               = TIM3;  
	
	TIM3_Handler.Init.Prescaler         = 0;        
	TIM3_Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TIM3_Handler.Init.Period            = 0;         
	TIM3_Handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1; 			
	TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	if(HAL_TIM_IC_Init(&TIM3_Handler) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
	TIM3_sConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
	TIM3_sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM3_sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	TIM3_sConfig.ICFilter    = 0; 
	
	if(HAL_TIM_IC_ConfigChannel(&TIM3_Handler, &TIM3_sConfig, TIM_CHANNEL_1) != HAL_OK)
	{
		/* Configuration Error */
		Error_Handler();
	}	
	
	TIM_IC_Start_DMA(&TIM3_Handler,TIM_CHANNEL_1,ProbeData,Probe_Data_Size); 		//Probe_Data_Size	
	
} 

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) 
{ 
	if(htim == &TIM3_Handler)
	{
		GPIO_InitTypeDef GPIO_Initure; 		
		
		__HAL_RCC_TIM3_CLK_ENABLE();  	
		__HAL_RCC_GPIOC_CLK_ENABLE();    
		__HAL_RCC_GPIOD_CLK_ENABLE();  		
		  
		GPIO_Initure.Pin       = GPIO_PIN_6;             
		GPIO_Initure.Mode      = GPIO_MODE_AF_PP;     
		GPIO_Initure.Pull      = GPIO_PULLUP;        
		GPIO_Initure.Speed     = GPIO_SPEED_HIGH;    
		GPIO_Initure.Alternate = GPIO_AF2_TIM3;  	

		HAL_GPIO_Init(GPIOC,&GPIO_Initure); 
		
		
		GPIO_Initure.Pin       = GPIO_PIN_All;             
		GPIO_Initure.Mode      = GPIO_MODE_INPUT;     
		GPIO_Initure.Pull      = GPIO_PULLDOWN;         	

		HAL_GPIO_Init(GPIOE,&GPIO_Initure); 		
		
		
		/* Enable DMA clock */
		__HAL_RCC_DMA1_CLK_ENABLE();	
		
		/* Configure the DMA handler for Transmission process */
		DMA_TIM_IC.Instance                 = DMA1_Channel6;

		DMA_TIM_IC.Init.Request             = DMA_REQUEST_5;	
		DMA_TIM_IC.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		DMA_TIM_IC.Init.PeriphInc           = DMA_PINC_DISABLE;
		DMA_TIM_IC.Init.MemInc              = DMA_MINC_ENABLE;
		DMA_TIM_IC.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		DMA_TIM_IC.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
//		DMA_TIM_IC.Init.Mode                = DMA_NORMAL;
		DMA_TIM_IC.Init.Mode                = DMA_CIRCULAR;		
		DMA_TIM_IC.Init.Priority            = DMA_PRIORITY_VERY_HIGH;

		HAL_DMA_Init(&DMA_TIM_IC);

		/* Link hdma_tim to hdma[TIM_DMA_ID_CC1] (channel1) */
		__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC1], DMA_TIM_IC);

		/* Initialize TIMx DMA handle */
		HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC1]);	
		
		/* Enable NVIC */
		HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);		
		
//		HAL_NVIC_SetPriority(TIM3_IRQn,0,0);     
//		HAL_NVIC_EnableIRQ(TIM3_IRQn);  		
	}
	 	
}


HAL_StatusTypeDef TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((pData == 0 ) && (Length > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&GPIOE->IDR, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 2  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->CCR3, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 3  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->CCR4, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 4  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}




void BSP_TIM4_PWM_Init(void)
{  	
	TIM4_Handler.Instance               = TIM4;  
	
	TIM4_Handler.Init.Prescaler         = (800-1);        
	TIM4_Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TIM4_Handler.Init.Period            = (10000-1);        
	TIM4_Handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1; 			
	TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	if(HAL_TIM_PWM_Init(&TIM4_Handler) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
		
	TIM4_sConfig.OCMode = TIM_OCMODE_PWM1;
	TIM4_sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM4_sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	TIM4_sConfig.Pulse = 5000;
	
	if(HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_sConfig,TIM_CHANNEL_4) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
	HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_4);	
	
} 

void BSP_TIM5_PWM_Init(void)
{  	
	TIM5_Handler.Instance               = TIM5;  
	
	TIM5_Handler.Init.Prescaler         = (800-1);        
	TIM5_Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TIM5_Handler.Init.Period            = (10000-1);         
	TIM5_Handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1; 			
	TIM5_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	if(HAL_TIM_PWM_Init(&TIM5_Handler) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
		
	TIM5_sConfig.OCMode = TIM_OCMODE_PWM1;
	TIM5_sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM5_sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	TIM5_sConfig.Pulse = 5000;
	
	if(HAL_TIM_PWM_ConfigChannel(&TIM5_Handler,&TIM5_sConfig,TIM_CHANNEL_2) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}	
	
	HAL_TIM_PWM_Start(&TIM5_Handler,TIM_CHANNEL_2);	
	
} 




void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) 
{ 
	GPIO_InitTypeDef GPIO_Initure; 
	
	if(htim == &TIM4_Handler)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();  	
		__HAL_RCC_GPIOB_CLK_ENABLE();    
		  
		GPIO_Initure.Pin       = GPIO_PIN_9;             
		GPIO_Initure.Mode      = GPIO_MODE_AF_PP;     
		GPIO_Initure.Pull      = GPIO_PULLUP;        
		GPIO_Initure.Speed     = GPIO_SPEED_HIGH;    
		GPIO_Initure.Alternate = GPIO_AF2_TIM4;  	

		HAL_GPIO_Init(GPIOB,&GPIO_Initure); 		
	}

	if(htim == &TIM5_Handler)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();  	
		__HAL_RCC_GPIOA_CLK_ENABLE();    
		  
		GPIO_Initure.Pin       = GPIO_PIN_1;             
		GPIO_Initure.Mode      = GPIO_MODE_AF_PP;     
		GPIO_Initure.Pull      = GPIO_PULLUP;        
		GPIO_Initure.Speed     = GPIO_SPEED_HIGH;    
		GPIO_Initure.Alternate = GPIO_AF2_TIM5;  	

		HAL_GPIO_Init(GPIOA,&GPIO_Initure); 		
	}	
}






void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

//	timer_conter ++;
	timer_conter = 0x01;	
	
}

/***********************************END OF FILE**********************************/


