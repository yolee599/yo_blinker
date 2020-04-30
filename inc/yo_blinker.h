/**
 * \file  yo_blinker.h
 * \brief header file for core
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
 *  <tr><th>2020-04-26<th>yolee599<th>refactor</tr>
 * </table>
 */

#ifndef __YO_BLINKER_H
#define __YO_BLINKER_H

#include <stdint.h>
#include "yo_blinker_conf.h"

#define YO_BLINKER_FOREVER UINT8_MAX /**< running for forever */

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
 * \param[in] obj_p the object
 * \param[in] write_fp the write io function
 */
void yo_blinker_init(yo_blinker_t *obj_p, yo_blinker_write_ft *write_fp);

/** \brief Write io
 * \note using at stop state only
 * \param[in] obj_p the object
 * \param[in] value the value
 */
void yo_blinker_write(yo_blinker_t *obj_p, uint16_t value);

/** \brief Set style
 * \param[in] obj_p the object
 * \param[in] style_p the style
 * \param[in] style_num the number of style
 * \param[in] total_cnt the total count
 *  \arg 0xFF forever
 *  \arg other count
 */
void yo_blinker_set_style(yo_blinker_t *obj_p, const yo_blinker_style_t *style_p, \
    uint8_t style_num, uint8_t total_cnt);

/** \brief Start
 * \param[in] obj_p the object
 */
void yo_blinker_start(yo_blinker_t *obj_p);

/** \brief Stop
 * \param[in] obj_p the object
 */
void yo_blinker_stop(yo_blinker_t *obj_p);

/** \brief This function must be called in a loop
 */
void yo_blinker_poll(void);

#endif /* __YO_BLINKER_H */
