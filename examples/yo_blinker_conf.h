/*
 * Copyright (c) 2019, yolee599
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-26     yolee599     first version
 */

#ifndef __YO_BLINKER_CONF_H
#define __YO_BLINKER_CONF_H

#include <stdint.h>

#define YO_NULL         0

#define YO_USING_OS     0

#define yo_assert(expr)

/* returns true if the tick a is after time b. */
#define yo_tick_after(a,b) ((int32_t)(b) - (int32_t)(a) < 0)

/* using only rtos */
/* typedef uint16_t yo_mutex_t; */

#endif /* __YO_BLINKER_CONF_H */
