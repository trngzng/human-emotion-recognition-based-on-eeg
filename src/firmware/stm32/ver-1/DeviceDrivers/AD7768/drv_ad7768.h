/**
 * @file       drv_ad7768_config.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-04-17
 * @author     Giang Phan
 *             
 * @brief      Function to configure the external ADC AD7768 device.
 *             
 * @note       None.  
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRV_AD7768_CONFIG_H
#define __DRV_AD7768_CONFIG_H

/* Includes ----------------------------------------------------------- */
#include "drv_ad7768_reg.h"
#include "common.h"
#include "main.h"

/* Public defines ----------------------------------------------------- */

#define AD7768_GPIO_MODE_INPUT  (0)
#define AD7768_GPIO_MODE_OUTPUT (1)

#define AD7768_STATUS_CHIP_ERR            (1 << 3)
#define AD7768_STATUS_NO_CLOCK_ERR        (1 << 2)
#define AD7768_STATUS_RAM_BIST_PASS       (1 << 1)
#define AD7768_STATUS_RAM_BIST_RUNNING    (1)

#define AD7768_MAX_FREQ_PER_MODE  (6)

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  AD7768_DEVICE,
  AD7768_4_DEVICE,
} DRV_AD7768_DeviceTypeDef;

typedef enum 
{
  LOW_POWER_MODE = 0x00,
  MEDIAN_MODE = 0x2,
  FAST_MODE = 0x3,
} DRV_AD7768_PowerModesTypeDef;

typedef struct {
  uint32_t freq;
  uint32_t dec_rate;
} DRV_AD7768_FreqConfigTypeDef;

typedef struct {
  uint32_t num_of_freqs;
  DRV_AD7768_FreqConfigTypeDef freq_config[AD7768_MAX_FREQ_PER_MODE];
} DRV_AD7768_AvailFreqTypeDef;

typedef enum
{
  DATA_OUT_1_LINE,
  DATA_OUT_2_LINES,       /**< Available on AD7768 only */
  DATA_OUT_4_LINES,       /**< Available on AD7768-4 only */
  DATA_OUT_8_LINES,       /**< Available on AD7768 only */
  NUM_OF_DATA_OUT_LINES,
} DRV_AD7768_DataOutputLinesTypeDef;

typedef struct
{
  DRV_AD7768_DeviceTypeDef device_type;                       /**< Device type (AD7768 or AD7768-4) */
  SPI_HandleTypeDef *hspi;                                    /**< SPI handle for communication with AD7768 */
  GPIO_TypeDef *cs_port;                                      /**< GPIO port for chip select pin */
  uint16_t cs_pin;                                            /**< GPIO pin for chip select pin */
  uint16_t output_datalines;
  uint16_t sampling_freq;                                     /**< Sampling frequency in Hz */
  DRV_AD7768_PowerModesTypeDef power_mode;                    /**< Current power mode of the device */
  DRV_AD7768_AvailFreqTypeDef avail_freq[AD7768_MAX_FREQ_PER_MODE]; /**< Frequency configuration for each decimation rate */
  BoolTypeDef active;                             /**< Flag to indicate if the device is initialized */
  uint8_t device_status;
} DRV_AD7768_HandleTypeDef;

/* Public macros ------------------------------------------------------ */
/**
 * @brief  <macro description>
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
// #define PUBLIC_MACRO(a)  do_something_with(a)

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     spi           Pointer to the SPI_HandleTypeDef structure
 * @param[in]     cs_port       Pointer to the GPIO port for chip select pin
 * @param[in]     cs_pin        GPIO pin for chip select pin
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - DRV_OK: Success
 *  - DRV_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_Init(DRV_AD7768_HandleTypeDef *dev,
                                  SPI_HandleTypeDef *spi,
                                  GPIO_TypeDef *cs_port,
                                  uint16_t cs_pin);

/**
 * @brief  User GPIO Enable
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
BaseStatusTypeDef DRV_AD7768_GPIO_Enable(DRV_AD7768_HandleTypeDef *dev);

/**
 * @brief  User GPIO Enable
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
BaseStatusTypeDef DRV_AD7768_GPIO_SetDirection(DRV_AD7768_HandleTypeDef *dev, uint8_t pin, uint8_t direction);

/**
 * @brief  User GPIO Enable
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
BaseStatusTypeDef DRV_AD7768_GPIO_WritePin(DRV_AD7768_HandleTypeDef *dev, uint8_t pin, BoolTypeDef state);

/**
 * @brief  Initialize the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_HardReset(DRV_AD7768_HandleTypeDef *dev);

/**
 * @brief  Issue a sync pulse to AD7768 to apply pending register configurations.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_Sync(DRV_AD7768_HandleTypeDef *dev);

/**
 * @brief  Check the current status of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_CheckDeviceStatus(DRV_AD7768_HandleTypeDef *dev);

/**
 * @brief  Check the current status of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     mode          Power mode to set
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SetPowerMode(DRV_AD7768_HandleTypeDef *dev, DRV_AD7768_PowerModesTypeDef mode);

/**
 * @brief  Software reset the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SoftReset(DRV_AD7768_HandleTypeDef *dev);


#endif // __DRV_AD7768_CONFIG_H

/* End of file -------------------------------------------------------- */
