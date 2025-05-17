/**
 * @file       sys_service_mng.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-02
 * @author     Giang Phan
 *             
 * @brief      Manage the system services.
 *             
 * @note       None.
 * @example    None.
 */

/* Includes ----------------------------------------------------------- */
#include "sys_service_mng.h"
#include "bsp_dwt.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_SERVICE_MNG_Init(void)
{
  BaseStatusTypeDef ret;

  ret = SYS_Serial_Init();
  __ASSERT(ret == BS_OK, BS_ERROR);

  SYS_DATA_MNG_Init();

  ret = SYS_ACQ_MNG_Init();
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = SYS_DATA_MNG_CreateTopic(SYS_DATA_MNG_TOPIC_ADC_TO_ACQ_SYS, AD7768_SAMPLE_SIZE*4, 4);
__ASSERT(ret == BS_OK, BS_ERROR);
  
  return BS_OK;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
