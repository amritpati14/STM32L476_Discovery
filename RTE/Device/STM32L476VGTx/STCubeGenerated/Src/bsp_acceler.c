/********************************************************************************
  * file name		:	bsp_acceler.c
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

/*************************************Include***********************************/
#include "bsp_acceler.h"

#define SYNCHRO_WAIT(NB)       for(int i=0; i<NB; i++){__asm("dsb\n");}

static SPI_HandleTypeDef SpiHandle;



static void SPIx_WriteOnly(uint8_t TransmitByte);
static void SPIx_ReadOnly(uint8_t* ReceivedByte);




void ACCELER_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
		
	ACCEL_CS_GPIO_CLK_ENABLE();
	ACCEL_SPIx_GPIO_CLK_ENABLE();	
	ACCEL_SPIx_CLOCK_ENABLE();

	GPIO_InitStructure.Pin = ACCEL_SPIx_SCK_PIN ;	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = ACCEL_SPIx_AF;
	HAL_GPIO_Init(ACCEL_SPIx_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = ACCEL_SPIx_MOSI_PIN ;	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = ACCEL_SPIx_AF;
	HAL_GPIO_Init(ACCEL_SPIx_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = ACCEL_CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ACCEL_CS_GPIO_PORT, &GPIO_InitStructure);
		
//	ACCEL_CS_HIGH();

	ACCEL_CS_LOW();
				
	SpiHandle.Instance = ACCEL_SPIx;

	SpiHandle.Init.Mode = SPI_MODE_MASTER;
//	SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;	
	SpiHandle.Init.Direction = SPI_DIRECTION_1LINE;		
	SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;	
	SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
	SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
	SpiHandle.Init.NSS = SPI_NSS_SOFT;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
	SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial = 7;	

	HAL_SPI_Init(&SpiHandle);	
	
}




uint8_t LSM303C_ReadID(void)
{
	uint8_t change = 0x03,tmp = 0x0c;
	  
	/* Configure the low level interface */

	ACCELER_IO_Init();

	SPI_1LINE_TX(&SpiHandle);
	__HAL_SPI_ENABLE(&SpiHandle);
	
	SPIx_WriteOnly(CTRL_REG4_A);
	SPIx_WriteOnly(change);
	

//	SPIx_ReadOnly(&tmp);
	
	
	SPIx_WriteOnly(CTRL_REG4_A | 0x80);	
	
	SYNCHRO_WAIT(300);
	__HAL_SPI_DISABLE(&SpiHandle);
	SPI_1LINE_RX(&SpiHandle);
	SYNCHRO_WAIT(300);
	  
	SPIx_ReadOnly(&tmp);
	  
	SYNCHRO_WAIT(300);	  
	SPI_1LINE_TX(&SpiHandle);
	__HAL_SPI_ENABLE(&SpiHandle);
	SYNCHRO_WAIT(300);
  
	
	return (uint8_t)tmp;
}


static void SPIx_WriteOnly(uint8_t TransmitByte)
{
	
	/* check TXE flag */
	while ((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

	/* Write the data */
	*((__IO uint8_t *)&SpiHandle.Instance->DR) = TransmitByte;

	/* Wait BSY flag */
	while((SpiHandle.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
	
}


static void SPIx_ReadOnly(uint8_t* ReceivedByte)
{
	
	__HAL_SPI_ENABLE(&SpiHandle);
	__asm("dsb\n");
	__asm("dsb\n");
	__asm("dsb\n");
	__asm("dsb\n");
	__HAL_SPI_DISABLE(&SpiHandle);	
	
	/* check RXNE flag */
	while ((SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

	/* Read the data */
	*ReceivedByte = *((__IO uint8_t *)&SpiHandle.Instance->DR);

	/* Wait BSY flag */
	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);
	
}

//static void SPIx_WriteOnly(uint8_t TransmitByte)
//{
////	__HAL_SPI_DISABLE(&SpiHandle);	
////	SpiHandle.Instance->CR1  |= SPI_CR1_BIDIMODE ;
////	SpiHandle.Instance->CR1  &= ~SPI_CR1_RXONLY;
////	SPI_1LINE_TX(&SpiHandle);
//	
//	/* Enable the SPI */
////	__HAL_SPI_ENABLE(&SpiHandle);
//	
//	/* check TXE flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

//	/* Write the data */
//	*((__IO uint8_t *)&SpiHandle.Instance->DR) = TransmitByte;

//	/* Wait BSY flag */
////	while ((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);
//	while((SpiHandle.Instance->SR & SPI_FLAG_FTLVL) != SPI_FTLVL_EMPTY);
//	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

//	/* disable the SPI */
////	__HAL_SPI_DISABLE(&SpiHandle);	
//	
//}

//static void SPIx_ReadOnly(uint8_t* ReceivedByte)
//{
////	__HAL_SPI_DISABLE(&SpiHandle);
////	SpiHandle.Instance->CR1  |= SPI_CR1_BIDIMODE | SPI_CR1_RXONLY;
////	SPI_1LINE_RX(&SpiHandle);
//	
//	/* Enable the SPI */
////	__HAL_SPI_ENABLE(&SpiHandle);
//	
////	__HAL_SPI_ENABLE(&SpiHandle);
////	__asm("dsb\n");
////	__asm("dsb\n");
////	__asm("dsb\n");
////	__asm("dsb\n");
////	__HAL_SPI_DISABLE(&SpiHandle);	
//	

//	/* check RXNE flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

//	/* Read the data */
//	*ReceivedByte = *((__IO uint8_t *)&SpiHandle.Instance->DR);

//	/* Wait BSY flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
////	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

//	/* disable the SPI */
////	__HAL_SPI_DISABLE(&SpiHandle);	
//	
//}



//static void SPIx_WriteOnly(uint8_t TransmitByte)
//{
//	/* disable the SPI */	
//	__HAL_SPI_DISABLE(&SpiHandle);	
//	
//	SpiHandle.Instance->CR1  |= SPI_CR1_BIDIMODE ;
//	SPI_1LINE_TX(&SpiHandle);
//	
//	/* Enable the SPI */
//	__HAL_SPI_ENABLE(&SpiHandle);
//	
//	
//	/* Write the data */
//	*((__IO uint8_t *)&SpiHandle.Instance->DR) = TransmitByte;	
//		
//	/* check TXE flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

//	/* Wait BSY flag */
////	while ((SpiHandle.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);
//	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

//	/* disable the SPI */
//	__HAL_SPI_DISABLE(&SpiHandle);	
//	
//}

//static void SPIx_ReadOnly(uint8_t* ReceivedByte)
//{
//	/* disable the SPI */	
//	__HAL_SPI_DISABLE(&SpiHandle);
//	
//	SpiHandle.Instance->CR1  |= SPI_CR1_BIDIMODE ;
//	SPI_1LINE_RX(&SpiHandle);
//	
//	/* Enable the SPI */
//	__HAL_SPI_ENABLE(&SpiHandle);	

//	/* check RXNE flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

//	/* Read the data */
//	*ReceivedByte = *((__IO uint8_t *)&SpiHandle.Instance->DR);

//	/* Wait BSY flag */
//	while ((SpiHandle.Instance->SR & SPI_FLAG_RXNE) == SPI_FLAG_RXNE);
////	while ((SpiHandle.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

//	/* disable the SPI */
//	__HAL_SPI_DISABLE(&SpiHandle);	
//	
//}


/***********************************END OF FILE**********************************/


