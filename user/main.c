/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-08-06 17:00:28
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-13 19:03:02
 * @Description  : 
 */
#include "platform.h"
#include "cm3_driver.h"

#include "ipmi.h"
#include "ipmi_event.h"
#include "ipmi_protocol.h"
#include "soft_timer.h"

static void blink_task(void);
static void msg_handler(void);
static void update_sensor_handler(void);

int main(void)
{
    timer_item blink;
    timer_item ipmi_msg;
    timer_item update_sens;

    Systick_init(1000, IT_ENABLE);
    
    init_gpio();
    init_uart();
    
    ipmc_init();

    update_sens.interval = 2000;
    update_sens.timer_id = 0;
    update_sens.timeout_handler = update_sensor_handler;
    
    ipmi_msg.interval = 2;
    ipmi_msg.timer_id = 1;
    ipmi_msg.timeout_handler = msg_handler;
    

    blink.interval = 500;
    blink.timer_id = 2;
    blink.timeout_handler = blink_task;

    add_timer(&update_sens);
    add_timer(&ipmi_msg);
    add_timer(&blink);
    
    // TODO: 定时器的添加使能等使用的id实际上是对应定时器在定时器数组中的角标，因此修改添加等需要保持一致
    timer_start(0);
    timer_start(1);
    timer_start(2);

    timeout_handler();

    while (1);
}

static void blink_task(void)
{
    ledTrigger();
    PRINTF("hello --new platform\r\n");
}

/*** 
 * @brief 定时更新传感器数据任务
 * @return [void]
 */
static void update_sensor_handler(void)
{
    update_sensor();
}

/*** 
 * @brief 请求处理任务
 * @return [void]
 */
static void msg_handler(void)
{
    ipmi_msg_handler();
}

