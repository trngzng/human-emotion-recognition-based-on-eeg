/**
 * @file       sys_data_mng.h
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-02
 * @author     Giang Phan
 *             
 * @brief      Handle the system data manager based on the PubSub pattern.
 *             
 * @note       None. 
 * @example    None.
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_DATA_MNG_H
#define __SYS_DATA_MNG_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "cbuffer.h"
#include "common.h"
/* Public defines ----------------------------------------------------- */
#define SYS_DATA_MNG_MAX_SUBSCRIBERS  (10)  /**< The maximum number of subscribers for a topic */

typedef void (*SYS_DATA_MNG_CbFunc)(uint8_t *, uint32_t); /**< The callback function type for the subscribers */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief <enum descriptiton>
 */
typedef enum 
{
  SYS_DATA_MNG_TOPIC_ADC_DSP = 0x00,  /**< Description of PUBLIC_ENUM_1 */
  SYS_DATA_MNG_TOPIC_ADC_USB,         /**< Description of PUBLIC_ENUM_1 */
  SYS_DATA_MNG_TOPIC_DSP_USB,         /**< Description of PUBLIC_ENUM_1 */
  SYS_DATA_MNG_MAX_TOPIC
} SYS_DATA_MNG_IDTopicTypeDef;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Initialize the system data manager
 *
 * @param[in]     id          The ID of the topic to be created
 * @param[in]     msg_size    The size of the message in bytes
 * @param[in]     num_of_msg  The number of messages of this topic
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
void SYS_DATA_MNG_Init(void);

/**
 * @brief  Create new topic
 *
 * @param[in]     id          The ID of the topic to be created
 * @param[in]     msg_size    The size of the message in bytes
 * @param[in]     num_of_msg  The number of messages of this topic
 *
 * @attention  <API attention note>
 *
 * @return
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_DATA_MNG_CreateTopic(SYS_DATA_MNG_IDTopicTypeDef id, uint32_t msg_size, uint32_t num_of_msg);

/**
 * @brief  Subscribe to a topic
 *
 * @param[in]     id          The ID of the topic to be created
 * @param[in]     cb_func     The callback function to be called when a message is published
 *
 * @attention  <API attention note>
 *
 * @return
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_DATA_MNG_SubscribeTopic(SYS_DATA_MNG_IDTopicTypeDef id, SYS_DATA_MNG_CbFunc cb_func);

/**
 * @brief  Subscribe to a topic
 *
 * @param[in]     id          The ID of the topic to be created
 * @param[in]     msg         The message to be published
 * @param[in]     size        The size of the message in bytes
 *
 * @attention  <API attention note>
 *
 * @return
 *  - BS_OK: Success
 *  - BS_ERROR: Error
 */
BaseStatusTypeDef SYS_DATA_MNG_PublishMsg(SYS_DATA_MNG_IDTopicTypeDef id, uint8_t *msg, uint32_t size);

#endif // __SYS_DATA_MNG_H

/* End of file -------------------------------------------------------- */
