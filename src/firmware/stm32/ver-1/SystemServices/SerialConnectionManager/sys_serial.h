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
#include "common.h"
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

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Initialize the system serial service
 *
 * @attention  Must initialize the USB device libraries before use
 *
 * @return
 *  - BS_OK: Success
 *  - else: Error
 */
BaseStatusTypeDef SYS_Serial_Init();

/**
 * @brief  Send samples to GUI via USB
 *
 * @param[in]     psample       Pointer to samples' array
 * @param[in]     num           Length of data
 *
 * @attention  Must initialize the USB device libraries before use
 *
 * @return
 *  - SYS_SERIAL_READY: Success
 *  - else: Error
 */
BaseStatusTypeDef SYS_Serial_SendSamples(uint8_t *psample, uint8_t num);

/**
 * @brief  Print samples on the terminal
 *
 * @param[in]     psample       Pointer to samples' array
 * @param[in]     num           Length of data
 *
 * @attention  Must initialize the USB device libraries before use
 *
 * @return
 *  - SYS_SERIAL_READY: Success
 *  - else: Error
 */
BaseStatusTypeDef SYS_Serial_PrintSamples(uint8_t *psamples, uint8_t num);

#endif // __SYS_SERIAL_H

/* End of file -------------------------------------------------------- */
