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

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_SERVICE_MNG_Init(void)
{
  SYS_Serial_Init();
  SYS_ACQ_MNG_Init();
  SYS_DATA_MNG_Init();
  
  return BS_OK;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
