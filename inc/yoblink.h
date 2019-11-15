/*
 * Copyright (c) 2019, yolee
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     yolee        first version
 */

#ifndef __YOBLINK_H
#define __YOBLINK_H

#include <stdint.h>

#define YOBLINK_CNT_FOREVER UINT8_MAX /**< running for forever */

typedef void off_ft(void);
typedef void on_ft(void);

typedef struct
{
    const uint16_t *buf_p;
    uint8_t        num;
} yoblink_sty_t;

struct yoblink_s
{
    off_ft              *off_fp;     /**< trun off function */
    on_ft               *on_fp;      /**< trun on function */
    const yoblink_sty_t *sty_p;      /**< style */
    uint8_t             curr_idx;    /**< current running index of style */
    uint8_t             total_cnt;   /**< total count */
    uint8_t             curr_cnt;    /**< current count */
    uint16_t            remain_tick; /**< remaining tick */
    uint8_t             flag;        /**< flag */
    struct yoblink_s    *prev_p;     /**< prev object */
    struct yoblink_s    *next_p;     /**< next object */
};

typedef struct yoblink_s yoblink_t;

void yoblink_init(yoblink_t *blink_p, off_ft *off_fp, on_ft *on_fp);
void yoblink_off(yoblink_t *blink_p);
void yoblink_on(yoblink_t *blink_p);
void yoblink_start(yoblink_t *blink_p);
void yoblink_stop(yoblink_t *blink_p);
void yoblink_set_style(yoblink_t *blink_p, const yoblink_sty_t *sty_p, uint8_t total_cnt);
void yoblink_tick(void);

#endif /* __YOBLINK_H */
