/*
 * Copyright (c) 2019, yolee599
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-26     yolee599     first version
 */

#ifndef __YO_BLINKER_IF_H
#define __YO_BLINKER_IF_H

#include <stdint.h>
#include "yo_blinker_conf.h"

#if (YO_USING_OS)
/** \brief Initialize mutex
 * \param mutex_p the mutex
 */
void yo_blinker_if_mutex_init(yo_mutex_t *mutex_p);

/** \brief Locked mutex
 * \param mutex_p the mutex
 */
void yo_blinker_if_mutex_lock(yo_mutex_t *mutex_p);

/** \brief Unlocked mutex
 * \param mutex_p the mutex
 */
void yo_blinker_if_mutex_unlock(yo_mutex_t *mutex_p);
#endif  /* (YO_USING_OS) */

/** \brief Get system tick
 * \return the tick
 */
uint32_t yo_blinker_if_get_tick(void);

#endif /* __YO_BLINKER_IF_H */
