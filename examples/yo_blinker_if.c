/**
 * \file  yo_blinker_if.c
 * \brief source file for example interface
 * <p>
 * Copyright (c) 2019, yolee599
 * <p>
 * <b>License:</b><br>
 * MIT
 * <p>
 * <b>Change Logs:</b><br>
 * <table border="1">
 *  <tr><th>Date<th>Author<th>Notes</tr>
 *  <tr><th>2020-04-26<th>yolee599<th>first version</tr>
 * </table>
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
