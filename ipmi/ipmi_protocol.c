
/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-07-29 15:14:19
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-08 09:33:05
 * @Description  : 
 */

#include "ipmi_protocol.h"
#include "ipmi.h"
#include "i2c.h"
#include "ipmi_event.h"
#include <string.h>

/* 
    IPMI Message Package
    | LENGTH 32Byte   |                 |                    |      |                  |
    | Msg Type(1Byte) | Msg Code(1Byte) | Data Length(1Byte) | Data | Check sum(1Byte) |
*/

// TODO: send和recv的接收策略需要进一步确认
// TODO: 发送不需要中断，阻塞即可

#define RECV_FROM_MSG_BUFFER(p_data, wait_ms)

static void get_chksum(uint8_t* msg);
static int ipmi_msg_send(uint8_t addr, uint8_t type, uint8_t code, const uint8_t* msg, uint16_t msg_len, uint32_t timeout_ms);

static uint8_t ipmi_req_queue[IPMI_REQ_QUEUE_MAX_LEN][IPMI_PROTOCOL_MAX_LEN] = {0};
static uint8_t queue_front = 0;
static uint8_t queue_rear = 0;


/*** 
 * @brief 准备ipmi通信需要使用到的结构体、空间等
 * @return [void]
 */
void init_ipmi_protocol(void)
{
    //TODO: 你得做点啥吧 大概
}

int ipmi_response(uint8_t addr, uint8_t code, const uint8_t* msg, uint16_t msg_len)
{
    return ipmi_msg_send(addr, IPMI_MSG_TYPE_RES, code, msg, msg_len, 200);
}

int ipmi_event_send(const ipmi_event* event)
{
    return ipmi_msg_send(BMC_ADDR, IPMI_MSG_TYPE_EVENT, 0x01, (uint8_t*)event, sizeof(ipmi_event), 200);
}

/***
 * @brief 发送ipmi消息
 * @param addr [uint8_t]        目标地址
 * @param type [uint8_t]        消息类型代码
 * @param code [uint8_t]        消息功能码
 * @param msg [uint8_t*]        消息体
 * @param msg_len [uint16_t]    消息体长度
 * @param timeout_ms [uint32_t] 超时时间，单位ms
 * @return [int]                成功返回IPMI_ERR_OK或错误码
 */
static int ipmi_msg_send(uint8_t addr, uint8_t type, uint8_t code, const uint8_t* msg, uint16_t msg_len, uint32_t timeout_ms)
{
    int ret = IPMI_ERR_OK;
    int i2c_ret = I2C_ERR_OK;
    uint16_t try_count = 0;
    uint8_t send_msg[IPMI_PROTOCOL_MAX_LEN] = {0};

    /* 判断msg */
    if (msg == NULL || msg_len > IPMI_PROTOCOL_DATA_MAX_LEN) {
        return IPMI_ERR_MSG_ERROR;
    }

    /* 先判断I2C是否busy */
    while (1 == I2C_busy_status()) {
        if (try_count > IPMB_BUSY_TRY_COUNT) {
            ret = IPMI_ERR_BUSY;
            goto SEND_END;
        }
        delay_ms(IPMB_BUSY_TRY_INTERVAL_MS);
        try_count++;
    }

    /* 填充IPMI消息固定成员 */
    memcpy(&(send_msg[IPMI_PROTOCOL_MSG_TYPE_OFFSET]), &type, IPMI_PROTOCOL_MSG_TYPE_LEN);
    memcpy(&(send_msg[IPMI_PROTOCOL_MSG_CODE_OFFSET]), &code, IPMI_PROTOCOL_MSG_CODE_LEN);
    memcpy(&(send_msg[IPMI_PROTOCOL_MSG_DATA_LEN_OFFSET]), &msg_len, IPMI_PROTOCOL_MSG_DATA_LEN_LEN);
    memcpy(&(send_msg[IPMI_PROTOCOL_MSG_DATA_OFFSET]), msg, msg_len);
    /* 添加校验和 */
    get_chksum(send_msg);

    i2c_ret = I2C_satrt_send(addr, send_msg);
    if (i2c_ret == I2C_ERR_SB) {
        ret = IPMI_ERR_TIMEOUT;
        goto SEND_END;
    } else if (i2c_ret == I2C_ERR_ADDR) {
        ret = IPMI_ERR_NO_DEVICE;
        I2C_reset();
        goto SEND_END;
    }

    ret = i2c_send_complate(timeout_ms);

SEND_END:
    return ret;
}

/*** 
 * @brief 校验消息是否有误
 * @param msg [uint8_t*]    被校验的消息  
 * @return [int]            0：校验通过
 */
int check_msg(const uint8_t* msg)
{
    uint16_t sum = 0;
    uint8_t i = 0;

    for (i = 0; i < IPMI_PROTOCOL_MAX_LEN; i++) {
        sum += msg[i];
    }

    if (sum%256 == 0) {
        return 0;
    } else {
        return -1;
    }

}

/*** 
 * @brief 向消息中直接添加校验码
 * @param msg [uint8_t*]    指向消息的指针
 * @return [void]
 */
static void get_chksum(uint8_t *msg)
{
    uint16_t sum = 0;
    uint8_t chk = 0;

    for (uint16_t i = 0; i < (IPMI_PROTOCOL_MAX_LEN-1); i++)
        sum += msg[i];

    chk = (0x100 - sum % 0x100);

    msg[IPMI_PROTOCOL_MSG_CHK_OFFSET] = chk;
}

uint8_t get_queue_count(void)
{
    return ((queue_rear+IPMI_REQ_QUEUE_MAX_LEN-queue_front) % IPMI_REQ_QUEUE_MAX_LEN);
}

int add_request(const uint8_t* req)
{
    if (((queue_rear +1)%IPMI_REQ_QUEUE_MAX_LEN) == queue_front) /* 队列满 */
        return -1;

    if (check_msg(req) != 0) /* 校验不通过 */
        return -2;

    memcpy(ipmi_req_queue[queue_rear], req, IPMI_PROTOCOL_MAX_LEN);
    queue_rear = ((queue_rear + 1) % IPMI_REQ_QUEUE_MAX_LEN);

    return 0;
}

int get_req_item(uint8_t *const req)
{
    if (queue_rear == queue_front)
        return -1;

    memcpy(req, ipmi_req_queue[queue_front], IPMI_PROTOCOL_MAX_LEN);
    queue_front = ((queue_front + 1) % IPMI_REQ_QUEUE_MAX_LEN);

    return 0;
}
