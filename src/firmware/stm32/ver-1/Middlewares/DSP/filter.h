/**
 * @file       filter.h
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       2025-06-05
 * @author     Giang Phan
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note       None.
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __FILTER_H
#define __FILTER_H

/* Includes ----------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief <enum descriptiton>
 */
typedef enum 
{
  LOW_PASS_FILTER = 0, /**< Description of PUBLIC_ENUM_1 */
  NOTCH_FILTER, /**< Description of PUBLIC_ENUM_2 */
  HIGH_PASS_FILTER  /**< Description of PUBLIC_ENUM_3 */
} FilterType;

/**
 * @brief <structure descriptiton>
 */
typedef struct 
{
  
  FilterType type;
  uint8_t order; /**< Order of the filter */
  uint32_t sample_rate; /**< Sample rate of the filter */
  uint32_t cutoff_freq; /**< Cutoff frequency of the filter */
  const float *numerator; /**< Description of member_1 */
  const float *denominator;
  bool active; /**< Indicates if the filter is active */
} FilterHandleTypeDef;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize the filter specifications
 *
 * @param[in]   h           Pointer to the filter handle structure
 * @param[in]   type        Type of the filter (low-pass, notch, high-pass)
 * @param[in]   order       Order of the filter
 * @param[in]   sample_rate Sample rate of the filter
 * @param[in]   cutoff_freq Cutoff frequency of the filter
 * @param[in]   den         Pointer to the denominator coefficients
 * @param[in]   num         Pointer to the numerator coefficients
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - None
 */
void FILTER_Init(FilterHandleTypeDef *h, 
                FilterType type, 
                uint8_t order, 
                uint32_t sampling_rate, 
                uint32_t fc, 
                const float *den, 
                const float *num);

/**
 * @brief  Initialize the filter specifications
 *
 * @param[in]   h           Pointer to the filter handle structure
 * @param[in]   input       Input sample to be filtered
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - Filtered output sample
 */
float FILTER_Process(FilterHandleTypeDef *h, float input);

#endif // DSP_FILTER_H_

/* End of file -------------------------------------------------------- */
