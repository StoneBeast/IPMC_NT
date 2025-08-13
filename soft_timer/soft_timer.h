/*
 * @Author       : stoneBeast
 * @Date         : 2025-08-07 15:28:19
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-07 16:39:25
 * @Description  : 
 */
#ifndef __SOFT_TIMER_H
#define __SOFT_TIMER_H

#include <stdint.h>

#define SOFT_TIMER_MAX_COUNT    4

typedef struct {
    uint8_t timer_status;
    uint16_t timer_id;
    uint8_t time_out;
    uint32_t interval;
    uint32_t timeout_ticks;
    void (*timeout_handler)(void);
}timer_item;

uint16_t get_timer_count(void);
int add_timer(const timer_item *timer);
void timeout_handler(void);
void is_timeout(uint32_t now_ticks);
void timer_start(uint16_t timer_id);

#endif // !__SOFT_TIMER_H
