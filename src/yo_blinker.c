/**
 * \file  yo_blinker.c
 * \brief source file for core
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

#include "yo_blinker.h"
#include "port/yo_blinker_if.h"
#include <string.h>

#define YO_BLINKER_SF_STOP    (0x00) /**< stop status flag */
#define YO_BLINKER_SF_RUNNING (0x01) /**< running status flag */

#if (YO_USING_OS)
#define yo_blinker_mutex_init(mutex_p)   yo_blinker_if_mutex_init(mutex_p)
#define yo_blinker_mutex_lock(mutex_p)   yo_blinker_if_mutex_lock(mutex_p)
#define yo_blinker_mutex_unlock(mutex_p) yo_blinker_if_mutex_unlock(mutex_p)
#else
#define yo_blinker_mutex_init(mutex_p)
#define yo_blinker_mutex_lock(mutex_p)
#define yo_blinker_mutex_unlock(mutex_p)
#endif  /* (YO_USING_OS) */

static yo_blinker_t *m_head_p = YO_NULL, m_first;

/** \brief Detach
 * \param[in] obj_p the object
 */
static void yo_blinker_detach(yo_blinker_t *obj_p)
{
    obj_p->next_p->prev_p = obj_p->prev_p;
    obj_p->prev_p->next_p = obj_p->next_p;
    obj_p->prev_p = obj_p->next_p = obj_p;
}

void yo_blinker_init(yo_blinker_t *obj_p, yo_blinker_write_ft *write_fp)
{
    yo_assert(obj_p != YO_NULL);
    
    if (m_head_p == YO_NULL)
    {
        /* init the list */
        memset(&m_first, 0, sizeof(yo_blinker_t));
        m_first.prev_p = m_first.next_p = &m_first;
        m_head_p = &m_first;
    }
    
    /* init the object */
    yo_blinker_mutex_init(&obj_p->mutex);
    memset(obj_p, 0, sizeof(yo_blinker_t));
    obj_p->prev_p = obj_p->next_p = obj_p;
    obj_p->write_fp = write_fp;
}

void yo_blinker_write(yo_blinker_t *obj_p, uint16_t value)
{
    yo_assert(obj_p != YO_NULL);
    yo_assert(obj_p->control_fp != YO_NULL);
    yo_assert((obj_p->flag & YO_BLINKER_SF_RUNNING) == 0x00);
    
    obj_p->write_fp(value);
}

void yo_blinker_set_style(yo_blinker_t *obj_p, const yo_blinker_style_t *style_p, \
    uint8_t style_num, uint8_t total_cnt)
{
    yo_assert(obj_p != NULL);
    yo_assert(sty_p != NULL);
    
    yo_blinker_mutex_lock(&obj_p->mutex);
    
    /* set style */
    obj_p->style_p = style_p;
    obj_p->style_num = style_num;
    obj_p->total_cnt = total_cnt;
    
    if (obj_p->flag & YO_BLINKER_SF_RUNNING)
    {
        /* restart */
        obj_p->current_cnt = obj_p->current_idx = 0;
        obj_p->out_tick = yo_blinker_if_get_tick() + obj_p->style_p[0];
    }
    
    yo_blinker_mutex_unlock(&obj_p->mutex);
}

void yo_blinker_start(yo_blinker_t *obj_p)
{
    yo_assert(obj_p != NULL);
    yo_assert(obj_p->sty_p != NULL);
    
    if (obj_p->flag & YO_BLINKER_SF_RUNNING)
        return;
    
    yo_blinker_mutex_lock(&obj_p->mutex);
    
    /* start */
    obj_p->current_idx = obj_p->current_cnt = 0;
    obj_p->out_tick = yo_blinker_if_get_tick() + obj_p->style_p[0];
    obj_p->flag = YO_BLINKER_SF_RUNNING;
    obj_p->write_fp(1);
    
    /* insert the object to list */
    obj_p->prev_p = m_head_p;
    obj_p->next_p = m_head_p->next_p;
    m_head_p->next_p->prev_p = obj_p;
    m_head_p->next_p = obj_p;
    
    yo_blinker_mutex_unlock(&obj_p->mutex);
}

void yo_blinker_stop(yo_blinker_t *obj_p)
{
    yo_assert(obj_p != NULL);
    
    if ((obj_p->flag & YO_BLINKER_SF_RUNNING) == 0x00)
        return;
    
    yo_blinker_mutex_lock(&obj_p->mutex);
    
    /* stop */
    obj_p->flag = YO_BLINKER_SF_STOP;
    obj_p->write_fp(0);
    
    /* detach the object from list */
    yo_blinker_detach(obj_p);
    
    yo_blinker_mutex_unlock(&obj_p->mutex);
}

void yo_blinker_poll(void)
{
    yo_blinker_t *obj_p;
    uint32_t current_tick = yo_blinker_if_get_tick();
    
    for (obj_p = m_head_p->next_p; obj_p != m_head_p; obj_p = obj_p->next_p)
    {
        if (obj_p->current_cnt < obj_p->total_cnt && \
            yo_tick_after(current_tick, obj_p->out_tick))
        {
            yo_blinker_mutex_lock(&obj_p->mutex);
__repeat:
            if (obj_p->current_idx < obj_p->style_num - 1)
            {
                if (obj_p->style_p[++obj_p->current_idx] <= 0)
                {
                    goto __repeat; /* ignored for zero */
                }
                else
                {
                    obj_p->out_tick = yo_blinker_if_get_tick() + \
                        obj_p->style_p[obj_p->current_idx];
                    obj_p->write_fp(obj_p->current_idx % 2 ? 0 : 1);
                }
            }
            else
            {
                obj_p->current_idx = 0;
                obj_p->out_tick = yo_blinker_if_get_tick() + obj_p->style_p[0];
                if (obj_p->total_cnt != YO_BLINKER_FOREVER && \
                    ++obj_p->current_cnt >= obj_p->total_cnt)
                {
                    /* select prev object at first, to prevent broken link */
                    obj_p = obj_p->prev_p;
                    yo_blinker_detach(obj_p->next_p);
                }
                else
                {
                    obj_p->write_fp(1);
                }
            }
            
            yo_blinker_mutex_unlock(&obj_p->mutex);
        }
    }
}
