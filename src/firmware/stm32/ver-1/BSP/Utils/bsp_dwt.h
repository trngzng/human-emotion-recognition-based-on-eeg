/**
 * @file       bsp_dwt.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-17
 * @author     Giang Phan
 *             
 * @brief      Header file for the DWT (Data Watchpoint and Trace) unit.
 *             
 * @note          
 * @example    None

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_DWT_H
#define __BSP_DWT_H

/* Includes ----------------------------------------------------------- */
#include "stm32g4xx_hal.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/**
 * @brief  Start measurement of CPU cycles.
 *
 * @param[in]     label        Label for the measurement
 *
 * @attention  <API attention note>
 *
 * @return  
 *  -The current CPU cycle count at the start of the measurement
 */
#define __BSP_DWT_START_MEASUREMENT(label)   uint32_t __StartMeasurement_##label = BSP_DWT_GetCycles()

#define __BSP_DWT_STOP_MEASUREMENT(label)    uint32_t __StopMeasurement_##label = BSP_DWT_GetCycles(); \
                           printf("[%-10s] %.2f us\r\n", #label, BSP_DWT_ElapsedUs(_start_##label, _stop_##label))

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize the DWT (Data Watchpoint and Trace) unit.
 *
 * @param[in]     None
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
void BSP_DWT_Init(void);

/**
 * @brief  Get the number of CPU cycles since the last reset.
 *
 * @param[in]     None
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
uint32_t BSP_DWT_GetCycles(void);

/**
 * @brief  Convert the number of CPU cycles to microseconds.
 *
 * @param[in]     cycles       Number of CPU cycles
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - The number of microseconds corresponding to the given number of cycles
 */
float BSP_DWT_CyclesToUs(uint32_t cycles);


/**
 * @brief  Calculate the elapsed time in microseconds between two timestamps.
 *
 * @param[in]     start         Start timestamp
 * @param[in]     stop          Stop timestamp
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - The elapsed time in microseconds between the start and stop timestamps
 */
float BSP_DWT_ElapsedUs(uint32_t start, uint32_t stop);

#endif // __BSP_DWT_H

/* End of file -------------------------------------------------------- */
