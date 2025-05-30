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
#include "sys_data_mng.h"
#include "sys_serial.h"
#include "bsp_dwt.h"
#include <string.h>
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static int32_t rawSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL][SYS_ACQ_MNG_MAX_EEG_SAMPLES]; /**< Raw samples from ADC */
static float filteredSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL][SYS_ACQ_MNG_MAX_EEG_SAMPLES]; /**< Samples after the filter processing */
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_ACQ_MNG_Init(void)
{
  BaseStatusTypeDef ret = BS_OK;

  ret = BSP_EXT_ADC_Init();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC initialization was successful

  return BS_OK;   
}

void SYS_ACQ_MNG_ProcessData(uint8_t *data, uint32_t size)
{
//  static uint8_t sampleCount = 0;

//  UNUSED(size);

//  for (uint_fast8_t ch = 0; ch < 2; ch++)  // 2 channel
//  {
//    uint8_t *p = &data[ch * 4];
//    uint32_t raw24Bit = (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
//
//    // Sign extend 24-bit to 32-bit signed
//    if ((raw24Bit & 0x800000) == 0x800000)
//      raw24Bit |= 0xFF000000;
//
//    rawSamples[ch][sampleCount] = (int32_t)raw24Bit;
//
////    rawSamples[ch][sampleCount] = raw24Bit;
//
//  }
//
//  if (++sampleCount == SYS_ACQ_MNG_MAX_EEG_SAMPLES)
//  {
//    sampleCount = 0;
    SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, data, size);
//    SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, (uint8_t *)filteredSamples, sizeof(filteredSamples));
//  }
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
