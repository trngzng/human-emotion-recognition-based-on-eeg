/**
 * @file       bsp_ext_adc.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    major.minor.patch
 * @date       yyyy-mm-dd
 * @author     <first_name_1> <last_name_1>
 * @author     <first_name_2> <last_name_2>
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note          
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_ext_adc.h"
#include "main.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

/* Private variables -------------------------------------------------- */
static DRV_AD7768_HandleTypeDef uExtADC; /**< The external ADC stucture */
static uint8_t uRxBuffer[RECEIVED_FRAME_LENGTH] = {0}; /**< The buffer to store the samples from the ADC */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef BSP_EXT_ADC_Init(void)
{
  BaseStatusTypeDef ret = BS_ERROR;
  ret = DRV_AD7768_Init(&uExtADC, &hspi1,&hspi3, ADC_NCS_GPIO_Port, ADC_NCS_Pin, 32, AD7768_4_DEVICE, 1);
  __ASSERT(ret == BS_OK, ret);

  ret = DRV_AD7768_SetPowerMode(&uExtADC, LOW_POWER_MODE);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = DRV_AD7768_SetSamplingRate(&uExtADC, 1000, AD7768_CHANNEL_IN_MODE_A);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = DRV_AD7768_SetFilterType(&uExtADC, AD7768_WIDEBAND_FILTER_TYPE, AD7768_CHANNEL_IN_MODE_A);
  __ASSERT(ret == BS_OK, BS_ERROR);

  return BS_OK;
}

BaseStatusTypeDef BSP_EXT_ADC_StartReceivingSamples(void)
{
  __ASSERT(uExtADC.active == BS_TRUE, BS_ERROR);

  HAL_SPI_Receive_DMA(uExtADC.data_spi, uRxBuffer, sizeof(uRxBuffer));

  return BS_OK;
}

BaseStatusTypeDef BSP_EXT_ADC_StopReceivingSamples(void)
{
  __ASSERT(uExtADC.active == BS_TRUE, BS_ERROR);

  HAL_SPI_DMAStop(uExtADC.data_spi);
  
  return BS_OK;
}

/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
