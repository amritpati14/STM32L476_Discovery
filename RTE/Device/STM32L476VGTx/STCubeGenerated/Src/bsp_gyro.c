/********************************************************************************
  * file name		:	bsp_gyro.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/

#include "bsp_gyro.h"

SPI_HandleTypeDef GYRO_SpiHandle;

static void GYRO_GPIO_Init(void);

static uint8_t SPIx_WriteRead(uint8_t Byte);


/**
  * @brief  This function Initialize GYRO.
  * @param  None
  * @retval None
  */
void BSP_GYRO_Init(void)
{
	GYRO_GPIO_Init();
					
	GYRO_SpiHandle.Instance = GYRO_SPIx;

	GYRO_SpiHandle.Init.Mode              = SPI_MODE_MASTER;
	GYRO_SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;		
	GYRO_SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;	
	GYRO_SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	GYRO_SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	GYRO_SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	GYRO_SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	GYRO_SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	GYRO_SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	GYRO_SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	GYRO_SpiHandle.Init.CRCPolynomial     = 7;	

	if (HAL_SPI_Init(&GYRO_SpiHandle) != HAL_OK)
	{
		Error_Handler();	
	}			
}
/**
  * @brief  This function write data through SPI bus.
  * @param  pBuffer : pointer to data buffer
  * @param  WriteAddr : register
  * @param  NumByteToWrite : amount of data to be received
  * @retval None
  */
void GYRO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	/* Configure the MS bit:
		- When 0, the address will remain unchanged in multiple read/write commands.
		- When 1, the address will be auto incremented in multiple read/write commands.
	*/
	if (NumByteToRead > 0x01)
	{
		ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
	}
	else
	{
		ReadAddr |= (uint8_t)READWRITE_CMD;
	}
	/* Set chip select Low at the start of the transmission */
	GYRO_CS_LOW();
	
	/* Send the Address of the indexed register */
	SPIx_WriteRead(ReadAddr);

	/* Receive the data that will be read from the device (MSB First) */
	while (NumByteToRead >= 0x01)
	{
		/* Send dummy byte (0x00) to generate the SPI clock to GYROSCOPE (Slave device) */
		*pBuffer = SPIx_WriteRead(0x00);
		NumByteToRead--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */
	GYRO_CS_HIGH();

}
/**
  * @brief  This function write data through SPI bus.
  * @param  pBuffer : pointer to data buffer
  * @param  WriteAddr : register
  * @param  NumByteToWrite : amount of data to be sent
  * @retval None
  */
void GYRO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	/* Configure the MS bit:
		- When 0, the address will remain unchanged in multiple read/write commands.
		- When 1, the address will be auto incremented in multiple read/write commands.
	*/
	if (NumByteToWrite > 0x01)
	{
		WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
	}
	/* Set chip select Low at the start of the transmission */
	GYRO_CS_LOW();

	/* Send the Address of the indexed register */
	SPIx_WriteRead(WriteAddr);

	/* Send the data that will be written into the device (MSB First) */
	while (NumByteToWrite >= 0x01)
	{
		SPIx_WriteRead(*pBuffer);
		NumByteToWrite--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */
	GYRO_CS_HIGH();
}
/**
  * @brief  This function configurate GYRO GPIO.
  * @param  None
  * @retval None
  */
static void GYRO_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;	
		
	GYRO_CS_GPIO_CLK_ENABLE();
	GYRO_INT1_GPIO_CLK_ENABLE();
	GYRO_INT2_GPIO_CLK_ENABLE();
	GYRO_SPIx_GPIO_CLK_ENABLE();	
	GYRO_SPIx_CLOCK_ENABLE();

	GPIO_InitStructure.Pin = (GYRO_SPIx_SCK_PIN | GYRO_SPIx_MOSI_PIN | GYRO_SPIx_MISO_PIN);	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GYRO_SPIx_AF;
	HAL_GPIO_Init(GYRO_SPIx_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GYRO_CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GYRO_SPIx_AF;	
	HAL_GPIO_Init(GYRO_CS_GPIO_PORT, &GPIO_InitStructure);
		
	GYRO_CS_HIGH();
	
	GPIO_InitStructure.Pin = GYRO_INT1_PIN | GYRO_INT2_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GYRO_INT1_GPIO_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  This function describe spi write and read protocol.
  * @param  TransmitByte
  * @retval ReceivedByte
  */
static uint8_t SPIx_WriteRead(uint8_t TransmitByte)
{
	uint8_t ReceivedByte;

//	/* Enable the SPI */
//	__HAL_SPI_ENABLE(&GYRO_SpiHandle);
//	/* check TXE flag */
//	while ((GYRO_SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

//	/* Write the data */
//	*((__IO uint8_t *)&GYRO_SpiHandle.Instance->DR) = TransmitByte;

//	while ((GYRO_SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
//	ReceivedByte = *((__IO uint8_t *)&GYRO_SpiHandle.Instance->DR);

//	/* Wait BSY flag */
//	while ((GYRO_SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);
//	while ((GYRO_SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

//	/* disable the SPI */
//	__HAL_SPI_DISABLE(&GYRO_SpiHandle);
	
	HAL_SPI_TransmitReceive(&GYRO_SpiHandle,&TransmitByte,&ReceivedByte,1, 1000); 

	return ReceivedByte;
}


/**
  * @brief  This function read L3GD20 ID.
  * @param  None
  * @retval tmp
  */
uint8_t L3GD20_ReadID(void)
{
	uint8_t tmp = 0x00;
	  
	/* Read WHO I AM register */
	GYRO_IO_Read(&tmp, L3GD20_WHO_AM_I_ADDR, 1);
		  
	/* Return the ID */
	return (uint8_t)tmp;
}


/**
  * @brief  This function disable or enable high pass filter.
  * @param  HighPassFilterState
  * @retval None
  */
void L3GD20_FilterCmd(uint8_t HighPassFilterState)
{
	uint8_t tmpreg;
		
	/* Read CTRL_REG5 register */
	GYRO_IO_Read(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
		
	tmpreg &= 0xEF;
		
	tmpreg |= HighPassFilterState;
		
	/* Write value to MEMS CTRL_REG5 register */
	GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
}





/***********************************END OF FILE**********************************/


