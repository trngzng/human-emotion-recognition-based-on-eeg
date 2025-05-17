/**
 * @file       bsp_dwt.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-17
 * @author     Giang Phan
 *             
 * @brief      Source file for the DWT (Data Watchpoint and Trace) unit.
 *             
 * @note          
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_dwt.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void BSP_DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t BSP_DWT_GetCycles(void)
{
  return DWT->CYCCNT;
}

float BSP_DWT_CyclesToUs(uint32_t cycles)
{
  return (float)cycles / ((float)SystemCoreClock / 1e6f);
}

float BSP_DWT_ElapsedUs(uint32_t start, uint32_t stop)
{
  return BSP_DWT_CyclesToUs(stop - start);
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
