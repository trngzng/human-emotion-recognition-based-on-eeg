/**
 * @file       filter.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-06-05
 * @author     Giang Phan
 *             
 * @brief      Filter implementation for digital signal processing.
 *             
 * @note          
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "filter.h"
#include <string.h>
#include "math.h"
/* Private defines ---------------------------------------------------- */
#define FILTER_MAX_ORDER  (255)
/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
static float x[FILTER_MAX_ORDER];
static float y[FILTER_MAX_ORDER];

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void FILTER_Init(FilterHandleTypeDef *h, 
                FilterType type, 
                uint8_t order, 
                uint32_t sampling_rate, 
                uint32_t fc, 
                const float *den, 
                const float *num)
{
  h->type = type;
  h->order = order;
  h->sample_rate = sampling_rate;
  h->cutoff_freq = fc;
  h->numerator = num;
  h->denominator = den;
  h->active = true; // Set the filter to active by default

  memset(x, 0, sizeof(float) * (order + 1)); // Initialize input buffer
  memset(y, 0, sizeof(float) * (order + 1)); // Initialize output buffer
}

float FILTER_Process(FilterHandleTypeDef *h, float input)
{
  if (h->active == false)
  {
    return input;
  }

  float output = 0.0f;
  // Shift the input buffer
  for (uint_fast8_t i = h->order; i > 0; i--)
  {
    x[i] = x[i - 1];
  }
  x[0] = input;


  // Shift the output buffer
  for (uint_fast8_t i = h->order; i > 0; i--)
  {
    y[i] = y[i - 1];
  }
  y[0] = 0;

  // Apply the filter
  switch (h->type)
  {
  case LOW_PASS_FILTER:
    for (int i = 0; i < h->order + 1; i++)
      {
        output += h->numerator[i] * x[i];
      }
    break;

  case NOTCH_FILTER:
    float cos_omega = cos(2.0f * M_PI * h->cutoff_freq / h->sample_rate);
    float a = 0.99;
    output = x[0] - 2.0f * cos_omega * x[1] + x[2] + 2.0f * a * cos_omega * y[1] - a * a * y[2];
    break;
  
  default:
    break;
  }

  y[0] = output;
  return output;
}
/* Private definitions ------------------------------------------------ */

/* End of file -------------------------------------------------------- */
