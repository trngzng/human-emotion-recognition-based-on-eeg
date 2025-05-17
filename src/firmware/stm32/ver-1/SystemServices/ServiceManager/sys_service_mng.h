/**
 * @file       sys_service_mng.h
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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_MNG_H
#define __SYS_MNG_H

/* Includes ----------------------------------------------------------- */
#include "sys_serial.h"
#include "sys_data_mng.h"
#include "sys_acquisition_mng.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */


/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize all system services.
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_SERVICE_MNG_Init(void);

/**
 * @brief  Process all system services.
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_SERVICE_MNG_Process(void);

#endif // __SYS_MNG_H

/* End of file -------------------------------------------------------- */
