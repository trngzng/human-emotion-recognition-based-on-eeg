/**
 * @file       <file_name>.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    m1.0.0
 * @date       2025-05-02
 * @author     Giang Phan
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note       None 
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_acquisition_mng.h"
#include <string.h>
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static volatile uint32_t rawSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL][SYS_ACQ_MNG_MAX_EEG_SAMPLES]; /**< Raw samples from ADC */
static volatile float filteredSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL][SYS_ACQ_MNG_MAX_EEG_SAMPLES]; /**< Samples after the filter processing */
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_ACQ_MNG_Init(void)
{
  BaseStatusTypeDef ret = BS_OK;

  ret = BSP_EXT_ADC_Init();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC initialization was successful
  ret = BSP_EXT_ADC_StartReceivingADCConversionData();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC started receiving ADC conversion data successfully

  return BS_OK;   
}

void SYS_ACQ_MNG_ProcessData(uint8_t *data, uint32_t size)
{
  static uint8_t sampleCount = 0;
  UNUSED(size);
  for (uint_fast8_t i = 0; i < 2; i++)
  {
    rawSamples[i][sampleCount] = data[i];
  }
  if (sampleCount++ == SYS_ACQ_MNG_MAX_EEG_SAMPLES) sampleCount = 0;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
