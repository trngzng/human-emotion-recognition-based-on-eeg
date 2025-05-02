/**
 * @file       sys_data_mng.c
 * @copyright  Copyright (C) 2024 GNG. All rights reserved.
 * @license    None.
 * @version    1.0.0
 * @date       2025-05-02
 * @author     Giang Phan
 *             
 * @brief      Handle the system data manager based on the PubSub pattern.
 *             
 * @note          
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include "sys_data_mng.h"

/* Private defines ---------------------------------------------------- */
#define SYS_DATA_MNG_BUFF_SIZE  (255) /*!< Description of PRIVATE_DEFINE_1 */

/* Private enumerate/structure ---------------------------------------- */
/**
 * @brief <enum descriptiton>
 */
typedef enum 
{
  PRIVATE_ENUM_1, /**< Description of PRIVATE_ENUM_1 */
  PRIVATE_ENUM_2, /**< Description of PRIVATE_ENUM_2 */
  PRIVATE_ENUM_3  /**< Description of PRIVATE_ENUM_3 */
} private_enum_type_t;

/**
 * @brief <structure descriptiton>
 */
typedef struct 
{
  SYS_DATA_MNG_IDTopicTypeDef id;     /**< Description of member_1 */
  CB_HandleTypeDef cbuf;
  uint32_t num_of_subscribers;        /**< Description of member_2 */
  uint32_t message_size;
  SYS_DATA_MNG_CbFunc cb_func[SYS_DATA_MNG_MAX_SUBSCRIBERS]; /**< Description of member_3 */
} SYS_DATA_MNG_TopicTypeDef;

/* Private macros ----------------------------------------------------- */
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
#define PRIVATE_MACRO(a)  do_something_with(a)

/* Public variables --------------------------------------------------- */
int g_var_1;
int g_var_2;

/* Private variables -------------------------------------------------- */
static SYS_DATA_MNG_TopicTypeDef sTopicList[SYS_DATA_MNG_MAX_TOPIC];    /**< The system data manager topics */

static uint8_t sMsgBroker[SYS_DATA_MNG_BUFF_SIZE];                  /**< The message buffer for the topics */
static uint32_t sMsgBrokerTopPtr = 0;                               /**< The pointer to the top of the message buffer */
static uint8_t sCbufReadBuffer[SYS_DATA_MNG_BUFF_SIZE];             /**< Buffer to store data from circular buffer */
static SYS_DATA_MNG_IDTopicTypeDef sNumOfTopics = 0;                /**< Current number of topics in the system data */

/* Private function prototypes ---------------------------------------- */
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
static void private_function(void);
/* Function definitions ----------------------------------------------- */

void SYS_DATA_MNG_Init(void)
{
  memset(&sTopicList, 0, sizeof(sTopicList));
  memset(sCbufReadBuffer, 0, sizeof(sCbufReadBuffer));
}

BaseStatusTypeDef SYS_DATA_MNG_CreateTopic(SYS_DATA_MNG_IDTopicTypeDef id, uint32_t msg_size, uint32_t num_of_msg)
{
  BaseStatusTypeDef ret = BS_ERROR;

  __ASSERT(id < SYS_DATA_MNG_MAX_TOPIC, BS_ERROR);
  __ASSERT(sNumOfTopics < SYS_DATA_MNG_MAX_TOPIC, BS_ERROR);

  __ASSERT(sTopicList[id].cbuf.active == BS_FALSE, BS_ERROR);
  __ASSERT(msg_size > 0, BS_ERROR);
  __ASSERT(num_of_msg > 0, BS_ERROR);
  __ASSERT((msg_size * num_of_msg) <= (SYS_DATA_MNG_BUFF_SIZE - sMsgBrokerTopPtr), BS_ERROR);

  ret = CB_Init(&sTopicList[id].cbuf, sMsgBroker, msg_size * num_of_msg);
  __ASSERT(ret == CB_SUCCESS, BS_ERROR);

  sTopicList[id].num_of_subscribers = 0;
  sTopicList[id].message_size = msg_size;
  sTopicList[id].id = id;

  sMsgBrokerTopPtr += (msg_size * num_of_msg);
  sNumOfTopics++;

  return BS_OK;
}

BaseStatusTypeDef SYS_DATA_MNG_SubscribeTopic(SYS_DATA_MNG_IDTopicTypeDef id, SYS_DATA_MNG_CbFunc cb_func)
{
  __ASSERT(id < SYS_DATA_MNG_MAX_TOPIC, BS_ERROR);
  __ASSERT(sTopicList[id].cbuf.active == BS_TRUE, BS_ERROR);
  __ASSERT(id < sNumOfTopics, BS_ERROR);
  __ASSERT(cb_func != NULL, BS_ERROR);
  __ASSERT(sTopicList[id].num_of_subscribers < SYS_DATA_MNG_MAX_SUBSCRIBERS, BS_ERROR);
  for (uint_fast8_t i = 0; i < SYS_DATA_MNG_MAX_SUBSCRIBERS; i++)
  {
    __ASSERT(sTopicList[id].cb_func[i] != cb_func, BS_ERROR);
  }
  
  sTopicList[id].cb_func[sTopicList[id].num_of_subscribers] = cb_func;
  sTopicList[id].num_of_subscribers++;

  return BS_OK;
}

/* Private definitions ------------------------------------------------ */
static void private_function(void)
{
}
/* End of file -------------------------------------------------------- */
