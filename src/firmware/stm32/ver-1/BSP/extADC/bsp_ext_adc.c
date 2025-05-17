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
#include "sys_data_mng.h"
#include "main.h"
#include <string.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

typedef struct __attribute__((packed)) {
  union {
    uint8_t raw;
    struct {
      uint8_t ch_id       : 3;  // Bits 0–2
      uint8_t saturated   : 1;  // Bit 3
      uint8_t filter_type : 1;  // Bit 4
      uint8_t repeated    : 1;  // Bit 5
      uint8_t not_settled: 1;  // Bit 6
      uint8_t error       : 1;  // Bit 7
    };
  } header;
  uint8_t data[3];  // 24-bit signed, MSB first
} BSP_EXT_ADC_ConversionOutputFormatTypeDef;

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

/* Private variables -------------------------------------------------- */
static DRV_AD7768_HandleTypeDef uExtADC; /**< The external ADC stucture */
static uint8_t uRxBuffer[RECEIVED_FRAME_LENGTH] = {0}; /**< The buffer to store the samples from the ADC */

static BSP_EXT_ADC_ConversionOutputFormatTypeDef uRxData[4] = {0}; /**< The buffer to store the samples from the ADC */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef BSP_EXT_ADC_Init(void)
{
  BaseStatusTypeDef ret = BS_ERROR;
  ret = DRV_AD7768_Init(&uExtADC, &hspi1,&hspi3, ADC_NCS_GPIO_Port, ADC_NCS_Pin, EXTERNAL_ADC_MODULE_MASTER_CLOCK_MHZ, AD7768_4_DEVICE, 1);
  __ASSERT(ret == BS_OK, ret);

  ret = DRV_AD7768_SetPowerMode(&uExtADC, LOW_POWER_MODE);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = DRV_AD7768_SetSamplingRate(&uExtADC, 1000, AD7768_CHANNEL_IN_MODE_A);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = DRV_AD7768_SetFilterType(&uExtADC, AD7768_WIDEBAND_FILTER_TYPE, AD7768_CHANNEL_IN_MODE_A);
  __ASSERT(ret == BS_OK, BS_ERROR);

  return BS_OK;
}

BaseStatusTypeDef BSP_EXT_ADC_StartReceivingADCConversionData(void)
{
  __ASSERT(uExtADC.active == BS_TRUE, BS_ERROR);

  HAL_SPI_Receive_DMA(uExtADC.data_spi, uRxBuffer, sizeof(uRxBuffer));

  return BS_OK;
}

BaseStatusTypeDef BSP_EXT_ADC_StopReceivingADCConversionData(void)
{
  __ASSERT(uExtADC.active == BS_TRUE, BS_ERROR);

  __HAL_SPI_DISABLE(uExtADC.data_spi);
  
  return BS_OK;
}

BaseStatusTypeDef BSP_EXT_ADC_ParseADCConversionData(void)
{
  __ASSERT(uExtADC.active == BS_TRUE, BS_ERROR);

  static BoolTypeDef parseSecondHalf = BS_FALSE;

  uint8_t base = (parseSecondHalf == BS_TRUE) ? 2 : 0;

  for (uint8_t i = 0; i < 2; ++i)
  {
    uint8_t idx = base + i;

    uRxData[idx].header.raw = uRxBuffer[idx * 4];
    uRxData[idx].data[0] = uRxBuffer[idx * 4 + 1];
    uRxData[idx].data[1] = uRxBuffer[idx * 4 + 2];
    uRxData[idx].data[2] = uRxBuffer[idx * 4 + 3];
  }

  if (parseSecondHalf)
  {
    // Send 24-bit sample N of 4 channels
    SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_TOPIC_ADC_TO_ACQ_SYS, (uint8_t *)uRxData, sizeof(uRxData));
    parseSecondHalf = BS_FALSE;
  }
  else
  {
    parseSecondHalf = BS_TRUE;
  }


  return BS_OK;
}

/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
