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
#include "bsp_dwt.h"

/* Private defines ---------------------------------------------------- */
#define PACKET_SOP  (0x55U)
#define PACKET_EOP  (0xAAU)

/* Private enumerate/structure ---------------------------------------- */

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
  uint8_t sop[3];
  uint8_t length;
  uint8_t cmd;
  uint8_t data[56];
  uint8_t eop[3];
} SYS_SERIAL_PacketTypeDef;

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* Private variables -------------------------------------------------- */
static SYS_SERIAL_HandleTypeDef sSerial; /**< System serial instance */

SYS_SERIAL_PacketTypeDef packetBuf = {{PACKET_SOP, PACKET_SOP, PACKET_SOP},
                                      SYS_SERIAL_CMD_DEVICE_MODE,
                                      0,
                                      {0},
                                      {PACKET_EOP, PACKET_EOP, PACKET_EOP}};

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
BaseStatusTypeDef SYS_Serial_Init()
{
  __ASSERT(sSerial.status == SYS_SERIAL_RESET, BS_ERROR);
  __ASSERT(&hUsbDeviceFS != NULL, BS_ERROR);
  __ASSERT(hUsbDeviceFS.pData != NULL, BS_ERROR);

  sSerial.pDev = &hUsbDeviceFS;
  sSerial.status = SYS_SERIAL_READY;


  return BS_OK;
}

BaseStatusTypeDef SYS_Serial_SendSamples(uint8_t *psample, uint8_t num)
{
  __ASSERT(sSerial.status == SYS_SERIAL_READY, BS_ERROR);
  __ASSERT(psample != NULL, BS_ERROR);
  __ASSERT(num > 0, BS_ERROR);
  uint8_t result;

  memcpy(packetBuf.data, psample, num);
  packetBuf.length = num;

  result = CDC_Transmit_FS((uint8_t *)&packetBuf, sizeof(packetBuf));

  __ASSERT(result == 0, BS_ERROR);

  return BS_OK;
}

BaseStatusTypeDef SYS_Serial_PrintSamples(uint8_t *data, uint8_t num)
{
  __ASSERT(sSerial.status == SYS_SERIAL_READY, BS_ERROR);
  __ASSERT(data != NULL, BS_ERROR);
  __ASSERT(num > 0, BS_ERROR);

  // Ép kiểu con trỏ sang int32_t*
  int32_t *samples = (int32_t *)data;

  char msg[256];
  uint16_t pos = 0;

  // Timestamp
  float t_us = BSP_DWT_CyclesToUs(BSP_DWT_GetCycles());
  int n = snprintf(msg, sizeof(msg), "TS: %.2f us | ", t_us);
  pos += n;
  // Gửi USB
  printf("%s", msg);

  return BS_OK;
}



/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
