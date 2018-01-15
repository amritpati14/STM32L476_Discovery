/********************************************************************************
  * file name		:	bsp_flash.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/

#include "bsp_flash.h"


QSPI_HandleTypeDef QSPIHandle;


static void    Flash_QSPI_MspInit(void);
static uint8_t Flash_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
static uint8_t Flash_QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint8_t Flash_QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static uint8_t Flash_QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);

/**
  * @brief  Initializes the QSPI interface.
  * @retval QSPI memory status
  */
uint8_t BSP_Flash_Init(void)
{
	QSPIHandle.Instance = QUADSPI;

	/* Call the DeInit function to reset the driver */
	if (HAL_QSPI_DeInit(&QSPIHandle) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* System level initialization */
	Flash_QSPI_MspInit();

	/* QSPI initialization */
	QSPIHandle.Init.ClockPrescaler     = 1; /* QSPI clock = 80MHz / (ClockPrescaler+1) = 40MHz */
	QSPIHandle.Init.FifoThreshold      = 4;
	QSPIHandle.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_NONE;
	QSPIHandle.Init.FlashSize          = POSITION_VAL(N25Q128A_FLASH_SIZE) - 1;    //flash size
	QSPIHandle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	QSPIHandle.Init.ClockMode          = QSPI_CLOCK_MODE_0;

	if (HAL_QSPI_Init(&QSPIHandle) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* QSPI memory reset */
	if (Flash_QSPI_ResetMemory(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_NOT_SUPPORTED;
	}

	/* Configuration of the dummy cucles on QSPI memory side */
	if (Flash_QSPI_DummyCyclesCfg(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_NOT_SUPPORTED;
	}

	return QSPI_OK;
}


/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the read command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QUAD_INOUT_FAST_READ_CMD;           //Instruction
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.Address           = ReadAddr;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = N25Q128A_DUMMY_CYCLES_READ_QUAD;    //Dummy Cycles
	sCommand.NbData            = Size;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
	QSPI_CommandTypeDef sCommand;
	uint32_t end_addr, current_size, current_addr;

	/* Calculation of the size between the write address and the end of the page */
	current_size = N25Q128A_PAGE_SIZE - (WriteAddr % N25Q128A_PAGE_SIZE);    //page_size

	/* Check if the size of the data is less than the remaining place in the page */
	if (current_size > Size)
	{
		current_size = Size;
	}

	/* Initialize the adress variables */
	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;

	/* Initialize the program command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = EXT_QUAD_IN_FAST_PROG_CMD;    //Instruction
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Perform the write page by page */
	do
	{
		sCommand.Address = current_addr;
		sCommand.NbData  = current_size;

		/* Enable write operations */
		if (Flash_QSPI_WriteEnable(&QSPIHandle) != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* Configure the command */
		if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* Transmission of the data */
		if (HAL_QSPI_Transmit(&QSPIHandle, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
		{
			return QSPI_ERROR;
		}

		/* Configure automatic polling mode to wait for end of program */
		if (Flash_QSPI_AutoPollingMemReady(&QSPIHandle, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
		{
			return QSPI_ERROR;
		}

		/* Update the address and size variables for next page programming */
		current_addr += current_size;
		pData += current_size;
		current_size = ((current_addr + N25Q128A_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : N25Q128A_PAGE_SIZE;    //page_size
	}while (current_addr < end_addr);

	return QSPI_OK;
}



/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_Erase_Block(uint32_t BlockAddress)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the erase command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = SUBSECTOR_ERASE_CMD;    //Instruction
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.Address           = BlockAddress;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (Flash_QSPI_WriteEnable(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait for end of erase */
	if (Flash_QSPI_AutoPollingMemReady(&QSPIHandle, N25Q128A_SUBSECTOR_ERASE_MAX_TIME) != QSPI_OK)    //subsector erase max time
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


/**
  * @brief  Erases the specified sector of the QSPI memory.
  * @param  Sector: Sector address to erase (0 to 255)
  * @retval QSPI memory status
  * @note This function is non blocking meaning that sector erase
  *       operation is started but not completed when the function
  *       returns. Application has to call BSP_QSPI_GetStatus()
  *       to know when the device is available again (i.e. erase operation
  *       completed).
  */
uint8_t Flash_QSPI_Erase_Sector(uint32_t Sector)
{
	QSPI_CommandTypeDef sCommand;

	if (Sector >= (uint32_t)(N25Q128A_FLASH_SIZE / N25Q128A_SECTOR_SIZE))
	{
		return QSPI_ERROR;
	}

	/* Initialize the erase command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = SECTOR_ERASE_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.Address           = (Sector * N25Q128A_SECTOR_SIZE);
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (Flash_QSPI_WriteEnable(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


/**
  * @brief  Erases the entire QSPI memory.
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_Erase_Chip(void)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the erase command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = BULK_ERASE_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Enable write operations */
	if (Flash_QSPI_WriteEnable(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* Send the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait for end of erase */
	if (Flash_QSPI_AutoPollingMemReady(&QSPIHandle, N25Q128A_BULK_ERASE_MAX_TIME) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}




/**
  * @brief  Reads current status of the QSPI memory.
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_GetStatus(void)
{
	QSPI_CommandTypeDef sCommand;
	uint8_t reg;

	/* Initialize the read flag status register command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = READ_FLAG_STATUS_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Check the value of the register */
	if ((reg & (N25Q128A_FSR_PRERR | N25Q128A_FSR_VPPERR | N25Q128A_FSR_PGERR | N25Q128A_FSR_ERERR)) != 0)
	{
		return QSPI_ERROR;
	}
	else if ((reg & (N25Q128A_FSR_PGSUS | N25Q128A_FSR_ERSUS)) != 0)
	{
		return QSPI_SUSPENDED;
	}
	else if ((reg & N25Q128A_FSR_READY) != 0)
	{
		return QSPI_OK;
	}
	else
	{
		return QSPI_BUSY;
	}
}

/**
  * @brief  Return the configuration of the QSPI memory.
  * @param  pInfo: pointer on the configuration structure
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_GetInfo(QSPI_Info *pInfo)
{
	/* Configure the structure with the memory configuration */
	pInfo->FlashSize          = N25Q128A_FLASH_SIZE;
	pInfo->EraseSectorSize    = N25Q128A_SUBSECTOR_SIZE;
	pInfo->EraseSectorsNumber = (N25Q128A_FLASH_SIZE / N25Q128A_SUBSECTOR_SIZE);
	pInfo->ProgPageSize       = N25Q128A_PAGE_SIZE;
	pInfo->ProgPagesNumber    = (N25Q128A_FLASH_SIZE / N25Q128A_PAGE_SIZE);

	return QSPI_OK;
}

/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @retval QSPI memory status
  */
uint8_t Flash_QSPI_EnableMemoryMappedMode(void)
{
	QSPI_CommandTypeDef      sCommand;
	QSPI_MemoryMappedTypeDef sMemMappedCfg;

	/* Configure the command for the read instruction */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = QUAD_INOUT_FAST_READ_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = N25Q128A_DUMMY_CYCLES_READ_QUAD;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the memory mapped mode */
	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;

	if (HAL_QSPI_MemoryMapped(&QSPIHandle, &sCommand, &sMemMappedCfg) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


/**
  * @brief  Initializes the QSPI MSP.
  * @retval None
  */
static void Flash_QSPI_MspInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable the QuadSPI memory interface clock */
	__HAL_RCC_QSPI_CLK_ENABLE();

	/* Reset the QuadSPI memory interface */
	__HAL_RCC_QSPI_FORCE_RESET();
	__HAL_RCC_QSPI_RELEASE_RESET();

	/* Enable GPIO clocks */
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* QSPI CS GPIO pin configuration  */
	GPIO_InitStruct.Pin       = GPIO_PIN_11;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* QSPI CLK, D0, D1, D2 and D3 GPIO pins configuration  */
	GPIO_InitStruct.Pin       = (GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}


/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t Flash_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the reset enable command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = RESET_ENABLE_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Send the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Send the reset memory command */
	sCommand.Instruction = RESET_MEMORY_CMD;
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait the memory is ready */
	if (Flash_QSPI_AutoPollingMemReady(&QSPIHandle, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t Flash_QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef sCommand;
	uint8_t reg;

	/* Initialize the read volatile configuration register command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = READ_VOL_CFG_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Enable write operations */
	if (Flash_QSPI_WriteEnable(&QSPIHandle) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* Update volatile configuration register (with new dummy cycles) */
	sCommand.Instruction = WRITE_VOL_CFG_REG_CMD;
	MODIFY_REG(reg, N25Q128A_VCR_NB_DUMMY, (N25Q128A_DUMMY_CYCLES_READ_QUAD << POSITION_VAL(N25Q128A_VCR_NB_DUMMY)));

	/* Configure the write volatile configuration register command */
	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Transmission of the data */
	if (HAL_QSPI_Transmit(&QSPIHandle, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t Flash_QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
	QSPI_CommandTypeDef     sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Enable write operations */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = WRITE_ENABLE_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&QSPIHandle, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait for write enabling */
	sConfig.Match           = N25Q128A_SR_WREN;
	sConfig.Mask            = N25Q128A_SR_WREN;
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	sCommand.Instruction    = READ_STATUS_REG_CMD;
	sCommand.DataMode       = QSPI_DATA_1_LINE;

	if (HAL_QSPI_AutoPolling(&QSPIHandle, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout: Timeout for auto-polling
  * @retval None
  */
static uint8_t Flash_QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
	QSPI_CommandTypeDef     sCommand;
	QSPI_AutoPollingTypeDef sConfig;

	/* Configure automatic polling mode to wait for memory ready */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = READ_STATUS_REG_CMD;    //Instruction
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	sConfig.Match           = 0;
	sConfig.Mask            = N25Q128A_SR_WIP;    //
	sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
	sConfig.StatusBytesSize = 1;
	sConfig.Interval        = 0x10;
	sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	if (HAL_QSPI_AutoPolling(&QSPIHandle, &sCommand, &sConfig, Timeout) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}















/***********************************END OF FILE**********************************/


