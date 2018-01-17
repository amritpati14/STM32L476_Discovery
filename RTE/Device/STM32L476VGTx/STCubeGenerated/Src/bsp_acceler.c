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
	ACCEL_SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; /* MAX 10MHz */
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




/**
  * @brief  Set LSM303C Accelerometer Initialization.
  * @param  InitStruct: Init parameters
  * @retval None
  */
void LSM303C_AccInit(uint16_t InitStruct)
{  
	uint8_t ctrl = 0x00;
	  
	/* Write value to ACC MEMS CTRL_REG1 register */
	ctrl = (uint8_t) InitStruct;
	ACCELERO_IO_Write(LSM303C_CTRL_REG1_A, ctrl);
	  
	/* Write value to ACC MEMS CTRL_REG4 register */
	ctrl = ((uint8_t) (InitStruct >> 8));
	ACCELERO_IO_Write(LSM303C_CTRL_REG4_A, ctrl);
}


/**
  * @brief  Read X, Y & Z Acceleration values 
  * @param  pData: Data out pointer
  * @retval None
  */
void LSM303C_ACCELERO_ReadXYZ(int16_t* pData)
{
	int16_t pnRawData[3];
	uint8_t ctrlx[2]={0,0};
	uint8_t buffer[6];
	uint8_t i = 0;
	uint8_t sensitivity = LSM303C_ACC_SENSITIVITY_2G;
	  
	/* Read the acceleration control register content */
	ctrlx[0] = ACCELERO_IO_Read(LSM303C_CTRL_REG4_A);
	ctrlx[1] = ACCELERO_IO_Read(LSM303C_CTRL_REG5_A);
	  
	/* Read output register X, Y & Z acceleration */
	buffer[0] = ACCELERO_IO_Read(LSM303C_OUT_X_L_A); 
	buffer[1] = ACCELERO_IO_Read(LSM303C_OUT_X_H_A);
	buffer[2] = ACCELERO_IO_Read(LSM303C_OUT_Y_L_A);
	buffer[3] = ACCELERO_IO_Read(LSM303C_OUT_Y_H_A);
	buffer[4] = ACCELERO_IO_Read(LSM303C_OUT_Z_L_A);
	buffer[5] = ACCELERO_IO_Read(LSM303C_OUT_Z_H_A);
	  
	for(i=0; i<3; i++)
	{
		pnRawData[i]=((int16_t)((uint16_t)buffer[2*i+1] << 8) + buffer[2*i]);
	}
	  
	/* Normal mode */
	/* Switch the sensitivity value set in the CRTL4 */
	switch(ctrlx[0] & LSM303C_ACC_FULLSCALE_8G)
	{
		case LSM303C_ACC_FULLSCALE_2G:
			sensitivity = LSM303C_ACC_SENSITIVITY_2G;
		break;
		
		case LSM303C_ACC_FULLSCALE_4G:
			sensitivity = LSM303C_ACC_SENSITIVITY_4G;
		break;
		
		case LSM303C_ACC_FULLSCALE_8G:
			sensitivity = LSM303C_ACC_SENSITIVITY_8G;
		break;
	}
	  
	/* Obtain the ACC value for the three axis */
	for(i=0; i<3; i++)
	{
		pData[i]=(pnRawData[i] * sensitivity);
	}
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


