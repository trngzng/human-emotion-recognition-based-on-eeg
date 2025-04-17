/**
 * @file       sys_def.h
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       2025-02-10
 * @author     Giang Phan
 *             
 * @brief      Definition of macros for system services
 *             
 * @note       None.
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_DEF_H
#define __SYS_DEF_H

/* Includes ----------------------------------------------------------- */
/* Public defines ----------------------------------------------------- */
#define PUBLIC_DEFINE_1  (0) /*!< Description of PUBLIC_DEFINE_1 */
#define PUBLIC_DEFINE_2  (0) /*!< Description of PUBLIC_DEFINE_2 */

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief <enum descriptiton>
 */
typedef enum 
{
  SYS_OK      = 0x00U, /**< System service is OK */
  SYS_TIMEOUT = 0x01U, /**< System service is busy */
  SYS_ERROR   = 0x02U  /**< System service failed */
} SYS_StatusTypeDef;

/* Public macros ------------------------------------------------------ */
/**
 * @brief  <macro description>
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
//#define PUBLIC_MACRO(a)  do_something_with(a)

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif // __SYS_DEF_H

/* End of file -------------------------------------------------------- */
