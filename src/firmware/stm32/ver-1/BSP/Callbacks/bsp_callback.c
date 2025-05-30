/**
 * @file       <file_name>.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    major.minor.patch
 * @date       yyyy-mm-dd
 * @author     <first_name_1> <last_name_1>
 * @author     <first_name_2> <last_name_2>
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note          
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_callback.h"
#include "bsp_ext_adc.h"
#include "main.h"
#include "bsp_dwt.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
//void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
//{
//  if (hspi->Instance == SPI3)
//  {
//    BSP_EXT_ADC_ParseADCConversionData();
//  }
//}
//
//void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
//{
//  if (hspi->Instance == SPI3)
//  {
//    BSP_EXT_ADC_ParseADCConversionData();
//  }
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_15)
  {
//    __BSP_DWT_START_MEASUREMENT(AdcReception);
    BSP_EXT_ADC_StartReceivingADCConversionData();
    BSP_EXT_ADC_ParseADCConversionData();
//    __BSP_DWT_STOP_MEASUREMENT(AdcReception);
  }
}

/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
