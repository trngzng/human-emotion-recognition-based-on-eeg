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
#define RECEIVED_FRAME_LENGTH                 (32 * 4 / 8)  /**< AD7768-4 has 4 channel with data frame: 8-bit header and 24-bit ADC conversion  data */
#define EXTERNAL_ADC_MODULE_MASTER_CLOCK_MHZ  (32) /**< The external ADC module master clock in Hz */
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
 * @brief  Start receiving the ADC conversion data from the external ADC module.
 *
 * @param[in]     <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef BSP_EXT_ADC_StartReceivingADCConversionData(void);

/**
 * @brief  Stop receiving the ADC conversion data from the external ADC module.
 *
 * @param[in]     <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef BSP_EXT_ADC_StopReceivingADCConversionData(void);

#endif // __BSP_EXT_ADC_H

/* End of file -------------------------------------------------------- */
