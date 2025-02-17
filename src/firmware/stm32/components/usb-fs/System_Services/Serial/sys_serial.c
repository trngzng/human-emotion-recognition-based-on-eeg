/**
 * @file       sys_serial.c
 * @copyright  None.
 * @license    None.
 * @version    1.0.0
 * @date       2025-02-10
 * @author     Giang Phan
 *             
 * @brief      <A brief description of the content of the file>
 *             
 * @note       None.
 * @example    None.
 */

/* Includes ----------------------------------------------------------- */
#include "sys_serial.h"
#include "common.h"
#include "usbd_cdc_if.h"

/* Private defines ---------------------------------------------------- */
#define PACKET_SOP  (0xAAU)
#define PACKET_EOP  (0x5555U)

/* Private enumerate/structure ---------------------------------------- */
/**
 * Packet format:
 * .----------------.---------.-------------------.------------------.--------.----------------.
 * |Start of Packet | Command | Length (in Bytes) |       Data       |  CRC   |  End of Packet |
 * :----------------+---------+-------------------+------------------+--------+----------------:
 * |      0xAA      |  3-bit  |  5-bit (max 32)   | (1 - 32) x 8-bit | 16-bit |     0x5555     |
 * '----------------'---------'-------------------'------------------'--------'----------------'
 * Packet length (max): 37 bytes
 * Note Length field value = 0 means Data field has 1 byte.
**/
typedef struct __attribute__((packed))
{
  uint8_t sop;
  uint8_t length : 5;
  uint8_t cmd : 3;
  uint8_t data[32];
  uint16_t crc;
  uint16_t eop;
} SYS_SERIAL_PacketTypeDef;

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
SYS_SERIAL_PacketTypeDef packetBuf = {PACKET_SOP,
                                      SYS_SERIAL_CMD_DEVICE_MODE,
                                      0,
                                      {0},
                                      0,
                                      PACKET_EOP};

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
SYS_StatusTypeDef SYS_Serial_Init(SYS_SERIAL_HandleTypeDef *pserial, USBD_HandleTypeDef *pdev)
{
  __ASSERT(pserial != NULL, SYS_ERROR);
  __ASSERT(pserial->status == SYS_SERIAL_RESET, SYS_OK);
  __ASSERT(pdev != NULL, SYS_ERROR);
  __ASSERT(pdev->pData != NULL, SYS_ERROR);

  pserial->pDev = pdev;
  pserial->status = SYS_SERIAL_READY;

  return SYS_OK;
}

SYS_StatusTypeDef SYS_Serial_SendSamples(SYS_SERIAL_HandleTypeDef *pserial, float *psample, uint8_t num)
{
  __ASSERT(pserial != NULL, SYS_ERROR);
  __ASSERT(pserial->status == SYS_SERIAL_READY, SYS_TIMEOUT);
  __ASSERT(psample != NULL, SYS_ERROR);
  __ASSERT((num > 0) && (num <= 8), SYS_ERROR);

  uint8_t result;

  for (uint_fast8_t i = 0; i < num; i++)
  {
    uint32_t *pTemp = (uint32_t *)(psample + i);

    packetBuf.data[0 + i*sizeof(float)] = (uint8_t)(*pTemp >> 24);
    packetBuf.data[1 + i*sizeof(float)] = (uint8_t)(*pTemp >> 16);
    packetBuf.data[2 + i*sizeof(float)] = (uint8_t)(*pTemp >> 8);
    packetBuf.data[3 + i*sizeof(float)] = (uint8_t)*pTemp;
  }
  packetBuf.length = num*sizeof(float) - 1;

  result = CDC_Transmit_FS((uint8_t *)&packetBuf, sizeof(packetBuf));
  __ASSERT(result == 0, SYS_ERROR);

  return SYS_OK;
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
