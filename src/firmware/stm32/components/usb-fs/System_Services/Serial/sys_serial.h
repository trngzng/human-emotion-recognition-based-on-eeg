/**
 * @file       sys_serial.h
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       2025-02-09
 * @author     Giang Phan
 *             
 * @brief      Manage serial communications of system.
 *             
 * @note       None.
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_SERIAL_H
#define __SYS_SERIAL_H

/* Includes ----------------------------------------------------------- */
#include "usb_device.h"
#include "sys_def.h"
/* Public defines ----------------------------------------------------- */
#define PUBLIC_DEFINE_1  (0) /*!< Description of PUBLIC_DEFINE_1 */
#define PUBLIC_DEFINE_2  (0) /*!< Description of PUBLIC_DEFINE_2 */

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief List of system serial states
 */
typedef enum 
{
  SYS_SERIAL_RESET   = 0x00U,   /**< System serial not yet initialized */
  SYS_SERIAL_READY   = 0x01U,   /**< System serial initialized and ready for use */
  SYS_SERIAL_BUSY    = 0x02U,   /**< System serial internal process is ongoing*/
  SYS_SERIAL_TIMEOUT = 0x03U,   /**< System serial internal process is timeout */
  SYS_SERIAL_ERROR   = 0x04U    /**< System serial internal process is error */
} SYS_SERIAL_StateTypeDef;

/**
 * @brief List of system serial states
 */
typedef enum
{
  SYS_SERIAL_CMD_DEVICE_MODE   = 0x00U,   /**< Select mode for device */
} SYS_SERIAL_CommandTypeDef;

/**
 * @brief <structure descriptiton>
 */
typedef struct 
{
  USBD_HandleTypeDef *pDev;
  uint8_t *pTxPacket;
  uint8_t *pRxPacket;
  SYS_SERIAL_StateTypeDef status;
} SYS_SERIAL_HandleTypeDef;

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
 * @brief  Initialize the system serial service
 *
 * @param[in]     pserial       System serial instance
 * @param[in]     pdev          USB device handler
 *
 * @attention  Must initialize the USB device libraries before use
 *
 * @return
 *  - SYS_SERIAL_READY: Success
 *  - else: Error
 */
SYS_StatusTypeDef SYS_Serial_Init(SYS_SERIAL_HandleTypeDef *pserial, USBD_HandleTypeDef *pdev);

/**
 * @brief  Send samples to GUI via USB
 *
 * @param[in]     pserial       System serial instance
 * @param[in]     psample       Pointer to samples' array
 * @param[in]     num           Length of data
 *
 * @attention  Must initialize the USB device libraries before use
 *
 * @return
 *  - SYS_SERIAL_READY: Success
 *  - else: Error
 */
SYS_StatusTypeDef SYS_Serial_SendSamples(SYS_SERIAL_HandleTypeDef *pserial, float *psample, uint8_t num);

#endif // __SYS_SERIAL_H

/* End of file -------------------------------------------------------- */
