/**
 * \file  main.c
 * \brief source file for example
 * <p>
 * Copyright (c) 2019, yolee599
 * <p>
 * <b>License:</b><br>
 * MIT
 * <p>
 * <b>Change Logs:</b><br>
 * <table border="1">
 *  <tr><th>Date<th>Author<th>Notes</tr>
 *  <tr><th>2019-11-14<th>yolee599<th>first version</tr>
 *  <tr><th>2020-04-26<th>yolee599<th>refactor, added beep example</tr>
 * </table>
 */

#include "stm32f10x.h"
#include "yo_blinker.h"

static yo_blinker_t m_beep;
static const yo_blinker_style_t m_beep_style[2] = { 50, 5000 };

static yo_blinker_t m_led_red;
static const yo_blinker_style_t m_led_style[4] = { 50, 2000, 1000, 2000 };

static void beep_write(uint16_t value)
{
    if (value)
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
    else
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

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
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /* beep */
	gpio_init_tmp.GPIO_Pin = GPIO_Pin_8;
	gpio_init_tmp.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_tmp.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &gpio_init_tmp);
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    
    /* led_red */
	gpio_init_tmp.GPIO_Pin = GPIO_Pin_5;
	gpio_init_tmp.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_tmp.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio_init_tmp);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    
    yo_blinker_init(&m_beep, beep_write);
    yo_blinker_init(&m_led_red, led_red_write);
}

/** \brief main function
 */
int main(void)
{
    init();
    
    yo_blinker_set_style(&m_beep, m_beep_style, 2, 4);
    yo_blinker_start(&m_beep);
    
    yo_blinker_set_style(&m_led_red, m_led_style, 4, YO_BLINKER_FOREVER);
    yo_blinker_start(&m_led_red);
    
    while (1)
    {
        yo_blinker_poll();
    }
    
    /* return 0; */
}
