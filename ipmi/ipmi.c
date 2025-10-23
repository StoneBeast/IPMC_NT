/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-07-29 15:15:04
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-13 14:35:44
 * @Description  : 
 */

#include "platform.h"
#include "ipmi.h"
#include "ipmi_protocol.h"
#include "ipmi_event.h"
#include <string.h>
#include "ipmi_sdr.h"

static void get_ipmb_addr(void);

uint8_t g_addr = 0;

void ipmc_init(void)
{
    int init_ret = 0;

    get_ipmb_addr();
    init_ipmi_i2c(g_addr);
    init_ipmi_protocol();
    init_ret = init_ipmi_event();
    if (init_ret != 0)
        while(1);

    init_sensor_i2c();
    init_ipmi_sdr();
}

static void get_ipmb_addr(void)
{
    uint8_t ga = get_ga();
    uint8_t temp_addr = 0;
    
    ga += IPMC_BASE_ADDR;
    temp_addr = (ga<<1);
    
    //TODO: 做temp_addr合法性校验
    g_addr = temp_addr;
}

static uint8_t get_version_info_handler(char* const info)
{
    uint8_t info_len = 0;

    sprintf((char*)info, "Version %d.%d.%d Built on %s %s", MAIN_VERSION, SUB_VERSION, FIX_VERSION, __DATE__, __TIME__);
    info_len = strlen((char*)info);

    return info_len;
}

void ipmi_msg_handler(void)
{
    uint8_t req[IPMI_PROTOCOL_MAX_LEN] = {0};
    uint8_t res[IPMI_PROTOCOL_MAX_LEN] = {0};

    const char* temp_res = "hello";
    int res_ret = IPMI_ERR_OK;
    uint8_t temp_data = 0;
    uint8_t res_body_len = 0;

    while (get_req_item(req) == 0) {
        /* 处理请求 */
        uint8_t code = req[IPMI_PROTOCOL_MSG_CODE_OFFSET];
        uint8_t data_len = req[IPMI_PROTOCOL_MSG_DATA_LEN_OFFSET];

        PRINTF("get req | code: %#02x, req_len: %d\r\n", code, data_len);

        switch (code)
        {
        case IPMI_MSG_CODE_SCAN:
            res[0] = g_addr;
            res_body_len = 1;
            break;
        case IPMI_MSG_CODE_GET_SDR:
            temp_data = get_sdr_by_id(req[IPMI_PROTOCOL_MSG_DATA_OFFSET], (ipmi_sdr*)(&(res[1])));
            res[0] = temp_data;
            res_body_len = 1+sizeof(ipmi_sdr);
            break;
        case IPMI_MSG_CODE_GET_VERSION:
            res_body_len = get_version_info_handler((char*)res);
            break;
        default:
            memcpy(req, temp_res, strlen(temp_res));
            res_body_len = strlen(temp_res);
            break;
        }

        res_ret = ipmi_response(BMC_ADDR, code, res, res_body_len);

        if (res_ret != IPMI_ERR_OK) {
            PRINTF("res error: %d\r\n", res_ret);
        }
    }
}

