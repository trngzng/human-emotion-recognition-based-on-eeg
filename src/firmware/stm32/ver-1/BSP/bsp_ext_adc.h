/**
 * @file       bsp_ext_adc.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-01
 * @author     Giang Phan
 *             
 * @brief      Handle the external ADC module.
 *             
 * @note          
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_EXT_ADC_H
#define __BSP_EXT_ADC_H

/* Includes ----------------------------------------------------------- */
#include "drv_ad7768.h"
/* Public defines ----------------------------------------------------- */
#define PUBLIC_DEFINE_1  (0) /*!< Description of PUBLIC_DEFINE_1 */
#define PUBLIC_DEFINE_2  (0) /*!< Description of PUBLIC_DEFINE_2 */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize the external ADC module.
 *
 * @param[in]     <param_name>  <param_despcription>
 *
 * @attention  The module must be initialized before use. The module always converts data when the system is powered on.
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef BSP_EXT_ADC_Init(void);

/**
 * @brief  Start receiving the samples from the external ADC module.
 *
 * @param[in]     <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef BSP_EXT_ADC_StartReceivingSamples(void);

#endif // __BSP_EXT_ADC_H

/* End of file -------------------------------------------------------- */
