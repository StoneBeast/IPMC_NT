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
