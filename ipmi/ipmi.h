#ifndef __IPMI_H
#define __IPMI_H

#define BMC_ADDR                0x20
#define IPMC_BASE_ADDR          0x40
#define IPMI_EVENT_MAX_LEN      10
#define IPMI_REQ_QUEUE_MAX_LEN  5
#define SENSOR_MAX_NUMBER       4

#define IPMI_MSG_CODE_SCAN      (0x01)
#define IPMI_MSG_CODE_GET_SDR   (0x02)

void ipmc_init(void);
void ipmi_msg_handler(void);

#endif // !__IPMI_H
