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

#ifndef __YO_BLINKER_H
#define __YO_BLINKER_H

#include <stdint.h>
#include "yo_blinker_conf.h"

/* running for forever */
#define YO_BLINKER_FOREVER UINT8_MAX

typedef void yo_blinker_write_ft(uint16_t value);

typedef uint16_t yo_blinker_style_t;

typedef struct yo_blinker_s
{
    struct yo_blinker_s      *prev_p;
    struct yo_blinker_s      *next_p;
    yo_blinker_write_ft      *write_fp;   /**< write io function */
    const yo_blinker_style_t *style_p;    /**< blinker style */
    uint8_t                  style_num;   /**< number of style */
    uint32_t                 out_tick;    /**< timeout of tick */
    uint8_t                  current_idx; /**< current running index for style */
    uint8_t                  total_cnt;
    uint8_t                  current_cnt;
    uint8_t                  flag;
#if (YO_USING_OS)
    yo_mutex_t               mutex;
#endif /* (YO_USING_OS) */
} yo_blinker_t;

/** \brief Initialize
 * \param obj_p the object
 * \param write_fp the write io function
 */
void yo_blinker_init(yo_blinker_t *obj_p, yo_blinker_write_ft *write_fp);

/** \brief Write io
 * \note using at stop state only
 * \param obj_p the object
 * \param value the value
 */
void yo_blinker_write(yo_blinker_t *obj_p, uint16_t value);

/** \brief Set style
 * \param obj_p the object
 * \param style_p the style
 * \param style_num the number of style
 * \param total_cnt the total count
 *  \arg 0xFF forever
 *  \arg other count
 */
void yo_blinker_set_style(yo_blinker_t *obj_p, const yo_blinker_style_t *style_p, \
    uint8_t style_num, uint8_t total_cnt);

/** \brief Start
 * \param obj_p the object
 */
void yo_blinker_start(yo_blinker_t *obj_p);

/** \brief Stop
 * \param obj_p the object
 */
void yo_blinker_stop(yo_blinker_t *obj_p);

/** \brief This function must be called in a loop
 */
void yo_blinker_poll(void);

#endif /* __YO_BLINKER_H */
