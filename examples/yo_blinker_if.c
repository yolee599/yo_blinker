/*
 * Copyright (c) 2019, yolee599
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-26     yolee599     first version
 */

#include "port/yo_blinker_if.h"

static uint32_t m_tick;

#if (YO_USING_OS)
void yo_blinker_if_mutex_init(yo_mutex_t *mutex_p)
{
}

void yo_blinker_if_mutex_lock(yo_mutex_t *mutex_p)
{
}

void yo_blinker_if_mutex_unlock(yo_mutex_t *mutex_p)
{
}
#endif  /* (YO_USING_OS) */

uint32_t yo_blinker_if_get_tick(void)
{
    return m_tick;
}

void SysTick_Handler(void)
{
    m_tick++;
}
