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

  BSP_DWT_Init();

  ret = SYS_Serial_Init();
  __ASSERT(ret == BS_OK, BS_ERROR);

  SYS_DATA_MNG_Init();

  ret = SYS_ACQ_MNG_Init();
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = SYS_DATA_MNG_CreateTopic(SYS_DATA_MNG_TOPIC_ADC_TO_ACQ_SYS, SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL*4, 5);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = SYS_DATA_MNG_SubscribeTopic(SYS_DATA_MNG_TOPIC_ADC_TO_ACQ_SYS, SYS_ACQ_MNG_ProcessData);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = SYS_DATA_MNG_CreateTopic(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL*4, 10);
  __ASSERT(ret == BS_OK, BS_ERROR);

  ret = SYS_DATA_MNG_SubscribeTopic(SYS_DATA_MNG_TOPIC_ACQ_SYS_TO_USB, SYS_Serial_SendSamples);
  __ASSERT(ret == BS_OK, BS_ERROR);

  return BS_OK;
}

BaseStatusTypeDef SYS_SERVICE_MNG_Process(void)
{
  SYS_DATA_MNG_FireEvent();
  return BS_OK;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
