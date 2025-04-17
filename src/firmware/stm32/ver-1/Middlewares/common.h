/**
 * @file       common.h
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       2025-02-10
 * @author     Giang Phan
 *
 * @brief      Common-used macros
 *
 * @note       None.
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __COMMON_H_
#define __COMMON_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */
#ifdef DEBUG
#define __ASSERT(_EXPR_, _RET_VAL_) \
  do                                \
  {                                 \
    if (!(_EXPR_))                  \
      return _RET_VAL_;             \
  } while (0)
#else
#define __ASSERT(_EXPR_, _RET_VAL_) ((void)0)
#endif

#define __CALLBACK(_CB_FUNC_, ...) \
  do                               \
  {                                \
    if ((_CB_FUNC_) != NULL)       \
      _CB_FUNC_(__VA_ARGS__);      \
  } while (0)

#define __SIZE_OF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*__BUFFER__))

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif /*__COMMON_H_ */
