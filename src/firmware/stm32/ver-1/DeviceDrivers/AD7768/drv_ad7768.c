/**
 * @file       drv_ad7768.c
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

/* Includes ----------------------------------------------------------- */
#include "drv_ad7768.h"
#include "stdio.h"
#include "common.h"

/* Private defines ---------------------------------------------------- */

#define AD7768_POWER_MODE_MSK  (0x03 << 4) /*!< Mask for POWER_MODE bits [5:4] */
#define AD7768_MCLK_DIV_MSK    (0x03 << 0) /*!< Mask for MCLK_DIV bits [1:0] */

#define AD7768_SPI_RESET_MSK	(0x03 << 0) /*!< Mask for SPI_RESET bits [1:0] */
#define AD7768_SPI_RESET_1		0x03
#define AD7768_SPI_RESET_2		0x02

#define AD7768_4_NUM_OF_CHANNLES  (4) /*!< Number of channels in AD7768-4 */
#define AD7768_NUM_OF_CHANNLES    (8) /*!< Number of channels in AD7768 */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

#define __AD7768_SET_POWER_MODE(x)		(((x) & 0x3) << 4)
#define __AD7768_GET_POWER_MODE(x)		(((x) >> 4) & 0x3)
#define __AD7768_SET_MCLK_DIV(x)		  (((x) & 0x3) << 0)
#define __AD7768_GET_MCLK_DIV(x)		  ((x) & 0x3)

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static const uint16_t ad7768_dec_rate[AD7768_NUM_OF_FREQ_PER_POWER_MODE] = {32, 64, 128, 256, 512, 1024};

static const uint8_t ad7768_mclk_div[AD7768_NUM_OF_POWER_MODES] = {32, 8, 4};

/* Private function prototypes ---------------------------------------- */
/**
 * @brief  Write data to the register of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     reg_addr      Register address to write to
 * @param[in]     reg_data      Data to write to the register
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
static BaseStatusTypeDef AD7768_WriteRegister(DRV_AD7768_HandleTypeDef *dev,
                                              uint8_t reg_addr,
                                              uint8_t reg_data);

/**
 * @brief  Write masked data to a specific register of the AD7768 device.
 *
 * @param[in]     dev        Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     reg_addr   Register address to write to
 * @param[in]     mask       Bitmask indicating which bits to modify
 * @param[in]     value      Value to apply to the masked bits
 *
 * @return
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
static BaseStatusTypeDef AD7768_WriteMask(DRV_AD7768_HandleTypeDef *dev,
                                          uint8_t reg_addr,
                                          uint8_t mask,
                                          uint8_t value);

/**
 * @brief  Read data from the register of the AD7768 device.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 * @param[in]     reg_addr      Register address to write to
 * @param[in]     reg_data      Data to write to the register
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - Data of the register
 */
static uint8_t AD7768_ReadRegister(DRV_AD7768_HandleTypeDef *dev,
                                            uint8_t reg_addr);

/**
 * @brief  Test the AD7768 initalization.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
static BaseStatusTypeDef AD7768_HelloWorld(DRV_AD7768_HandleTypeDef *dev);

/**
 * @brief  Calculate the available frequency correspond to the power mode.
 *
 * @param[in]     dev           Pointer to the DRV_AD7768_HandleTypeDef structure
 *
 * @attention  Must be called after setting the power mode.
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
static BaseStatusTypeDef AD7768_CalculateAvailFreq(DRV_AD7768_HandleTypeDef *dev);
/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef DRV_AD7768_Init(DRV_AD7768_HandleTypeDef *dev,
                                  SPI_HandleTypeDef *spi,
                                  GPIO_TypeDef *cs_port,
                                  uint16_t cs_pin,
                                  uint8_t master_clock,
                                  DRV_AD7768_DeviceTypeDef device_type,
                                  uint8_t datalines)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(spi != NULL, BS_ERROR);
  __ASSERT(cs_port != NULL, BS_ERROR);

  dev->device_type = AD7768_4_DEVICE;
  dev->hspi = spi;
  dev->cs_port = cs_port;
  dev->cs_pin = cs_pin;
  dev->mclk = master_clock;
  dev->output_datalines = datalines;
  dev->active = BS_TRUE;      // Set the device as active

  DRV_AD7768_SoftReset(dev); // Perform a software reset
  DRV_AD7768_SetPowerMode(dev, MEDIAN_MODE); // Set the power mode to MEDIAN_MODE
  AD7768_HelloWorld(dev);

  DRV_AD7768_CheckDeviceStatus(dev); // Check the device status                               

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_GPIO_Enable(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  AD7768_WriteRegister(dev, AD7768_REG_GPIO_CONTROL, 0x80);

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_GPIO_SetDirection(DRV_AD7768_HandleTypeDef *dev, uint8_t pin, uint8_t direction)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t reg_val = AD7768_ReadRegister(dev, AD7768_REG_GPIO_CONTROL);

  if (direction == AD7768_GPIO_MODE_OUTPUT) {
    reg_val |= (1 << pin); // Set the bit for output
  } else {
    reg_val &= ~(1 << pin); // Clear the bit for input
  }

  AD7768_WriteRegister(dev, AD7768_REG_GPIO_CONTROL, reg_val);

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_GPIO_WritePin(DRV_AD7768_HandleTypeDef *dev, uint8_t pin, BoolTypeDef state)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t reg_val = AD7768_ReadRegister(dev, AD7768_REG_GPIO_WRITE);

  if (state == BS_TRUE) {
    reg_val |= (1 << pin); // Set the bit for high
  } else {
    reg_val &= ~(1 << pin); // Clear the bit for low
  }

  AD7768_WriteRegister(dev, AD7768_REG_GPIO_WRITE, reg_val);

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_HardReset(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);

  HAL_GPIO_WritePin(ADC_NDRDY_GPIO_Port, ADC_NCS_Pin, GPIO_PIN_RESET);
  HAL_Delay(10); // Delay for the reset to take effect
  HAL_GPIO_WritePin(ADC_NDRDY_GPIO_Port, ADC_NCS_Pin, GPIO_PIN_SET);

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_Sync(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  // Step 1: Clear SPI_SYNC bit (bit = 0)
  AD7768_WriteMask(dev,
                  AD7768_DATA_CONTROL,
                  AD7768_DATA_CONTROL_SPI_SYNC_MSK,
                  AD7768_DATA_CONTROL_SPI_SYNC_CLEAR);

  // Small delay before sync assert
  HAL_Delay(1);

  // Step 2: Assert SPI_SYNC bit (bit = 1)
  AD7768_WriteMask(dev,
                  AD7768_DATA_CONTROL,
                  AD7768_DATA_CONTROL_SPI_SYNC_MSK,
                  AD7768_DATA_CONTROL_SPI_SYNC);

return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_CheckDeviceStatus(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  dev->device_status = AD7768_ReadRegister(dev, AD7768_DEVICE_STATUS);
  printf("AD7768 status register: 0x%x\n", dev->device_status);
  if ((dev->device_status & AD7768_STATUS_CHIP_ERR) == AD7768_STATUS_CHIP_ERR) 
  {
    printf("AD7768: Chip error detected!\n");
    return BS_ERROR; // Error: Device not responding
  }
  else if ((dev->device_status & AD7768_STATUS_NO_CLOCK_ERR) == AD7768_STATUS_NO_CLOCK_ERR) 
  {
    printf("AD7768: No clock error detected!\n");
    return BS_ERROR; // Error: Device not responding
  }
  else if ((dev->device_status & AD7768_STATUS_RAM_BIST_PASS) == AD7768_STATUS_RAM_BIST_PASS) 
  {
    printf("AD7768: RAM BIST passed!\n");
  }
  else if ((dev->device_status & AD7768_STATUS_RAM_BIST_RUNNING) == AD7768_STATUS_RAM_BIST_RUNNING) 
  {
    printf("AD7768: RAM BIST running!\n");
  }
  else 
  {
    printf("AD7768: No errors detected!\n");
  }

  return BS_OK; // Success: Device is responding
}

BaseStatusTypeDef DRV_AD7768_SetPowerMode(DRV_AD7768_HandleTypeDef *dev, DRV_AD7768_PowerModesTypeDef mode)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);
  
  uint8_t pwr_val = 0;
  uint8_t mclk_div_val = 0;

  dev->power_mode = mode; // Set the power mode
  pwr_val = __AD7768_SET_POWER_MODE(dev->power_mode);

  AD7768_WriteMask(dev,
                  AD7768_POWER_MODE,
                  AD7768_POWER_MODE_MSK,
                  pwr_val); // Set the power mode in the register

  mclk_div_val = __AD7768_SET_MCLK_DIV(dev->power_mode);

  AD7768_WriteMask(dev,
                  AD7768_POWER_MODE,
                  AD7768_MCLK_DIV_MSK,
                  mclk_div_val); // Set the MCLK divider correspond to the power mode

  DRV_AD7768_Sync(dev); // Issue a sync pulse to apply the changes

  // Double check the power mode
  uint8_t reg_val = AD7768_ReadRegister(dev, AD7768_POWER_MODE);
  if ((__AD7768_GET_POWER_MODE(reg_val) != mode) || (__AD7768_GET_MCLK_DIV(reg_val) != mclk_div_val)) 
  {
    printf("AD7768: Set power mode and MCLK divider failed!\n");
    return BS_ERROR; // Error: Set power mode failed
  }

  AD7768_CalculateAvailFreq(dev); // Calculate the available frequencies after change the power mode

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_SoftReset(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  // 
  AD7768_WriteMask(dev,
                  AD7768_DATA_CONTROL,
                  AD7768_SPI_RESET_MSK,
                  AD7768_SPI_RESET_1);

  // Small delay before reset deassertion
  HAL_Delay(1);

  // 
  AD7768_WriteMask(dev,
                  AD7768_DATA_CONTROL,
                  AD7768_SPI_RESET_MSK,
                  AD7768_SPI_RESET_2);

  return BS_OK;
}

BaseStatusTypeDef DRV_AD7768_SetOutputDataRate(DRV_AD7768_HandleTypeDef *dev, uint32_t freq)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t channel_per_dout;

  channel_per_dout = dev->device_type == AD7768_4_DEVICE ? AD7768_4_NUM_OF_CHANNLES : AD7768_NUM_OF_CHANNLES; // Get the number of channels per data output line


  return BS_OK;
}

/* Private definitions ------------------------------------------------ */
static BaseStatusTypeDef AD7768_WriteRegister(DRV_AD7768_HandleTypeDef *dev,
                                              uint8_t reg_addr,
                                              uint8_t reg_data)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  HAL_StatusTypeDef ret = HAL_OK;

  uint8_t tx_data[2];
  tx_data[0] = reg_addr & 0x7F; // MSB = 0 -> write command
  tx_data[1] = reg_data;

  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
  ret = HAL_SPI_Transmit(dev->hspi, tx_data, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

  __ASSERT(ret == HAL_OK, BS_ERROR);

  return BS_OK;
}

static uint8_t AD7768_ReadRegister(DRV_AD7768_HandleTypeDef *dev,
                                  uint8_t reg_addr)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t tx_data[2] = {reg_addr | 0x80, 0}; // MSB = 1 -> read command
  uint8_t rx_data[2] = {0};

  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(dev->hspi, tx_data, rx_data, 2, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

  return rx_data[1]; // Return the data read from the register
}

static BaseStatusTypeDef AD7768_HelloWorld(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  // Turn on the user GPIO for testing the initialization
  DRV_AD7768_GPIO_Enable(dev);
  for (uint8_t i = 0; i < 5; i++) {
    DRV_AD7768_GPIO_SetDirection(dev, i, AD7768_GPIO_MODE_OUTPUT); // Set GPIO pin "i" to output
  }
  
  for (uint8_t i = 0; i < 5; i++) {
    DRV_AD7768_GPIO_WritePin(dev, i, BS_FALSE); // Set GPIO pin "i" to high
    HAL_Delay(150);
    DRV_AD7768_GPIO_WritePin(dev, i, BS_TRUE); // Set GPIO pin "i" to low
    HAL_Delay(150);
  }

  return 0; // Success: Device is responding
}

static BaseStatusTypeDef AD7768_WriteMask(DRV_AD7768_HandleTypeDef *dev,
                                          uint8_t reg_addr,
                                          uint8_t mask,
                                          uint8_t value)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t reg_val = AD7768_ReadRegister(dev, reg_addr);

  // Write bits specified by mask
  reg_val &= ~mask;
  // Apply new masked bits
  reg_val |= value;
  // Write back modified value
  AD7768_WriteRegister(dev, reg_addr, reg_val);

  return BS_OK;
}

static BaseStatusTypeDef AD7768_CalculateAvailFreq(DRV_AD7768_HandleTypeDef *dev)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(dev->active == BS_TRUE, BS_ERROR);

  uint8_t pwr_mode_idx = (dev->power_mode) ? (dev->power_mode - 1): 0; // Get the index of the power mode
  uint32_t freq = dev->mclk * 1000000 / ad7768_mclk_div[pwr_mode_idx]; // Calculate the modulator frequency based on the power mode
  // Calculate the available frequency based on the power mode
  for (uint8_t i = 0; i < AD7768_NUM_OF_FREQ_PER_POWER_MODE; i++)
  {
    dev->avail_freq[pwr_mode_idx].freq_config[i].freq = freq / ad7768_dec_rate[i];
    dev->avail_freq[pwr_mode_idx].freq_config[i].dec_rate = ad7768_dec_rate[i];
  }
  dev->avail_freq[pwr_mode_idx].num_of_freqs = AD7768_NUM_OF_FREQ_PER_POWER_MODE; // Set the number of available frequencies

  return BS_OK;
}
/* End of file -------------------------------------------------------- */
