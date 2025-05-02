/**
 * @file       cbuffer.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       26/06/2024
 * @author     Minh Phung Nhat
 *             Hung Pham Duc
 *             Khoi Nguyen Thanh
 * 					 	 Giang Phan Truong
 * @brief      Circular Buffer
 *             This Circular Buffer is safe to use in IRQ with single reader,
 *             single writer. No need to disable any IRQ.
 *
 *             Capacity = <size> - 1
 * @note       None
 */
/* Define to prevent recursive inclusion ------------------------------ */
#include "cbuffer.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */
/**
 * @brief           Write 1 byte to circular buffer
 *
 * @param[in]       cb      Pointer to a cbuffer_t structure
 * @param[in]       byte    Data to write
 *
 * @return
 *  - (0) : Success
 *  - (-1): Error
 */
static uint32_t CB_WriteByteData(CB_HandleTypeDef *cb, uint8_t byte);

/**
 * @brief           Read 1 byte from circular buffer
 *
 * @param[in]       cb      Pointer to a cbuffer_t structure
 * @param[in]       byte    Pointer to data buffer
 *
 * @return
 *  - (0) : Success
 *  - (-1): Error
 */
static uint32_t CB_ReadByteData(CB_HandleTypeDef *cb, uint8_t *byte);

/* Function definitions ----------------------------------------------- */
uint32_t CB_Init(CB_HandleTypeDef *cb, void *buf, uint32_t size)
{
	if (cb == NULL)
		return CB_ERROR;
	if (buf == NULL)
		return CB_ERROR;
	if (size >= CB_MAX_SIZE)
		return CB_ERROR;

	cb->data = buf;
	cb->size = size;
	cb->writer = 0;
	cb->reader = 0;
	cb->overflow = 0;
	cb->active = 1;

	return CB_SUCCESS;
}

uint32_t CB_Clear(CB_HandleTypeDef *cb)
{
	if (cb == NULL)
		return CB_ERROR;

	cb->writer = 0;
	cb->reader = 0;
	cb->overflow = 0;

	return CB_SUCCESS;
}

uint32_t CB_ReadData(CB_HandleTypeDef *cb, void *buf, uint32_t nbytes)
{
	int data_count = 0;
	int num_avail_bytes = 0;
	if (cb == NULL)
		return CB_ERROR;

	if (buf == NULL)
		return CB_ERROR;

	if (!cb->active)
		return CB_ERROR;
	cb->active = 0;

	data_count = CB_DataCount(cb);
	if (data_count >= nbytes)
		num_avail_bytes = nbytes;
	else
		num_avail_bytes = data_count;

	for (int i = 0; i < num_avail_bytes; i++)
	{
		CB_ReadByteData(cb, (uint8_t *)buf + i);
	}

	cb->active = 1;
	return num_avail_bytes;
}

uint32_t CB_WriteData(CB_HandleTypeDef *cb, void *buf, uint32_t nbytes)
{
	int space_count = 0;
	int num_avail_bytes = 0;
	if (cb == NULL)
		return CB_ERROR;

	if (buf == NULL)
		return CB_ERROR;

	if (!cb->active)
		return CB_ERROR;
	cb->active = 0;

	space_count = CB_SpaceCount(cb);
	if (space_count >= nbytes)
	{
		num_avail_bytes = nbytes;
		cb->overflow = 0;
	}
	else
	{
		num_avail_bytes = space_count;
		cb->overflow = nbytes - space_count;
	}

	for (int i = 0; i < num_avail_bytes; i++)
	{
		CB_WriteByteData(cb, *((uint8_t *)buf + i));
	}

	cb->active = 1;
	return num_avail_bytes;
}

uint32_t CB_DataCount(CB_HandleTypeDef *cb)
{
	int res = 0;

	if (cb == NULL)
		return CB_ERROR;

	if (cb->writer >= cb->reader)
		res = cb->writer - cb->reader;
	else
		res = cb->size - cb->reader + cb->writer;

	return res;
}

uint32_t CB_SpaceCount(CB_HandleTypeDef *cb)
{
	int res = 0;

	if (cb == NULL)
		return CB_ERROR;

	if (cb->reader > cb->writer)
		res = cb->reader - cb->writer - 1;
	else if (cb->reader < cb->writer)
		res = cb->size - cb->writer + cb->reader - 1;
	else
		res = cb->size - 1;

	return res;
}

/* Private definitions ----------------------------------------------- */
static uint32_t CB_WriteByteData(CB_HandleTypeDef *cb, uint8_t byte)
{
	uint32_t next = cb->writer + 1;

	if (next == cb->size)
		next = 0;

	if (next == cb->reader)
		return CB_ERROR;

	*(cb->data + cb->writer) = byte;
	cb->writer = next;
	return CB_SUCCESS;
}

static uint32_t CB_ReadByteData(CB_HandleTypeDef *cb, uint8_t *byte)
{
	uint32_t next = cb->reader + 1;

	if (cb->reader == cb->writer)
		return CB_ERROR;

	if (next == cb->size)
		next = 0;

	*byte = *(cb->data + cb->reader);
	cb->reader = next;
	return CB_SUCCESS;
}

/* End of file -------------------------------------------------------- */