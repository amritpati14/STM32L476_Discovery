/********************************************************************************
  * file name		:	bsp_cs43l22.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/

#include "bsp_cs43l22.h"



I2C_HandleTypeDef            I2C1_Handle;

DMA_HandleTypeDef            hDfsdmDma;
DMA_HandleTypeDef            hSaiDma;

DFSDM_Channel_HandleTypeDef  DfsdmChannelHandle;
DFSDM_Filter_HandleTypeDef   DfsdmFilterHandle;

SAI_HandleTypeDef            SaiHandle;

static HAL_StatusTypeDef I2C1_ReadBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);

static HAL_StatusTypeDef I2C1_WriteBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);

static void I2C1_Error(void);
static void CS43L22_I2C_GPIO_Init(void);
static void CS43L22_I2C_NVIC_Init(void);

static void CS43L22_RESET(void);



static uint8_t Is_cs43l22_Stop = 1;

volatile uint8_t OutputDev = 0;





/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void CS43L22_I2C_Init(void)
{
	CS43L22_I2C_GPIO_Init();
	CS43L22_I2C_NVIC_Init();

	I2C1_Handle.Instance              = CS43L22_I2C;
	
    I2C1_Handle.Init.Timing           = AUDIO_I2C1_TIMING;
    I2C1_Handle.Init.OwnAddress1      = 0;
    I2C1_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2C1_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    I2C1_Handle.Init.OwnAddress2      = 0;
    I2C1_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2C1_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

	
	if (HAL_I2C_Init(&I2C1_Handle) != HAL_OK)
	{
		Error_Handler();
	}
	
	
	CS43L22_RESET();
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
uint32_t AUDIO_ReadID(uint16_t DeviceAddr)
{
	uint8_t Value;
	  
	Value = AUDIO_IO_Read(DeviceAddr, CS43L22_REG_ID);
	  
	return((uint32_t) Value);
}




/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */

uint8_t AUDIO_IO_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t Read_Value = 0;

	I2C1_ReadBuffer((uint16_t) Addr, (uint16_t) Reg, I2C_MEMADD_SIZE_8BIT, &Read_Value, 1);
	
	return Read_Value;
}


void AUDIO_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	
	I2C1_WriteBuffer(Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1);
	
}


/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
	/* Init of clock, gpio and PLLSAI1 clock */
	GPIO_InitTypeDef GPIO_Init;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
	  
	/* Enable DFSDM clock */
	__HAL_RCC_DFSDM1_CLK_ENABLE();
	  
	/* Configure PE9 for DFSDM_CKOUT and PE7 for DFSDM_DATIN2 */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_PULLDOWN;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = GPIO_AF6_DFSDM1;
	GPIO_Init.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOE, &GPIO_Init);
	GPIO_Init.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOE, &GPIO_Init);
	  
	/* Configure and enable PLLSAI1 clock to generate 11.294MHz */
	RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI1;
	RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_MSI;
	RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1M        = 1;
	RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1N        = 48;
	RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1P        = 17;
	RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
	RCC_PeriphCLKInitStruct.Sai1ClockSelection      = RCC_SAI1CLKSOURCE_PLLSAI1;
	if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */

void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	/* Configure DMA1_Channel4 */
	__HAL_RCC_DMA1_CLK_ENABLE();
	hDfsdmDma.Init.Request             = DMA_REQUEST_0;
	hDfsdmDma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hDfsdmDma.Init.PeriphInc           = DMA_PINC_DISABLE;
	hDfsdmDma.Init.MemInc              = DMA_MINC_ENABLE;
	hDfsdmDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hDfsdmDma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
	hDfsdmDma.Init.Mode                = DMA_CIRCULAR;
	hDfsdmDma.Init.Priority            = DMA_PRIORITY_HIGH;
	hDfsdmDma.Instance                 = DMA1_Channel4;
	__HAL_LINKDMA(hdfsdm_filter, hdmaReg, hDfsdmDma);
	if (HAL_OK != HAL_DMA_Init(&hDfsdmDma))
	{
		Error_Handler();
	}
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0x01, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}



/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
	GPIO_InitTypeDef  GPIO_Init;
	  
	/* Enable SAI1 clock */
	__HAL_RCC_SAI1_CLK_ENABLE();
	  
	/* Configure GPIOs used for SAI1 */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_Init.Mode      = GPIO_MODE_AF_PP;
	GPIO_Init.Pull      = GPIO_NOPULL;
	GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = GPIO_AF13_SAI1;
	GPIO_Init.Pin       = (GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
	HAL_GPIO_Init(GPIOE, &GPIO_Init);
	  
	/* Configure DMA used for SAI1 */
	__HAL_RCC_DMA2_CLK_ENABLE();
	hSaiDma.Init.Request             = DMA_REQUEST_1;
	hSaiDma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
	hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
	hSaiDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hSaiDma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
	hSaiDma.Init.Mode                = DMA_CIRCULAR;
	hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;
	hSaiDma.Instance                 = DMA2_Channel1;
	__HAL_LINKDMA(hsai, hdmatx, hSaiDma);
	if (HAL_OK != HAL_DMA_Init(&hSaiDma))
	{
		Error_Handler();
	}
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0x01, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
}



/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void DFSDM_Init(void)
{
	/* Initialize channel 2 */
	__HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmChannelHandle);
		
	DfsdmChannelHandle.Instance                      = DFSDM1_Channel2;
	DfsdmChannelHandle.Init.OutputClock.Activation   = ENABLE;
	DfsdmChannelHandle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
	DfsdmChannelHandle.Init.OutputClock.Divider      = 4; /* 11.294MHz/4 = 2.82MHz */
	DfsdmChannelHandle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
	DfsdmChannelHandle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
	DfsdmChannelHandle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
	DfsdmChannelHandle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
	DfsdmChannelHandle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
	DfsdmChannelHandle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
	DfsdmChannelHandle.Init.Awd.Oversampling         = 10; /* N.U. */
	DfsdmChannelHandle.Init.Offset                   = 0;
	DfsdmChannelHandle.Init.RightBitShift            = 2;
	if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmChannelHandle))
	{
		Error_Handler();
	}

	/* Initialize filter 0 */
	__HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilterHandle);
	DfsdmFilterHandle.Instance                          = DFSDM1_Filter0;
	DfsdmFilterHandle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
	DfsdmFilterHandle.Init.RegularParam.FastMode        = ENABLE;
	DfsdmFilterHandle.Init.RegularParam.DmaMode         = ENABLE;
	DfsdmFilterHandle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
	DfsdmFilterHandle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
	DfsdmFilterHandle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
	DfsdmFilterHandle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
	DfsdmFilterHandle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_RISING_EDGE; /* N.U. */
	DfsdmFilterHandle.Init.FilterParam.SincOrder        = DFSDM_FILTER_SINC3_ORDER;
	DfsdmFilterHandle.Init.FilterParam.Oversampling     = 64; /* 11.294MHz/(4*64) = 44.1KHz */
	DfsdmFilterHandle.Init.FilterParam.IntOversampling  = 1;
	if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilterHandle))
	{
		Error_Handler();
	}

	/* Configure regular channel and continuous mode for filter 0 */
	if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmFilterHandle, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON))
	{
		Error_Handler();
	}
}


/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void Playback_Init(void)
{
	/* Initialize SAI */
	__HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);

	SaiHandle.Instance = SAI1_Block_A;

	SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
	SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
	SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
	SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
	SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
	SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
	SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
	SaiHandle.Init.Mckdiv         = 0; /* N.U */
	SaiHandle.Init.MonoStereoMode = SAI_STEREOMODE;
	SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
	SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
	SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
	SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
	SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
	SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;

	SaiHandle.FrameInit.FrameLength       = 32; 
	SaiHandle.FrameInit.ActiveFrameLength = 16;
	SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
	SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
	SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

	SaiHandle.SlotInit.FirstBitOffset = 0;
	SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
	SaiHandle.SlotInit.SlotNumber     = 2; 
	SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

	if(HAL_OK != HAL_SAI_Init(&SaiHandle))
	{
		Error_Handler();
	}
	  
	/* Enable SAI to generate clock used by audio driver */
	__HAL_SAI_ENABLE(&SaiHandle);
	  
	/* Initialize audio driver */

	
	if(CS43L22_Init(AUDIO_I2C_ADDRESS,OUTPUT_DEVICE_HEADPHONE, 90, AUDIO_FREQUENCY_44K) != 0)
	{
		Error_Handler();
	}
	
}




/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
uint32_t CS43L22_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
	uint32_t counter = 0;
	  
	/* Initialize the Control interface of the Audio Codec */
	CS43L22_I2C_Init();     
		
	/* Keep Codec powered OFF */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x01);  
	  
	/*Save Output device for mute ON/OFF procedure*/
	switch (OutputDevice)
	{
	case OUTPUT_DEVICE_SPEAKER:
		OutputDev = 0xFA;
		break;
		
	case OUTPUT_DEVICE_HEADPHONE:
		OutputDev = 0xAF;
		break;
		
	case OUTPUT_DEVICE_BOTH:
		OutputDev = 0xAA;
		break;
		
	case OUTPUT_DEVICE_AUTO:
		OutputDev = 0x05;
		break;    
		
	default:
		OutputDev = 0x05;
		break;    
	}
	  
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);
	  
	/* Clock configuration: Auto detection */  
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_CLOCKING_CTL, 0x81);
	  
	/* Set the Slave Mode and the audio Standard */  
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_INTERFACE_CTL1, CODEC_STANDARD);
	  
	/* Set the Master volume */
	counter += cs43l22_SetVolume(DeviceAddr, Volume);
	  
	/* If the Speaker is enabled, set the Mono mode and volume attenuation level */
	if(OutputDevice != OUTPUT_DEVICE_HEADPHONE)
	{
	/* Set the Speaker Mono mode */  
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PLAYBACK_CTL2, 0x06);
		
	/* Set the Speaker attenuation level */  
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_SPEAKER_A_VOL, 0x00);
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_SPEAKER_B_VOL, 0x00);
	}
	  
	/* Additional configuration for the CODEC. These configurations are done to reduce
	the time needed for the Codec to power off. If these configurations are removed, 
	then a long delay should be added between powering off the Codec and switching 
	off the I2S peripheral MCLK clock (which is the operating clock for Codec).
	If this delay is not inserted, then the codec will not shut down properly and
	it results in high noise after shut down. */
	  
	/* Disable the analog soft ramp */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_ANALOG_ZC_SR_SETT, 0x00);
	/* Disable the digital soft ramp */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MISC_CTL, 0x04);
	/* Disable the limiter attack level */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_LIMIT_CTL1, 0x00);
	/* Adjust Bass and Treble levels */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_TONE_CTL, 0x0F);
	/* Adjust PCM volume level */
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PCMA_VOL, 0x0A);
	counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_PCMB_VOL, 0x0A);
	  
	/* Return communication control value */
	return counter;  
}



uint32_t CS43L22_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
	uint32_t counter = 0;
	  
	if(Is_cs43l22_Stop == 1)
	{
		/* Enable the digital soft ramp */
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MISC_CTL, 0x06);
		  
		/* Enable Output device */  
		counter += cs43l22_SetMute(DeviceAddr, AUDIO_MUTE_OFF);
			
		/* Power on the Codec */
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL1, 0x9E);  
		Is_cs43l22_Stop = 0;
	}
	  
	/* Return communication control value */
	return counter;  
}


uint32_t cs43l22_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
	uint32_t counter = 0;
	  
	/* Set the Mute mode */
	if(Cmd == AUDIO_MUTE_ON)
	{
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, 0xFF);
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x01);
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x01);
	}
	else /* AUDIO_MUTE_OFF Disable the Mute */
	{
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_A_VOL, 0x00);
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_HEADPHONE_B_VOL, 0x00);
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_POWER_CTL2, OutputDev);
	}
	return counter;
}



/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
uint32_t cs43l22_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
	uint32_t counter = 0;
	uint8_t convertedvol = VOLUME_CONVERT(Volume);

	if(Volume > 0xE6)
	{
		/* Set the Master volume */
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol - 0xE7); 
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol - 0xE7);     
	}
	else
	{
		/* Set the Master volume */
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_A_VOL, convertedvol + 0x19); 
		counter += CODEC_IO_Write(DeviceAddr, CS43L22_REG_MASTER_B_VOL, convertedvol + 0x19); 
	}

	return counter;
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	uint32_t result = 0;
	  
	AUDIO_IO_Write(Addr, Reg, Value);
	  
	return result;
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	
	DmaRecHalfBuffCplt = 1;
	
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	
	DmaRecBuffCplt = 1;
	
}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
static HAL_StatusTypeDef I2C1_ReadBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(&I2C1_Handle, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I21_Timeout);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the BUS */
		I2C1_Error();
	}
	return status;
}
/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
static HAL_StatusTypeDef I2C1_WriteBuffer(uint16_t Addr, uint16_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&I2C1_Handle, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I21_Timeout);

	/* Check the communication status */
	if (status != HAL_OK)
	{
		/* Re-Initiaize the BUS */
		I2C1_Error();
	}

	return status;
}



static void I2C1_Error(void)
{
	/* De-initialize the I2C communication BUS */
	HAL_I2C_DeInit(&I2C1_Handle);

	/* Re- Initiaize the I2C communication BUS */
	CS43L22_I2C_Init();
}



/**
  * @brief   CS43L22 I2C GPIO Init
  * @param   
  * @retval  
  * @note
  *
  */
static void CS43L22_I2C_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
	
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWREx_EnableVddIO2();	
	
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
	
    /* Enable GPIO clock */
    CS43L22_I2C_CLK_ENABLE();
	AUDIO_RESET_GPIO_CLK_ENABLE();

    /* Configure I2C Rx/Tx as alternate function  */
    GPIO_InitStructure.Pin       = CS43L22_I2C_SCL_GPIO_PIN | CS43L22_I2C_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Alternate = CS43L22_I2C_AF;
	
    HAL_GPIO_Init(CS43L22_I2C_GPIO_PORT, &GPIO_InitStructure);
	
	
    GPIO_InitStructure.Pin       = AUDIO_RESET_GPIO_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	
    HAL_GPIO_Init(AUDIO_RESET_GPIO_PORT, &GPIO_InitStructure);	

}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
static void CS43L22_I2C_NVIC_Init(void)
{
	
    /* Enable and set Discovery I2C1 Event Interrupt to the highest priority */
    HAL_NVIC_SetPriority(CS43L22_I2C_EV_IRQn, 0x00, 0);
    HAL_NVIC_EnableIRQ(CS43L22_I2C_EV_IRQn);

    /* Enable and set Discovery I2C1 Error Interrupt to the highest priority */
    HAL_NVIC_SetPriority(CS43L22_I2C_ER_IRQn, 0x00, 0);
    HAL_NVIC_EnableIRQ(CS43L22_I2C_ER_IRQn);

}

/**
  * @brief   
  * @param   
  * @retval  
  * @note
  *
  */
static void CS43L22_RESET(void)
{

	/* Power Down the codec */
	AUDIO_RESET_LOW();

	/* wait for a delay to insure registers erasing */
	HAL_Delay(5);

	/* Power on the codec */
	AUDIO_RESET_HIGH();

	/* wait for a delay to insure registers erasing */
	HAL_Delay(5);

}






/***********************************END OF FILE**********************************/


