/*
 * Copyright (c) 2019, yolee599
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     yolee599     first version
 * 2020-04-26     yolee599     refactor
 */

#include "stm32f10x.h"
#include "yo_blinker.h"

static yo_blinker_t m_led_red;
/* static const yo_blinker_style_t m_led_style[6] = { 1000, 500, 2000, 500, 3000, 500 }; */
static const yo_blinker_style_t m_led_style[2] = { 50, 2000 };

static void led_red_write(uint16_t value)
{
    if (value)
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    else
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

static void init(void)
{
    GPIO_InitTypeDef gpio_init_tmp;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    SysTick_Config(SystemCoreClock / 1000);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* led_red */
	gpio_init_tmp.GPIO_Pin = GPIO_Pin_5;
	gpio_init_tmp.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_tmp.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio_init_tmp);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    
    yo_blinker_init(&m_led_red, led_red_write);
}

/** \brief main function
 */
int main(void)
{
    init();
    
    yo_blinker_set_style(&m_led_red, m_led_style, 2, YO_BLINKER_FOREVER);
    yo_blinker_start(&m_led_red);
    
    while (1)
    {
        yo_blinker_poll();
    }
    
    /* return 0; */
}
