/*** 
 * @Author       : stoneBeast
 * @Date         : 2025-08-06 17:00:27
 * @Encoding     : UTF-8
 * @LastEditors  : stoneBeast
 * @LastEditTime : 2025-08-18 18:10:20
 * @Description  : 
 */
#include "platform.h"
void init_gpio(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOC, &GPIO_InitStruct);

}

static void init_ga_pin(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

}

uint8_t get_ga(void)
{
    uint8_t ga = 0;
    uint8_t temp = 0;

    init_ga_pin();
    
    temp = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
    ga &= (~(0x01));
    ga |= temp;

    temp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
    ga &= (~(0x02));
    ga |= (temp << 1);

    temp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
    ga &= (~(0x04));
    ga |= (temp << 2);

    temp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
    ga &= (~(0x08));
    ga |= (temp << 3);

    temp = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
    ga &= (~(0x10));
    ga |= (temp << 4);

    return ga;
}

void ledOn(void) 
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
}

void ledOff(void) 
{
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

void ledTrigger(void)
{
    uint8_t state = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(!state));
}
