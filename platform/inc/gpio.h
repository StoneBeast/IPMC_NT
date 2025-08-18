#ifndef __GPIO_H
#define __GPIO_H

void init_gpio(void);
uint8_t get_ga(void);
void ledOn(void);
void ledOff(void);
void ledTrigger(void);

#endif // !__GPIO_H
