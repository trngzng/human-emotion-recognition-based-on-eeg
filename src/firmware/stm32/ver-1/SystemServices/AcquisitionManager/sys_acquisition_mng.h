/**
 * @file       sys_acquisition_mng.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-02
 * @author     Giang Phan
 *             
 * @brief      Manage acquisition data
 *             
 * @note          
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_ACQ_MNG_H
#define __SYS_ACQ_MNG_H

/* Includes ----------------------------------------------------------- */
#include "bsp_ext_adc.h"
/* Public defines ----------------------------------------------------- */
#define SYS_ACQ_MNG_NUM_OF_EEG_CHANNEL  (2) /*!< Number of an EEG channel of this project*/
#define SYS_ACQ_MNG_MAX_EEG_SAMPLES     (7) /*!< Maximum EEG samples stored in buffer*/
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize system acquisition manager
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_ACQ_MNG_Init(void);

/**
 * @brief  Process data from acquisition manager
 *
 * @param[in]     data          Pointer to the data buffer
 * @param[in]     size          Size of the data buffer
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
void SYS_ACQ_MNG_ProcessData(uint8_t *data, uint32_t size);

#endif // __SYS_ACQ_MNG_H

/* End of file -------------------------------------------------------- */
