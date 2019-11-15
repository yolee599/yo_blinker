/*
 * Copyright (c) 2019, yolee
 *
 * License: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     yolee        first version
 */

#include "yoblink.h"
#include "yoblink_port.h"
#include <string.h>
#include <stddef.h>

/* some status flags */
#define YOBLINK_SF_STOP    (0x00) /**< stop */
#define YOBLINK_SF_RUNNING (0x01) /**< running */

/*static */yoblink_t *m_head_p = NULL, m_first;

/** \brief Calculate the tick from millisecond.
 * \param ms millisecond for delay
 * \return result
 */
static uint16_t yoblink_tick_from_ms(uint16_t ms)
{
    return ms / (1000 / YOBLINK_TICK_PER_SECOND);
}

/** \brief Detach the object.
 * \param[in] blink_p object
 */
static void yoblink_detach(yoblink_t *blink_p)
{
    blink_p->next_p->prev_p = blink_p->prev_p;
    blink_p->prev_p->next_p = blink_p->next_p;
    blink_p->prev_p = blink_p->next_p = blink_p;
}

/** \brief Init the object.
 * \param[in] blink_p object
 * \param[in] off_fp trun off function
 * \param[in] on_fp trun on function
 */
void yoblink_init(yoblink_t *blink_p, off_ft *off_fp, on_ft *on_fp)
{
    YOBLINK_ASSERT(blink_p != NULL);
    
    if (m_head_p == NULL)
    {
        /* init the list */
        memset(&m_first, 0, sizeof(yoblink_t));
        m_first.prev_p = m_first.next_p = &m_first;
        m_head_p = &m_first;
    }
    
    /* init the object */
    memset(blink_p, 0, sizeof(yoblink_t));
    blink_p->prev_p = blink_p->next_p = blink_p;
    blink_p->off_fp = off_fp;
    blink_p->on_fp = on_fp;
}

/** \brief Trun off.
 * \note using at stop state only
 * \param[in] blink_p object
 */
void yoblink_off(yoblink_t *blink_p)
{
    YOBLINK_ASSERT(blink_p != NULL);
    YOBLINK_ASSERT(blink_p->off_fp != NULL);
    
    blink_p->off_fp();
}

/** \brief Trun on.
 * \note using at stop state only
 * \param[in] blink_p object
 */
void yoblink_on(yoblink_t *blink_p)
{
    YOBLINK_ASSERT(blink_p != NULL);
    YOBLINK_ASSERT(blink_p->on_fp != NULL);
    
    blink_p->on_fp();
}

/** \brief Start the object.
 * \param[in] blink_p object
 */
void yoblink_start(yoblink_t *blink_p)
{
    YOBLINK_ASSERT(blink_p != NULL);
    YOBLINK_ASSERT(blink_p->sty_p != NULL);
    
    if (blink_p->flag & YOBLINK_SF_RUNNING)
        return;
    
    YOBLINK_LOCK();
    
    /* start */
    blink_p->curr_idx = blink_p->curr_cnt = 0;
    blink_p->remain_tick = yoblink_tick_from_ms(blink_p->sty_p->buf_p[0]);
    blink_p->flag = YOBLINK_SF_RUNNING;
    blink_p->on_fp();
    
    /* insert the object to list */
    blink_p->prev_p = m_head_p;
    blink_p->next_p = m_head_p->next_p;
    m_head_p->next_p->prev_p = blink_p;
    m_head_p->next_p = blink_p;
    
    YOBLINK_UNLOCK();
}

/** \brief Stop the object.
 * \param[in] blink_p object
 */
void yoblink_stop(yoblink_t *blink_p)
{
    YOBLINK_ASSERT(blink_p != NULL);
    
    if ((blink_p->flag & YOBLINK_SF_RUNNING) == 0x00)
        return;
    
    YOBLINK_LOCK();
    
    /* stop */
    blink_p->flag = YOBLINK_SF_STOP;
    blink_p->off_fp();
    
    /* detach the object from list */
    yoblink_detach(blink_p);
    
    YOBLINK_UNLOCK();
}

/** \brief Set style of yoblink
 * \param[in] blink_p object
 * \param[in] sty_p style
 * \param[in] total_cnt total count
 *  \arg 0xFF forever
 *  \arg other count
 */
void yoblink_set_style(yoblink_t *blink_p, const yoblink_sty_t *sty_p, uint8_t total_cnt)
{
    YOBLINK_ASSERT(blink_p != NULL);
    YOBLINK_ASSERT(sty_p != NULL);
    
    YOBLINK_LOCK();
    
    /* set style */
    blink_p->sty_p = sty_p;
    blink_p->total_cnt = total_cnt;
    
    if (blink_p->flag & YOBLINK_SF_RUNNING)
    {
        /* restart */
        blink_p->curr_cnt = blink_p->curr_idx = 0;
        blink_p->remain_tick = yoblink_tick_from_ms(blink_p->sty_p->buf_p[0]);
    }
    
    YOBLINK_UNLOCK();
}

/** \brief Tick
 */
void yoblink_tick(void)
{
    yoblink_t *blink_p;
    
    YOBLINK_LOCK_TICK();
    
    for (blink_p = m_head_p->next_p; blink_p != m_head_p; blink_p = blink_p->next_p)
    {
        if (blink_p->curr_cnt < blink_p->total_cnt)
        {
            if (blink_p->remain_tick-- <= 0)
            {
__repeat:
                if (blink_p->curr_idx < blink_p->sty_p->num - 1)
                {
                    blink_p->remain_tick = yoblink_tick_from_ms(blink_p->sty_p->buf_p[++blink_p->curr_idx]);
                    if (blink_p->remain_tick <= 0)
                    {
                        goto __repeat; /* ignored for zero */
                    }
                    else
                    {
                        if (blink_p->curr_idx % 2)
                            blink_p->off_fp();
                        else
                            blink_p->on_fp();
                    }
                }
                else
                {
                    blink_p->curr_idx = 0;
                    blink_p->remain_tick = yoblink_tick_from_ms(blink_p->sty_p->buf_p[0]);
                    if (blink_p->total_cnt != YOBLINK_CNT_FOREVER)
                        blink_p->curr_cnt++;
                    else
                        blink_p->on_fp();
                }
            }
        }
        else
        {
            blink_p = blink_p->prev_p;
            yoblink_detach(blink_p->next_p);
        }
    }
    
    YOBLINK_UNLOCK_TICK();
}
