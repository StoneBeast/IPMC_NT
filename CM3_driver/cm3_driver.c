#include <stdint.h>
#include "cm3_driver.h"

#define configSYSTICK_CLOCK_HZ platformCPU_CLOCK_HZ

#define portNVIC_SYSTICK_CTRL_REG           ( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG           ( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG  ( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI2_REG                ( * ( ( volatile uint32_t * ) 0xe000ed20 ) )

#define portNVIC_SYSTICK_CLK_BIT	        ( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT		( 1UL << 16UL )

static volatile uint32_t xTickCount = 0;

void Systick_init(uint32_t tick_rate_hz, IT_FUNCTION func)
{
    /* Stop and clear the SysTick. */
	portNVIC_SYSTICK_CTRL_REG = 0UL;
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

    portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / tick_rate_hz ) - 1UL;
    
    if (func == IT_ENABLE) {
        /* Configure SysTick to interrupt at the requested rate. */
        portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );
    } else {
        portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_ENABLE_BIT );
    }
	
}

void increment_tick(void)
{
    const uint32_t tick_count_const = xTickCount + 1;
    if (tick_count_const == 0) {
        /* overflow */
    }
    
    xTickCount = tick_count_const;
}

void delay_ms(uint32_t ms)
{
    const uint32_t until_ms = xTickCount + ms;
    
    while (until_ms > xTickCount);
}

uint32_t get_ticks(void)
{
    const uint32_t ticks = xTickCount;

    return ticks;
}
