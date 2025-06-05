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
#define PACKET_SOP      (0x55U)
#define PACKET_EOP      (0xAAU)
#define SOP_LEN         (3)
#define EOP_LEN         (3)
#define CMD_LEN         (1)
#define LENGTH_LEN      (1)
#define PAYLOAD_MAX_LEN (255)
#define PACKET_MAX_LEN  (SOP_LEN + CMD_LEN + LENGTH_LEN + PAYLOAD_MAX_LEN + EOP_LEN)

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


/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* Private variables -------------------------------------------------- */
static SYS_SERIAL_HandleTypeDef sSerial; /**< System serial instance */
static uint8_t packetBuf[PACKET_MAX_LEN];

/* Private function prototypes ---------------------------------------- */

/**
 * @brief  Initialize system acquisition manager
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return
 *  - Actual length of the output packet (in byte).
 */
uint16_t BuildPacket(uint8_t *packet, SYS_SERIAL_CommandTypeDef type, const uint8_t *payload, uint8_t len);

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

BaseStatusTypeDef SYS_Serial_SendRawSamples(uint8_t *sample, uint32_t size)
{
  __ASSERT(sSerial.status == SYS_SERIAL_READY, BS_ERROR);
  __ASSERT(sample != NULL, BS_ERROR);
  __ASSERT(size > 0, BS_ERROR);
  uint8_t result;

  sSerial.status = SYS_SERIAL_BUSY;
  uint16_t len = BuildPacket(packetBuf, DEVICE_SEND_RAW_SAMPLES, sample, size);

  result = CDC_Transmit_FS(packetBuf, len);
  sSerial.status = SYS_SERIAL_READY;

  __ASSERT(result == 0, BS_ERROR);


  return BS_OK;
}

BaseStatusTypeDef SYS_Serial_SendFilteredSamples(uint8_t *sample, uint32_t size)
{
  __ASSERT(sSerial.status == SYS_SERIAL_READY, BS_ERROR);
  __ASSERT(sample != NULL, BS_ERROR);
  __ASSERT(size > 0, BS_ERROR);
  uint8_t result;

  uint16_t len = BuildPacket(packetBuf, DEVICE_SEND_FILTERED_SAMPLES, sample, size);

  result = CDC_Transmit_FS(packetBuf, len);

  __ASSERT(result == 0, BS_ERROR);

  return BS_OK;
}

/* Private definitions ----------------------------------------------- */
uint16_t BuildPacket(uint8_t *packet, SYS_SERIAL_CommandTypeDef type, const uint8_t *payload, uint8_t len)
{
  __ASSERT(packet != NULL, 0xFFFF);
  __ASSERT(payload != NULL, 0xFFFF);
  __ASSERT(len <= 0xFF, 0xFFFF);

/**
 * Packet format:
 * .----------------.---------.-------------------.------------------.----------------.
 * |Start of Packet | Command | Length (in Bytes) |       Data       |  End of Packet |
 * :----------------+---------+-------------------+------------------+----------------:
 * |    0x555555    |  1-byte |      1-byte       |    0-255 Bytes   |    0xAAAAAA    |
 * '----------------'---------'-------------------'------------------'----------------'
**/

  memset(packet, PACKET_SOP, SOP_LEN);
  packet[SOP_LEN] = type;
  packet[SOP_LEN + CMD_LEN] = len;
  memcpy(packet + SOP_LEN + CMD_LEN + LENGTH_LEN, payload, len);
  memset(packet + SOP_LEN + CMD_LEN + LENGTH_LEN + len, PACKET_EOP, EOP_LEN);

  return SOP_LEN + CMD_LEN + LENGTH_LEN + len + EOP_LEN;
}

/* End of file -------------------------------------------------------- */
