/**
 * @file       drv_ad7768_reg.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-04-17
 * @author     Giang Phan
 *             
 * @brief      Register definitions and macros for the AD7768 device.
 *             
 * @note       None.  
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRV_AD7768_REG_H
#define __DRV_AD7768_REG_H

/* Includes ----------------------------------------------------------- */
/* Public defines ----------------------------------------------------- */

// Register addresses
#define AD7768_CH_STANDBY            0x00
#define AD7768_CH_MODE_A             0x01
#define AD7768_CH_MODE_B             0x02
#define AD7768_POWER_MODE            0x04
#define AD7768_DATA_CONTROL          0x06
#define AD7768_INTERFACE_CFG         0x07
#define AD7768_DEVICE_STATUS         0x09
#define AD7768_REG_GPIO_CONTROL      0x0E
#define AD7768_REG_GPIO_WRITE        0x0F
#define AD7768_REG_GPIO_READ         0x10

// CH_MODE Register
#define AD7768_CH_MODE_FILTER_TYPE_MSK       (1 << 3)
#define AD7768_CH_MODE_FILTER_TYPE_MODE(x)   (((x) & 0x1) << 3)
#define AD7768_CH_MODE_GET_FILTER_TYPE(x)    (((x) >> 3) & 0x1)

#define AD7768_CH_MODE_DEC_RATE_MSK          0x07
#define AD7768_CH_MODE_DEC_RATE_MODE(x)      (((x) & 0x7) << 0)

// POWER_MODE Register
#define AD7768_POWER_MODE_POWER_MODE_MSK     (0x3 << 4)
#define AD7768_POWER_MODE_POWER_MODE(x)      (((x) & 0x3) << 4)
#define AD7768_POWER_MODE_GET_POWER_MODE(x)  (((x) >> 4) & 0x3)

#define AD7768_POWER_MODE_MCLK_DIV_MSK       (0x3 << 0)
#define AD7768_POWER_MODE_MCLK_DIV_MODE(x)   (((x) & 0x3) << 0)

#define AD7768_MAP_POWER_MODE_TO_REGVAL(x)   ((x) ? ((x) + 1) : 0)
#define AD7768_MAP_REGVAL_TO_POWER_MODE(x)   ((x) ? ((x) - 1) : 0)

// DATA_CONTROL Register
#define AD7768_DATA_CONTROL_SPI_RESET_MSK    0x03
#define AD7768_DATA_CONTROL_SPI_RESET_1      0x03
#define AD7768_DATA_CONTROL_SPI_RESET_2      0x02

#define AD7768_DATA_CONTROL_SPI_SYNC_MSK     (1 << 7)
#define AD7768_DATA_CONTROL_SPI_SYNC         (1 << 7)
#define AD7768_DATA_CONTROL_SPI_SYNC_CLEAR   0x00

// INTERFACE_CFG Register
#define AD7768_INTERFACE_CFG_DCLK_DIV_MSK       0x03
#define AD7768_INTERFACE_CFG_DCLK_DIV_MODE(x)   (4 - __builtin_ffs(x))

#define AD7768_INTERFACE_CFG_CRC_SELECT_MSK     (0x3 << 2)
#define AD7768_INTERFACE_CFG_CRC_SELECT         (0x01 << 2)

// Write flag for SPI
#define AD7768_WR_FLAG_MSK(x)     (0x80 | ((x) & 0x7F))

// GAIN registers
#define AD7768_GAIN_BASE_ADDRESS            0x36
#define AD7768_GAIN_SIZEOF                  (3)

// Output format
#define AD7768_OUTPUT_MODE_TWOS_COMPLEMENT   0x01

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

#endif // __DRV_AD7768_REG_H

/* End of file -------------------------------------------------------- */
