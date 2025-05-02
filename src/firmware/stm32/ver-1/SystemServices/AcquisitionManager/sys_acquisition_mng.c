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

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static uint32_t adcConversionData[SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL][SYS_ACQ_MNG_MAX_EEG_SAMPLES]; /**< ADC conversion data buffer */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_ACQ_MNG_Init(void)
{
  BaseStatusTypeDef ret = BS_OK;

  ret = BSP_EXT_ADC_Init();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC initialization was successful
  ret = BSP_EXT_ADC_StartReceivingSamples();
  __ASSERT(ret == BS_OK, BS_ERROR); // Check if the ADC started receiving samples successfully

  return BS_OK;   
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
