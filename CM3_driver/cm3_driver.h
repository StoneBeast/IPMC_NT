#ifndef __CM3_DRIVER_H
#define __CM3_DRIVER_H

#include "platformConfig.h"

typedef enum {
    IT_DISABLE = 0,
    IT_ENABLE,
} IT_FUNCTION;

void Systick_init(uint32_t tick_rate_hz, IT_FUNCTION func);
void increment_tick(void);
uint32_t get_ticks(void);

#endif // !__CM3_DRIVER_H
