#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "stm32f10x.h"
#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "uart.h"
#include "adc.h"
#include "cm3_driver.h"
#include "mem.h"
#include "nct75.h"


#define PRINTF(...) printf(__VA_ARGS__)
void delay_ms(uint32_t ms);

#endif // !__PLATFORM_H
