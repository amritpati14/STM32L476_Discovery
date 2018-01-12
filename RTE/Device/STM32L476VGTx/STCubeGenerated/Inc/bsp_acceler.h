/********************************************************************************
  * file name		:	bsp_acceler.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_ACCELER_H
#define _BSP_ACCELER_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	 

/*************************************REGISTER***********************************/	 
/* Acceleration Registers */
#define LSM303C_WHO_AM_I_ADDR             0x0F  /* device identification register */
#define LSM303C_ACT_THS_A                 0x1E
#define LSM303C_ACT_DUR_A                 0x1F
#define LSM303C_CTRL_REG1_A               0x20  /* Control register 1 acceleration */
#define LSM303C_CTRL_REG2_A               0x21  /* Control register 2 acceleration */
#define LSM303C_CTRL_REG3_A               0x22  /* Control register 3 acceleration */
#define LSM303C_CTRL_REG4_A               0x23  /* Control register 4 acceleration */
#define LSM303C_CTRL_REG5_A               0x24  /* Control register 5 acceleration */
#define LSM303C_CTRL_REG6_A               0x25  /* Control register 6 acceleration */
#define LSM303C_CTRL_REG7_A               0x26  /* Control register 6 acceleration */   
#define LSM303C_STATUS_REG_A              0x27  /* Status register acceleration */
#define LSM303C_OUT_X_L_A                 0x28  /* Output Register X acceleration */
#define LSM303C_OUT_X_H_A                 0x29  /* Output Register X acceleration */
#define LSM303C_OUT_Y_L_A                 0x2A  /* Output Register Y acceleration */
#define LSM303C_OUT_Y_H_A                 0x2B  /* Output Register Y acceleration */
#define LSM303C_OUT_Z_L_A                 0x2C  /* Output Register Z acceleration */
#define LSM303C_OUT_Z_H_A                 0x2D  /* Output Register Z acceleration */ 
#define LSM303C_FIFO_CTRL                 0x2E  /* Fifo control Register acceleration */
#define LSM303C_FIFO_SRC                  0x2F  /* Fifo src Register acceleration */

#define LSM303C_IG_CFG1_A                 0x30  /* Interrupt 1 configuration Register acceleration */
#define LSM303C_IG_SRC1_A                 0x31  /* Interrupt 1 source Register acceleration */
#define LSM303C_IG_THS_X1_A               0x32
#define LSM303C_IG_THS_Y1_A               0x33
#define LSM303C_IG_THS_Z1_A               0x34
   
#define LSM303C_IG_DUR1_A                 0x32
#define LSM303C_INT1_DURATION_A           0x33  /* Interrupt 1 DURATION register acceleration */

#define LSM303C_INT2_CFG_A                0x34  /* Interrupt 2 configuration Register acceleration */
#define LSM303C_INT2_SOURCE_A             0x35  /* Interrupt 2 source Register acceleration */
#define LSM303C_INT2_THS_A                0x36  /* Interrupt 2 Threshold register acceleration */
#define LSM303C_INT2_DURATION_A           0x37  /* Interrupt 2 DURATION register acceleration */

#define LSM303C_CLICK_CFG_A               0x38  /* Click configuration Register acceleration */
#define LSM303C_CLICK_SOURCE_A            0x39  /* Click 2 source Register acceleration */
#define LSM303C_CLICK_THS_A               0x3A  /* Click 2 Threshold register acceleration */

#define LSM303C_TIME_LIMIT_A              0x3B  /* Time Limit Register acceleration */
#define LSM303C_TIME_LATENCY_A            0x3C  /* Time Latency Register acceleration */
#define LSM303C_TIME_WINDOW_A             0x3D  /* Time window register acceleration */

/* Magnetic field Registers */
#define LSM303C_CTRL_REG1_M               0x20  /* Magnetic control register 1 */
#define LSM303C_CTRL_REG2_M               0x21  /* Magnetic control register 2 */
#define LSM303C_CTRL_REG3_M               0x22  /* Magnetic control register 3 */
#define LSM303C_CTRL_REG4_M               0x23  /* Magnetic control register 4 */
#define LSM303C_CTRL_REG5_M               0x24  /* Magnetic control register 5 */   

#define LSM303C_STATUS_REG_M              0x27  /* Magnetic status register M  */

#define LSM303C_OUT_X_L_M                 0x28  /* Output Register X magnetic field */
#define LSM303C_OUT_X_H_M                 0x29  /* Output Register X magnetic field */
#define LSM303C_OUT_Y_L_M                 0x2A  /* Output Register Y magnetic field */
#define LSM303C_OUT_Y_H_M                 0x2B  /* Output Register Y magnetic field */
#define LSM303C_OUT_Z_L_M                 0x2C  /* Output Register Z magnetic field */ 
#define LSM303C_OUT_Z_H_M                 0x2D  /* Output Register Z magnetic field */

#define LSM303C_TEMP_OUT_L_M              0x2E  /* Temperature Register magnetic field */
#define LSM303C_TEMP_OUT_H_M              0x2F  /* Temperature Register magnetic field */

#define LSM303C_INT_CFG_M                 0x30  /* Axis interrupt configuration        */
#define LSM303C_INT_SRC_M                 0x31  /* Axis interrupt source               */
#define LSM303C_INT_THS_L_M               0x32  /* Interrupt threshold L               */
#define LSM303C_INT_THS_H_M               0x33  /* Interrupt threshold M               */ 	 
	 
	 
#define LMS303C_ACC_ID                      ((uint8_t)0x41)
#define LMS303C_MAG_ID                      ((uint8_t)0x3D)



/* Acc_OutPut_DataRate_Selection */
#define LSM303C_ACC_ODR_BITPOSITION         ((uint8_t)0x70)  /*!< Output Data Rate bit position */
#define LSM303C_ACC_ODR_OFF                 ((uint8_t)0x00)  /*!< Output Data Rate powerdown */
#define LSM303C_ACC_ODR_10_HZ               ((uint8_t)0x10)  /*!< Output Data Rate = 10 Hz */
#define LSM303C_ACC_ODR_50_HZ               ((uint8_t)0x20)  /*!< Output Data Rate = 50 Hz */
#define LSM303C_ACC_ODR_100_HZ              ((uint8_t)0x30)  /*!< Output Data Rate = 100 Hz */
#define LSM303C_ACC_ODR_200_HZ              ((uint8_t)0x40)  /*!< Output Data Rate = 200 Hz */
#define LSM303C_ACC_ODR_400_HZ              ((uint8_t)0x50)  /*!< Output Data Rate = 400 Hz */
#define LSM303C_ACC_ODR_800_HZ              ((uint8_t)0x60)  /*!< Output Data Rate = 800 Hz */


/* Acc_Axes_Selection */
#define LSM303C_ACC_X_ENABLE                ((uint8_t)0x01)
#define LSM303C_ACC_Y_ENABLE                ((uint8_t)0x02)
#define LSM303C_ACC_Z_ENABLE                ((uint8_t)0x04)
#define LSM303C_ACC_AXES_ENABLE             ((uint8_t)0x07)
#define LSM303C_ACC_AXES_DISABLE            ((uint8_t)0x00)


/* Acc_High_Resolution */
#define LSM303C_ACC_HR_ENABLE               ((uint8_t)0x80)
#define LSM303C_ACC_HR_DISABLE              ((uint8_t)0x00)


/* Acc_Full_Scale_Selection */
#define LSM303C_ACC_SENSITIVITY_2G     ((uint8_t)1)  /*!< accelerometer sensitivity with 2 g full scale [mg/LSB] */
#define LSM303C_ACC_SENSITIVITY_4G     ((uint8_t)2)  /*!< accelerometer sensitivity with 4 g full scale [mg/LSB] */
#define LSM303C_ACC_SENSITIVITY_8G     ((uint8_t)4)  /*!< accelerometer sensitivity with 8 g full scale [mg/LSB] */
#define LSM303C_ACC_SENSITIVITY_16G    ((uint8_t)12) /*!< accelerometer sensitivity with 12 g full scale [mg/LSB] */

/* Acc_Block_Data_Update */
#define LSM303C_ACC_BDU_CONTINUOUS   ((uint8_t)0x00) /*!< Continuos Update */
#define LSM303C_ACC_BDU_MSBLSB       ((uint8_t)0x08) /*!< Single Update: output registers not updated until MSB and LSB reading */

/* Acc_Endian_Data_selection */
#define LSM303C_ACC_BLE_LSB                 ((uint8_t)0x00) /*!< Little Endian: data LSB @ lower address */
#define LSM303C_ACC_BLE_MSB                 ((uint8_t)0x40) /*!< Big Endian: data MSB @ lower address */

/* Acc_High_Pass_Filter_Mode */
#define LSM303C_ACC_HPM_REF_SIGNAL          ((uint8_t)0x08)
#define LSM303C_ACC_HPM_NORMAL_MODE         ((uint8_t)0x00)

/* Acc_High_Pass_CUT OFF_Frequency */
#define LSM303C_ACC_DFC1_ODRDIV50       ((uint8_t)0x00)
#define LSM303C_ACC_DFC1_ODRDIV100      ((uint8_t)0x20)
#define LSM303C_ACC_DFC1_ODRDIV9        ((uint8_t)0x40)
#define LSM303C_ACC_DFC1_ODRDIV400      ((uint8_t)0x60)

/* Acc_High_Pass_Filter_status */
#define LSM303C_ACC_HPF_DISABLE         ((uint8_t)0x00)
#define LSM303C_ACC_HPF_ENABLE          ((uint8_t)0x08)

/* Acc_High_Pass_Filter_Click_status */
#define LSM303C_ACC_HPF_CLICK_DISABLE   ((uint8_t)0x00)
#define LSM303C_ACC_HPF_CLICK_ENABLE    ((uint8_t)0x04)

/* Acc_High_Pass_Filter_HPI2S_status */
#define LSM303C_ACC_HPI2S_INT1_DISABLE  ((uint8_t)0x00)
#define LSM303C_ACC_HPI2S_INT1_ENABLE	  ((uint8_t)0x01)
#define LSM303C_ACC_HPI2S_INT2_DISABLE  ((uint8_t)0x00)
#define LSM303C_ACC_HPI2S_INT2_ENABLE   ((uint8_t)0x02)

/* Acc_Interrupt1_Configuration_definition */
#define LSM303C_IT1_CLICK               ((uint8_t)0x80)
#define LSM303C_IT1_AOI1                ((uint8_t)0x40)
#define LSM303C_IT1_AOI2                ((uint8_t)0x20)
#define LSM303C_IT1_DRY1                ((uint8_t)0x10)
#define LSM303C_IT1_DRY2                ((uint8_t)0x08)
#define LSM303C_IT1_WTM                 ((uint8_t)0x04)
#define LSM303C_IT1_OVERRUN             ((uint8_t)0x02)

/* Acc_Interrupt2_Configuration_definition */
#define LSM303C_IT2_CLICK               ((uint8_t)0x80)
#define LSM303C_IT2_INT1                ((uint8_t)0x40)
#define LSM303C_IT2_INT2                ((uint8_t)0x20)
#define LSM303C_IT2_BOOT                ((uint8_t)0x10)
#define LSM303C_IT2_ACT                 ((uint8_t)0x08)
#define LSM303C_IT2_HLACTIVE            ((uint8_t)0x02)

/* Acc_INT_Combination_Status */
#define LSM303C_OR_COMBINATION          ((uint8_t)0x00)  /*!< OR combination of enabled IRQs */
#define LSM303C_AND_COMBINATION         ((uint8_t)0x80)  /*!< AND combination of enabled IRQs */
#define LSM303C_MOV_RECOGNITION         ((uint8_t)0x40)  /*!< 6D movement recognition */
#define LSM303C_POS_RECOGNITION         ((uint8_t)0xC0)  /*!< 6D position recognition */

/* Acc_INT_Axes */
#define LSM303C_Z_HIGH                  ((uint8_t)0x20)  /*!< Z High enabled IRQs */
#define LSM303C_Z_LOW                   ((uint8_t)0x10)  /*!< Z low enabled IRQs */
#define LSM303C_Y_HIGH                  ((uint8_t)0x08)  /*!< Y High enabled IRQs */
#define LSM303C_Y_LOW                   ((uint8_t)0x04)  /*!< Y low enabled IRQs */
#define LSM303C_X_HIGH                  ((uint8_t)0x02)  /*!< X High enabled IRQs */
#define LSM303C_X_LOW                   ((uint8_t)0x01)  /*!< X low enabled IRQs */

/* Acc_INT_Click */
#define LSM303C_Z_DOUBLE_CLICK          ((uint8_t)0x20)  /*!< Z double click IRQs */
#define LSM303C_Z_SINGLE_CLICK          ((uint8_t)0x10)  /*!< Z single click IRQs */
#define LSM303C_Y_DOUBLE_CLICK          ((uint8_t)0x08)  /*!< Y double click IRQs */
#define LSM303C_Y_SINGLE_CLICK          ((uint8_t)0x04)  /*!< Y single click IRQs */
#define LSM303C_X_DOUBLE_CLICK          ((uint8_t)0x02)  /*!< X double click IRQs */
#define LSM303C_X_SINGLE_CLICK          ((uint8_t)0x01)  /*!< X single click IRQs */

/* Acc_INT1_Interrupt_status */
#define LSM303C_INT1INTERRUPT_DISABLE   ((uint8_t)0x00)
#define LSM303C_INT1INTERRUPT_ENABLE    ((uint8_t)0x80)


/* Acc_INT1_Interrupt_ActiveEdge */
#define LSM303C_INT1INTERRUPT_LOW_EDGE  ((uint8_t)0x20)
#define LSM303C_INT1INTERRUPT_HIGH_EDGE ((uint8_t)0x00)



/* Mag_Temperature_Sensor */
#define LSM303C_MAG_TEMPSENSOR_ENABLE        ((uint8_t) 0x80)   /*!< Temp sensor Enable */
#define LSM303C_MAG_TEMPSENSOR_DISABLE       ((uint8_t) 0x00)   /*!< Temp sensor Disable */

/* Mag_XY-axis_Operating_Mode */
#define LSM303C_MAG_OM_XY_LOWPOWER           ((uint8_t) 0x00 << 5)
#define LSM303C_MAG_OM_XY_MEDIUM             ((uint8_t) 0x01 << 5)
#define LSM303C_MAG_OM_XY_HIGH               ((uint8_t) 0x02 << 5)
#define LSM303C_MAG_OM_XY_ULTRAHIGH          ((uint8_t) 0x03 << 5)

/* Mag_Data_Rate */
#define LSM303C_MAG_ODR_0_625_HZ             ((uint8_t) 0x00 << 2)  /*!< Output Data Rate = 0.625 Hz */
#define LSM303C_MAG_ODR_1_25_HZ              ((uint8_t) 0x01 << 2)  /*!< Output Data Rate = 1.25 Hz  */
#define LSM303C_MAG_ODR_2_5_HZ               ((uint8_t) 0x02 << 2)  /*!< Output Data Rate = 2.5 Hz   */
#define LSM303C_MAG_ODR_5_0_HZ               ((uint8_t) 0x03 << 2)  /*!< Output Data Rate = 5.0 Hz   */
#define LSM303C_MAG_ODR_10_HZ                ((uint8_t) 0x04 << 2)  /*!< Output Data Rate = 10 Hz    */
#define LSM303C_MAG_ODR_20_HZ                ((uint8_t) 0x05 << 2)  /*!< Output Data Rate = 20 Hz    */
#define LSM303C_MAG_ODR_40_HZ                ((uint8_t) 0x06 << 2)  /*!< Output Data Rate = 40 Hz    */
#define LSM303C_MAG_ODR_80_HZ                ((uint8_t) 0x07 << 2)  /*!< Output Data Rate = 80 Hz    */

/* Mag_Operation_Mode  */
#define LSM303C_MAG_SELECTION_MODE           ((uint8_t) 0x03)
#define LSM303C_MAG_CONTINUOUS_MODE          ((uint8_t) 0x00)
#define LSM303C_MAG_SINGLE_MODE              ((uint8_t) 0x01)
#define LSM303C_MAG_POWERDOWN1_MODE          ((uint8_t) 0x02)
#define LSM303C_MAG_POWERDOWN2_MODE          ((uint8_t) 0x03)

/* Mag_Big_little-endian_selection */
#define LSM303C_MAG_BLE_LSB                  ((uint8_t) 0x00)
#define LSM303C_MAG_BLE_MSB                  ((uint8_t) 0x02)

/* Mag_Bloc_update_magnetic_data */
#define LSM303C_MAG_BDU_CONTINUOUS           ((uint8_t) 0x00)
#define LSM303C_MAG_BDU_MSBLSB              ((uint8_t) 0x40)

/* Magnetometer_Sensitivity */
#define LSM303C_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303C_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */

/**************************************MACRO*************************************/	

/* SPI SCLK,MOSI,MISO GPIO CONFIG */
#define ACCEL_SPIx                               SPI2
	 
#define ACCEL_SPIx_GPIO_PORT                     GPIOD                      /* GPIOD */
#define ACCEL_SPIx_SCK_PIN                       GPIO_PIN_1                 /* PD.01*/
#define ACCEL_SPIx_MISO_PIN                      GPIO_PIN_3                 /* PD.03 */
#define ACCEL_SPIx_MOSI_PIN                      GPIO_PIN_4                 /* PD.04 */
#define ACCEL_SPIx_AF                            GPIO_AF5_SPI2
#define ACCEL_SPIx_CLOCK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define ACCEL_SPIx_CLOCK_DISABLE()               __HAL_RCC_SPI2_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define ACCEL_SPIx_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()
#define ACCEL_SPIx_GPIO_FORCE_RESET()            __HAL_RCC_SPI2_FORCE_RESET()
#define ACCEL_SPIx_GPIO_RELEASE_RESET()          __HAL_RCC_SPI2_RELEASE_RESET()

/* ACCELER CS */
#define ACCEL_CS_GPIO_PORT                       GPIOE                       /* GPIOE */
#define ACCEL_CS_PIN                             GPIO_PIN_0                  /* PE.00 */	 
#define ACCEL_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCEL_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOE_CLK_DISABLE()

#define ACCEL_CS_LOW()                           HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_RESET)
#define ACCEL_CS_HIGH()                          HAL_GPIO_WritePin(ACCEL_CS_GPIO_PORT, ACCEL_CS_PIN, GPIO_PIN_SET)	

	 
/* MAGNETIC CS */
#define MAGNE_CS_GPIO_PORT                       GPIOC                       /* GPIOC */
#define MAGNE_CS_PIN                             GPIO_PIN_0                  /* PC.00 */	 
#define MAGNE_CS_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define MAGNE_CS_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOC_CLK_DISABLE()

#define MAGNE_CS_LOW()                           HAL_GPIO_WritePin(MAGNE_CS_GPIO_PORT, MAGNE_CS_PIN, GPIO_PIN_RESET)
#define MAGNE_CS_HIGH()                          HAL_GPIO_WritePin(MAGNE_CS_GPIO_PORT, MAGNE_CS_PIN, GPIO_PIN_SET)		 
 



#define __SPI_DIRECTION_1LINE_TX(__HANDLE__) do{\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             }while(0);

#define __SPI_DIRECTION_1LINE_RX(__HANDLE__) do {\
                                             CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_RXONLY | SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE);\
                                             SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_BIDIMODE);\
                                             } while(0);



/*************************************FUNCTION***********************************/	

extern void Error_Handler (void);	

void BSP_ACCELER_Init(void);
											 
uint8_t ACCELERO_IO_Read(uint8_t RegisterAddr);
											 
void ACCELERO_IO_Write(uint8_t RegisterAddr, uint8_t Value);											 

uint8_t LSM303C_ACCELERO_ReadID(void); 
											 
											 
uint8_t MAGNETIC_IO_Read(uint8_t RegisterAddr);

void MAGNETIC_IO_Write(uint8_t RegisterAddr, uint8_t Value);											 
											 
uint8_t LSM303C_MAGNETIC_ReadID(void);




#ifdef __cplusplus
}
#endif

#endif /* _BSP_ACCELER_H */




/***********************************END OF FILE**********************************/

