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
#include "filter.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static float rawSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL]; /**< Raw samples from ADC */
static float filteredSamples[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL]; /**< Samples after the filter processing */

const float firCoeff100HzSampling1000[51] = {6.23707493203100e-19,
                                             0.000651936536801578,
                                             0.00124319314520673,
                                             0.00154534133131494,
                                             0.00121969027941796,
                                             -1.30863330037917e-18,
                                             -0.00201548144449766,
                                             -0.00415052127829799,
                                             -0.00522690670351652,
                                             -0.00402425886959341,
                                             3.10179234333809e-18,
                                             0.00606470479754808,
                                             0.0119023545680876,
                                             0.0143515447823021,
                                             0.0106508925575157,
                                             -5.31825881490377e-18,
                                             -0.0152975048205391,
                                             -0.0297614666016229,
                                             -0.0360342519662000,
                                             -0.0272952563088151,
                                             7.11141785786268e-18,
                                             0.0441130473576423,
                                             0.0976505458279451,
                                             0.149177852216505,
                                             0.186419208415108,
                                             0.200000000000000,
                                             0.186419208415108,
                                             0.149177852216505,
                                             0.0976505458279451,
                                             0.0441130473576423,
                                             7.11141785786268e-18,
                                             -0.0272952563088151,
                                             -0.0360342519662000,
                                             -0.0297614666016229,
                                             -0.0152975048205391,
                                             -5.31825881490377e-18,
                                             0.0106508925575157,
                                             0.0143515447823021,
                                             0.0119023545680876,
                                             0.00606470479754808,
                                             3.10179234333809e-18,
                                             -0.00402425886959341,
                                             -0.00522690670351652,
                                             -0.00415052127829800,
                                             -0.00201548144449766,
                                             -1.30863330037917e-18,
                                             0.00121969027941796,
                                             0.00154534133131494,
                                             0.00124319314520673,
                                             0.000651936536801578,
                                             6.23707493203100e-19};

static FilterHandleTypeDef notchFilter50HzChannel[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL];
static FilterHandleTypeDef lowPassFilter100HzChannel[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL];


/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_ACQ_MNG_Init(void)
{
  BaseStatusTypeDef ret = BS_OK;

  ret = BSP_EXT_ADC_Init();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC initialization was successful

  for (uint_fast8_t i = 0; i < SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL; i++)
  {
    FILTER_Init(&notchFilter50HzChannel[i], NOTCH_FILTER, 2, 1000, 50, NULL, NULL);
    FILTER_Init(&lowPassFilter100HzChannel[i], LOW_PASS_FILTER, 50, 1000, 100, NULL, firCoeff100HzSampling1000);
  }

  return BS_OK;   
}

void SYS_ACQ_MNG_ProcessData(uint8_t *data, uint32_t size)
{
  float tmp = 0;

//  UNUSED(size);

   memcpy(rawSamples, data, sizeof(rawSamples));

   for (uint_fast8_t i = 0; i < 2; i++)
   {
     tmp = FILTER_Process(&notchFilter50HzChannel[i], rawSamples[i]);
     filteredSamples[i] = FILTER_Process(&lowPassFilter100HzChannel[i], tmp);
   }

  SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, (uint8_t *)rawSamples, sizeof(rawSamples));
//  SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, (uint8_t *)filteredSamples, sizeof(filteredSamples));
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
