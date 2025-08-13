/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-08-05 18:37:42
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-07 13:43:14
 * @Description  : 
 */

#include <string.h>

#include "platform.h"

#include "ipmi.h"
#include "ipmi_event.h"
#include "ipmi_protocol.h"

static ipmi_event event_queue[IPMI_EVENT_MAX_LEN] = {0};
static uint8_t queue_front = 0;
static uint8_t queue_rear = 0;

int init_ipmi_event(void)
{
    return 0;
}

uint8_t get_event_count(void)
{
    return ((queue_rear + IPMI_EVENT_MAX_LEN - queue_front) % IPMI_EVENT_MAX_LEN);
}

int add_event(const ipmi_event* event)
{
    if (((queue_rear + 1) % IPMI_EVENT_MAX_LEN) == queue_front) /* 队列满 */
        return -1;

    memcpy(&(event_queue[queue_rear]), event, sizeof(ipmi_event));
    queue_rear = ((queue_rear + 1) % IPMI_EVENT_MAX_LEN);

    return 0;
}

int get_event_item(ipmi_event *const event)
{
    if (queue_rear == queue_front)
        return -1;

    memcpy(event, &(event_queue[queue_front]), sizeof(ipmi_event));
    queue_front = ((queue_front+1)%IPMI_EVENT_MAX_LEN);

    return 0;
}

