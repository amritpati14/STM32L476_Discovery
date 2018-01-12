/********************************************************************************
  * file name		:	bsp_acceler.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/
#include "bsp_acceler.h"


SPI_HandleTypeDef ACCEL_SpiHandle;

static void ACCELER_GPIO_Init(void);

static uint8_t SPIx_Read(void);
static void SPIx_Write(uint8_t TransmitByte);



/**
  * @brief  This function Initialize ACCELER.
  * @param  None
  * @retval None
  */
void BSP_ACCELER_Init(void)
{
	ACCELER_GPIO_Init();
	
	ACCEL_SpiHandle.Instance = ACCEL_SPIx;

	ACCEL_SpiHandle.Init.Mode              = SPI_MODE_MASTER;
	ACCEL_SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;			
	ACCEL_SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;	
	ACCEL_SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	ACCEL_SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	ACCEL_SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	ACCEL_SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	ACCEL_SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	ACCEL_SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	ACCEL_SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	ACCEL_SpiHandle.Init.CRCPolynomial     = 7;	

	if (HAL_SPI_Init(&ACCEL_SpiHandle) != HAL_OK)
	{
		Error_Handler();	
	}	
}


void ACCELERO_IO_Write(uint8_t RegisterAddr, uint8_t Value)
{
	ACCEL_CS_LOW();
	
	__SPI_DIRECTION_1LINE_TX(&ACCEL_SpiHandle);
	/* Call SPI Read data bus function */
	SPIx_Write(RegisterAddr);
	SPIx_Write(Value);
	
	ACCEL_CS_HIGH();
}


uint8_t ACCELERO_IO_Read(uint8_t RegisterAddr)
{
	uint8_t ReadData;
	
	RegisterAddr = RegisterAddr | ((uint8_t)0x80);
	
	ACCEL_CS_LOW();
	
	__SPI_DIRECTION_1LINE_TX(&ACCEL_SpiHandle);
	SPIx_Write(RegisterAddr);
	
	__SPI_DIRECTION_1LINE_RX(&ACCEL_SpiHandle);
	ReadData = SPIx_Read();
	
	ACCEL_CS_HIGH();
	
	return ReadData;
}

void MAGNETIC_IO_Write(uint8_t RegisterAddr, uint8_t Value)
{
	MAGNE_CS_LOW();
	
	__SPI_DIRECTION_1LINE_TX(&ACCEL_SpiHandle);
	/* Call SPI Read data bus function */
	SPIx_Write(RegisterAddr);
	SPIx_Write(Value);
	
	MAGNE_CS_HIGH();
}


uint8_t MAGNETIC_IO_Read(uint8_t RegisterAddr)
{
	uint8_t ReadData;
	
	RegisterAddr = RegisterAddr | ((uint8_t)0x80);
	
	MAGNE_CS_LOW();
	
	__SPI_DIRECTION_1LINE_TX(&ACCEL_SpiHandle);
	SPIx_Write(RegisterAddr);
	
	__SPI_DIRECTION_1LINE_RX(&ACCEL_SpiHandle);
	ReadData = SPIx_Read();
	
	MAGNE_CS_HIGH();
	
	return ReadData;
}

uint8_t LSM303C_ACCELERO_ReadID(void)
{
	uint8_t tmp = 0x00;
	   	
	ACCELERO_IO_Write(LSM303C_CTRL_REG4_A, 0x07);
	/* Read value at Who am I register address */
	tmp = ACCELERO_IO_Read(LSM303C_WHO_AM_I_ADDR);  
	
	return (uint8_t)tmp;
}

uint8_t LSM303C_MAGNETIC_ReadID(void)
{
	uint8_t tmp = 0x00;
	   	
	MAGNETIC_IO_Write(LSM303C_CTRL_REG3_M, 0x84);
	/* Read value at Who am I register address */
	tmp = MAGNETIC_IO_Read(LSM303C_WHO_AM_I_ADDR);  
	
	return (uint8_t)tmp;
}

/**
  * @brief  This function configurate ACCELER GPIO.
  * @param  None
  * @retval None
  */
static void ACCELER_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/* Enable ACCEL_CS gpio clock */		
	ACCEL_CS_GPIO_CLK_ENABLE();
	/* Enable MAGNE_CS gpio clock */	
	MAGNE_CS_GPIO_CLK_ENABLE();
	/* Enable SPIx gpio clock */
	ACCEL_SPIx_GPIO_CLK_ENABLE();	
	/* Enable SPIx clock */	
	ACCEL_SPIx_CLOCK_ENABLE();
	/* Configure SPIx SCK, MOSI and MISO */
	GPIO_InitStructure.Pin       = (ACCEL_SPIx_SCK_PIN | ACCEL_SPIx_MOSI_PIN | ACCEL_SPIx_MISO_PIN);	
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull      = GPIO_NOPULL;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = ACCEL_SPIx_AF;
	HAL_GPIO_Init(ACCEL_SPIx_GPIO_PORT, &GPIO_InitStructure);	
	
	/* Configure ACCELER CS */
	GPIO_InitStructure.Pin   = ACCEL_CS_PIN;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ACCEL_CS_GPIO_PORT, &GPIO_InitStructure);
	
	ACCEL_CS_HIGH();
	
	/* Configure MAGNETIC CS */
	GPIO_InitStructure.Pin   = MAGNE_CS_PIN;
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(MAGNE_CS_GPIO_PORT, &GPIO_InitStructure);
	
	MAGNE_CS_HIGH();	
}

static void SPIx_Write(uint8_t TransmitByte)
{
	/* Enable the SPI */
	__HAL_SPI_ENABLE(&ACCEL_SpiHandle);
	/* check TXE flag */
	while ((ACCEL_SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

	/* Write the data */
	*((__IO uint8_t *)&ACCEL_SpiHandle.Instance->DR) = TransmitByte;

	/* Wait BSY flag */
	while ((ACCEL_SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

	/* disable the SPI */
	__HAL_SPI_DISABLE(&ACCEL_SpiHandle);	
}


static uint8_t SPIx_Read(void)
{
	uint8_t ReceivedByte;
	__HAL_SPI_ENABLE(&ACCEL_SpiHandle);
	__DSB();
	__DSB();
	__DSB();
	__DSB();
	__DSB();
	__DSB();
	__DSB();
	__DSB();
	__HAL_SPI_DISABLE(&ACCEL_SpiHandle);

	while ((ACCEL_SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
	/* read the received data */
	ReceivedByte = *(__IO uint8_t *)&ACCEL_SpiHandle.Instance->DR;

	/* Wait for the BSY flag reset */
	while ((ACCEL_SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

	return ReceivedByte;	
}



/***********************************END OF FILE**********************************/


