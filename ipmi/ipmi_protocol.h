/*
 * @Author       : stoneBeast
 * @Date         : 2025-08-06 17:06:19
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-08 09:34:22
 * @Description  : 
 */
#ifndef __IPMI_PROTOCOL_H
#define __IPMI_PROTOCOL_H

#include <stdint.h>
#include "ipmi_event.h"

#define IPMI_SEND_CMP_BIT                   (0)

#define IPMI_PROTOCOL_MAX_LEN               32
#define IPMI_PROTOCOL_FORMAT_LEN            3
#define IPMI_PROTOCOL_DATA_MAX_LEN          (IPMI_PROTOCOL_MAX_LEN - IPMI_PROTOCOL_FORMAT_LEN)

#define IPMI_PROTOCOL_MSG_TYPE_OFFSET       0
#define IPMI_PROTOCOL_MSG_TYPE_LEN          1
#define IPMI_PROTOCOL_MSG_CODE_OFFSET       1
#define IPMI_PROTOCOL_MSG_CODE_LEN          1
#define IPMI_PROTOCOL_MSG_DATA_LEN_OFFSET   2
#define IPMI_PROTOCOL_MSG_DATA_LEN_LEN      1
#define IPMI_PROTOCOL_MSG_DATA_OFFSET       3
#define IPMI_PROTOCOL_MSG_CHK_OFFSET        (IPMI_PROTOCOL_MAX_LEN-1)
#define IPMI_PROTOCOL_MSG_CHK_LEN           1

void init_ipmi_protocol(void);
int check_msg(const uint8_t *msg);
int add_request(const uint8_t* req);
int ipmi_response(uint8_t addr, uint8_t code, const uint8_t* msg, uint16_t msg_len);
int ipmi_event_send(const ipmi_event* event);
int get_req_item(uint8_t *const req);

#define IPMB_BUSY_TRY_COUNT                 (10)
#define IPMB_BUSY_TRY_INTERVAL_MS           (10)
// #define IPMB

#define IPMI_ERR_OK                         (0)
#define IPMI_ERR_TIMEOUT                    (-1)
#define IPMI_ERR_BUSY                       (-2)
#define IPMI_ERR_NO_DEVICE                  (-3)
#define IPMI_ERR_MSG_ERROR                  (-4)
#define IPMI_ERR_RES_TIMEOUT                (-5)

#define IPMI_MSG_TYPE_REQ                   (1)
#define IPMI_MSG_TYPE_RES                   (2)
#define IPMI_MSG_TYPE_EVENT                 (3)


#endif // !__IPMI_PROTOCOL_H
