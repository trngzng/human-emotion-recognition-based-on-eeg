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

#define AD7768_NUM_OF_FREQ_PER_POWER_MODE (6)
#define AD7768_NUM_OF_POWER_MODES         (3)

#define AD7768_WIDEBAND_FILTER_TYPE       (0)
#define AD7768_SINC5_FILTER_TYPE          (1)

#define AD7768_CHANNEL_IN_MODE_A          (0)
#define AD7768_CHANNEL_IN_MODE_B          (1)

#define AD7768_SAMPLE_SIZE                (32) // Sample size in bits

#define AD7768_MAX_DCLK_DIV			          (8)
#define AD7768_MAX_CHANNEL                (8) // Maximum number of channels

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
  DRV_AD7768_FreqConfigTypeDef freq_config[AD7768_NUM_OF_FREQ_PER_POWER_MODE];
} DRV_AD7768_AvailFreqTypeDef;

typedef struct
{
  uint8_t filter_type;  /**< Filter type (Wideband or Sinc5) */
  uint32_t dec_rate;    /**< Decimation rate */
  uint8_t mode;
  BoolTypeDef active;   /**< Flag to indicate if the filter is enabled */
} DRV_AD7768_ChannelTypeDef;

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
  DRV_AD7768_DeviceTypeDef device_type;                               /**< Device type (AD7768 or AD7768-4) */
  SPI_HandleTypeDef *config_spi;                                      /**< SPI handle for communication with AD7768 */
  SPI_HandleTypeDef *data_spi;                                        /**< SPI handle for received data */
  GPIO_TypeDef *cs_port;                                              /**< GPIO port for chip select pin */
  uint16_t cs_pin;                                                    /**< GPIO pin for chip select pin */
  uint8_t mclk;                                                       /**< MCLK frequency in MHz */
  uint8_t output_datalines;                                           /**< Number of data output lines (1, 2, 4, or 8) */
  DRV_AD7768_ChannelTypeDef channel[AD7768_MAX_CHANNEL];              /**< Array to store channel activity status */
  uint16_t sampling_freq;                                             /**< Sampling frequency in Hz */
  DRV_AD7768_PowerModesTypeDef power_mode;                            /**< Current power mode of the device */
  DRV_AD7768_AvailFreqTypeDef avail_freq[AD7768_NUM_OF_POWER_MODES];  /**< Frequency configuration for each power mode */
  BoolTypeDef active;                                                 /**< Flag to indicate if the device is initialized */
  uint8_t device_status;
  uint32_t gain[AD7768_MAX_CHANNEL];                                  /**< Gain for each channel */
  uint32_t offset[AD7768_MAX_CHANNEL];                                /**< Offset for each channel */
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
 * @param[in]     config_spi    Pointer to the SPI_HandleTypeDef structure that configures the device
 * @param[in]     data_spi      Pointer to the SPI_HandleTypeDef structure that receives data
 * @param[in]     cs_port       Pointer to the GPIO port for chip select pin
 * @param[in]     cs_pin        GPIO pin for chip select pin
 * @param[in]     master_clock  Master clock of the device
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
                                  SPI_HandleTypeDef *config_spi,
                                  SPI_HandleTypeDef *data_spi,
                                  GPIO_TypeDef *cs_port,
                                  uint16_t cs_pin,
                                  uint8_t master_clock,
                                  DRV_AD7768_DeviceTypeDef device_type,
                                  uint8_t datalines);

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

/**
 * @brief  Determine the output sampling rate (DCLK frequency) of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     freq          Desired output sampling rate in Hz
 * @param[in]     mode          Mode of the device (0 for mode A, 1 for mode B)
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SetSamplingRate(DRV_AD7768_HandleTypeDef *dev, uint32_t freq, uint8_t mode);

/**
 * @brief  Enable the channel(s) of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     channel       Channel number to enable (0-7 for AD7768, 0-3 for AD7768-4)
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_EnableChannel(DRV_AD7768_HandleTypeDef *dev, uint8_t channel);

/**
 * @brief  Disable the channel(s) of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     channel       Channel number to disable (0-7 for AD7768, 0-3 for AD7768-4)
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_DisableChannel(DRV_AD7768_HandleTypeDef *dev, uint8_t channel);

/**
 * @brief  Set the type of the built-in filter (wideband or sinc).
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     filter_type   Filter type to set (0 for wideband, 1 for sinc5)
 * @param[in]     mode          Mode of the device (0 for mode A, 1 for mode B)
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SetFilterType(DRV_AD7768_HandleTypeDef *dev, uint8_t filter_type, uint8_t mode);

/**
 * @brief  Set the type of the built-in filter (wideband or sinc).
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     buffer        Pointer to the buffer to store received data
 * @param[in]     size          Size of the buffer in bytes
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SetReceivedBuffer(DRV_AD7768_HandleTypeDef *dev, uint8_t *buffer, uint32_t size);

/**
 * @brief  Get the current gain of one channel of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     ch            The channel number to set the gain from (0-7 for AD7768, 0-3 for AD7768-4)
 * @param[out]    gain          The desired gain value
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef DRV_AD7768_SetGain(DRV_AD7768_HandleTypeDef *dev, uint8_t ch, uint32_t gain);

/**
 * @brief  Get the current gain of one channel of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     ch            The channel number to get the gain from (0-7 for AD7768, 0-3 for AD7768-4)
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - The current gain value of the specified channel
 */
uint32_t DRV_AD7768_GetGain(DRV_AD7768_HandleTypeDef *dev, uint8_t ch);

#endif // __DRV_AD7768_CONFIG_H

/* End of file -------------------------------------------------------- */
