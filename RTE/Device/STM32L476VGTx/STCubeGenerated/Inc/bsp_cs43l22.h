/********************************************************************************
  * file name		:	bsp_cs43l22.h
  * author			:
  * date			:    
  * description	    :         
********************************************************************************/

#ifndef _BSP_CS43L22_H
#define _BSP_CS43L22_H



#ifdef __cplusplus
 extern "C" {
#endif

/*************************************INCLUDE************************************/

#include "stm32l4xx_hal.h"	

/*************************************REGISTER***********************************/	 
	

/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
#define OUTPUT_DEVICE_AUTO            4

/* AUDIO FREQUENCY */
#define AUDIO_FREQUENCY_192K          ((uint32_t)192000)
#define AUDIO_FREQUENCY_96K           ((uint32_t)96000)
#define AUDIO_FREQUENCY_48K           ((uint32_t)48000)
#define AUDIO_FREQUENCY_44K           ((uint32_t)44100)
#define AUDIO_FREQUENCY_32K           ((uint32_t)32000)
#define AUDIO_FREQUENCY_22K           ((uint32_t)22050)
#define AUDIO_FREQUENCY_16K           ((uint32_t)16000)
#define AUDIO_FREQUENCY_11K           ((uint32_t)11025)
#define AUDIO_FREQUENCY_8K            ((uint32_t)8000)  

	
/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0





/** CS43l22 Registers  ***/
#define   CS43L22_REG_ID                  0x01
#define   CS43L22_REG_POWER_CTL1          0x02
#define   CS43L22_REG_POWER_CTL2          0x04
#define   CS43L22_REG_CLOCKING_CTL        0x05
#define   CS43L22_REG_INTERFACE_CTL1      0x06
#define   CS43L22_REG_INTERFACE_CTL2      0x07
#define   CS43L22_REG_PASSTHR_A_SELECT    0x08
#define   CS43L22_REG_PASSTHR_B_SELECT    0x09
#define   CS43L22_REG_ANALOG_ZC_SR_SETT   0x0A
#define   CS43L22_REG_PASSTHR_GANG_CTL    0x0C
#define   CS43L22_REG_PLAYBACK_CTL1       0x0D
#define   CS43L22_REG_MISC_CTL            0x0E
#define   CS43L22_REG_PLAYBACK_CTL2       0x0F
#define   CS43L22_REG_PASSTHR_A_VOL       0x14
#define   CS43L22_REG_PASSTHR_B_VOL       0x15
#define   CS43L22_REG_PCMA_VOL            0x1A
#define   CS43L22_REG_PCMB_VOL            0x1B
#define   CS43L22_REG_BEEP_FREQ_ON_TIME   0x1C
#define   CS43L22_REG_BEEP_VOL_OFF_TIME   0x1D
#define   CS43L22_REG_BEEP_TONE_CFG       0x1E
#define   CS43L22_REG_TONE_CTL            0x1F
#define   CS43L22_REG_MASTER_A_VOL        0x20
#define   CS43L22_REG_MASTER_B_VOL        0x21
#define   CS43L22_REG_HEADPHONE_A_VOL     0x22
#define   CS43L22_REG_HEADPHONE_B_VOL     0x23
#define   CS43L22_REG_SPEAKER_A_VOL       0x24
#define   CS43L22_REG_SPEAKER_B_VOL       0x25
#define   CS43L22_REG_CH_MIXER_SWAP       0x26
#define   CS43L22_REG_LIMIT_CTL1          0x27
#define   CS43L22_REG_LIMIT_CTL2          0x28
#define   CS43L22_REG_LIMIT_ATTACK_RATE   0x29
#define   CS43L22_REG_OVF_CLK_STATUS      0x2E
#define   CS43L22_REG_BATT_COMPENSATION   0x2F
#define   CS43L22_REG_VP_BATTERY_LEVEL    0x30
#define   CS43L22_REG_SPEAKER_STATUS      0x31
#define   CS43L22_REG_TEMPMONITOR_CTL     0x32
#define   CS43L22_REG_THERMAL_FOLDBACK    0x33
#define   CS43L22_REG_CHARGE_PUMP_FREQ    0x34	 


/* Audio codec I2C address */
#define AUDIO_I2C_ADDRESS                       ((uint16_t) 0x94)

/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* Set 0x90112626 value to reach 100 KHz speed (Rise time = 640ns, Fall time = 20ns) */
#define AUDIO_I2C1_TIMING                  0x90112626

#define   I21_Timeout   3000


#define CODEC_STANDARD                0x04

/**************************************MACROS************************************/	

/**  CS43L22 ID
  * @brief Chip ID Register: Chip I.D. and Revision Register
  *  Read only register
  *  Default value: 0x01
  *  [7:3] CHIPID[4:0]: I.D. code for the CS43L22.
  *        Default value: 11100b
  *  [2:0] REVID[2:0]: CS43L22 revision level.
  *        Default value: 
  *        000 - Rev A0
  *        001 - Rev A1
  *        010 - Rev B0
  *        011 - Rev B1
  */
#define  CS43L22_ID                       0xE0


/* Definition for CS43L22 I2C Pins */
#define CS43L22_I2C_GPIO_PORT                GPIOB

#define CS43L22_I2C_SCL_GPIO_PIN             GPIO_PIN_6
#define CS43L22_I2C_SDA_GPIO_PIN             GPIO_PIN_7

#define CS43L22_I2C_AF                       GPIO_AF4_I2C1

/* Definition for I2C1 clock resources */
#define CS43L22_I2C                          I2C1
#define CS43L22_I2C_CLK_ENABLE()             __HAL_RCC_I2C1_CLK_ENABLE()
#define CS43L22_I2C_CLK_DISABLE()            __HAL_RCC_I2C1_CLK_DISABLE()

#define CS43L22_I2C_FORCE_RESET()            __HAL_RCC_I2C1_FORCE_RESET()
#define CS43L22_I2C_RELEASE_RESET()          __HAL_RCC_I2C1_RELEASE_RESET()


/* Definition for I2C1's NVIC */
#define CS43L22_I2C_EV_IRQn                  I2C1_EV_IRQn
#define CS43L22_I2C_EV_IRQHandler            I2C1_EV_IRQHandler

#define CS43L22_I2C_ER_IRQn                  I2C1_ER_IRQn
#define CS43L22_I2C_ER_IRQHandler            I2C1_ER_IRQHandler


/* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_PORT                GPIOE
#define AUDIO_RESET_GPIO_PIN                 GPIO_PIN_3

#define AUDIO_RESET_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()
#define AUDIO_RESET_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOE_CLK_DISABLE()


#define AUDIO_RESET_HIGH()                   HAL_GPIO_WritePin(AUDIO_RESET_GPIO_PORT, AUDIO_RESET_GPIO_PIN, GPIO_PIN_SET)	
#define AUDIO_RESET_LOW()                    HAL_GPIO_WritePin(AUDIO_RESET_GPIO_PORT, AUDIO_RESET_GPIO_PIN, GPIO_PIN_RESET)




#define VOLUME_CONVERT(Volume)    (((Volume) > 100)? 100:((uint8_t)(((Volume) * 255) / 100)))  
	 
/*************************************FUNCTION***********************************/	

extern uint8_t DmaRecHalfBuffCplt,DmaRecBuffCplt;

extern void Error_Handler (void);	

void CS43L22_I2C_Init(void);

uint8_t AUDIO_IO_Read(uint8_t Addr, uint8_t Reg);

uint32_t AUDIO_ReadID(uint16_t DeviceAddr);


void DFSDM_Init(void);

void Playback_Init(void);

uint32_t CS43L22_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);

void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);



uint32_t CS43L22_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size);

uint32_t cs43l22_SetVolume(uint16_t DeviceAddr, uint8_t Volume);


uint32_t cs43l22_SetMute(uint16_t DeviceAddr, uint32_t Cmd);


#ifdef __cplusplus
}
#endif

#endif /* _BSP_CS43L22_H */




/***********************************END OF FILE**********************************/

