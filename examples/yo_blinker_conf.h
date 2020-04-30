/**
 * \file  yo_blinker_conf.h
 * \brief config header file for example
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
