/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-08-07 15:28:29
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-11 10:22:37
 * @Description  : 
 */
#include "soft_timer.h"
#include <string.h>
#include "cm3_driver.h"

static uint16_t timer_count = 0;
timer_item timer_list[SOFT_TIMER_MAX_COUNT];

uint16_t get_timer_count(void)
{
    return timer_count;
}

int add_timer(const timer_item *timer)
{
    if (timer_count >= SOFT_TIMER_MAX_COUNT)
        return -1;

    memcpy(&(timer_list[timer_count]), timer, sizeof(timer_item));
    timer_list[timer_count].timer_status = 0;
    timer_list[timer_count].time_out = 0;
    timer_count++;
    return 0;
}

void is_timeout(uint32_t now_ticks)
{
    uint16_t i = 0;

    for (i = 0; i < timer_count; i++)
    {
        if ((timer_list[i].time_out == 0) && (((int32_t)(timer_list[i].timeout_ticks - now_ticks)) <= 0) && (timer_list[i].timer_status == 1)) {
            timer_list[i].time_out = 1;
        }
    }
}

void timeout_handler(void)
{
    uint16_t i = 0;

    while (1) {
        for (i = 0; i < timer_count; i++)
        {
            if ((timer_list[i].time_out == 1) && (timer_list[i].timer_status == 1)) {
                timer_list[i].timeout_handler();
                timer_list[i].timeout_ticks = (timer_list[i].interval+get_ticks());

                timer_list[i].time_out = 0;
            }
        }
            
    }
}

void timer_start(uint16_t timer_id)
{
    if (timer_id >= SOFT_TIMER_MAX_COUNT)
        return;

    timer_list[timer_id].timeout_ticks = get_ticks()+timer_list[timer_id].interval;
    timer_list[timer_id].timer_status = 1;
}

