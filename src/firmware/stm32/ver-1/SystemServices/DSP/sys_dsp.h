/**
 * @file       sys_dsp.h
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       yyyy-mm-dd
 * @author     Giang Phan
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note       None.
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __CODE_TEMPLATE_H
#define __CODE_TEMPLATE_H

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
  PUBLIC_ENUM_1, /**< Description of PUBLIC_ENUM_1 */
  PUBLIC_ENUM_2, /**< Description of PUBLIC_ENUM_2 */
  PUBLIC_ENUM_3  /**< Description of PUBLIC_ENUM_3 */
}
public_enum_type_t;

/**
 * @brief <structure descriptiton>
 */
typedef struct 
{
  uint32_t member_1, /**< Description of member_1 */
  uint32_t member_2, /**< Description of member_2 */
  uint32_t member_3  /**< Description of member_3 */
}
public_struct_type_t;

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
#define PUBLIC_MACRO(a)  do_something_with(a)

/* Public variables --------------------------------------------------- */
extern int g_var_1; /**< Description of public variable g_var_1 */
extern int g_var_2; /**< Description of public variable g_var_2 */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  <function description>
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
 void public_function(void);

#endif // DSP_SYS_DSP_H_

/* End of file -------------------------------------------------------- */
