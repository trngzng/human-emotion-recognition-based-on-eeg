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

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static const uint16_t ad7768_dec_rate[6] = {32, 64, 128, 256, 512, 1024};

static const uint8_t ad7768_mclk_div[3] = {32, 8, 4};

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
/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef DRV_AD7768_Init(DRV_AD7768_HandleTypeDef *dev,
                                  SPI_HandleTypeDef *spi,
                                  GPIO_TypeDef *cs_port,
                                  uint16_t cs_pin)
{
  __ASSERT(dev != NULL, BS_ERROR);
  __ASSERT(spi != NULL, BS_ERROR);
  __ASSERT(cs_port != NULL, BS_ERROR);

  dev->hspi = spi;
  dev->cs_port = cs_port;
  dev->cs_pin = cs_pin;
  dev->active = BS_TRUE;      // Set the device as active

  AD7768_HelloWorld(dev);     // Test the device
  DRV_AD7768_HardReset(dev);  // Reset the device
  HAL_Delay(100);             // Delay for the reset to take effect

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

  uint8_t reg_val = AD7768_ReadRegister(dev, AD7768_CH_MODE);
  if (reg_val == 0xFF) {
    return 1; // Error: Device not responding
  }

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
  reg_val |= (value & mask);
  // Write back modified value
  AD7768_WriteRegister(dev, reg_addr, reg_val);

  return BS_OK;
}
/* End of file -------------------------------------------------------- */
