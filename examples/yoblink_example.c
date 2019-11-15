#include "rtthread.h"
#include "yoblink.h"

static rt_thread_t m_task_yoblink;
/*static */yoblink_t m_led1, m_led2;

static const uint16_t M_SYTLE1_BUF[4] = { 100, 100, 100, 100 };
static const yoblink_sty_t M_SYTLE1 = { M_SYTLE1_BUF, 4 };

static const uint16_t M_SYTLE2_BUF[4] = { 100, 100, 100, 100 };
static const yoblink_sty_t M_SYTLE2 = { M_SYTLE2_BUF, 3 };

static void task_entry(void* param_p)
{
    while (1)
    {
        yoblink_tick();
        rt_thread_mdelay(10);
    }
}

static void led1_off(void)
{
    rt_kprintf("led 1 off\n");
}

static void led1_on(void)
{
    rt_kprintf("led 1 on\n");
}

static void led2_off(void)
{
    rt_kprintf("led 2 off\n");
}

static void led2_on(void)
{
    rt_kprintf("led 2 on\n");
}

void yoblink_example(void)
{
    yoblink_init(&m_led1, led1_off, led1_on);
    yoblink_set_style(&m_led1, &M_SYTLE1, 10);
    yoblink_start(&m_led1);
    
    yoblink_init(&m_led2, led2_off, led2_on);
    yoblink_set_style(&m_led2, &M_SYTLE2, YOBLINK_CNT_FOREVER);
    yoblink_start(&m_led2);
    
    m_task_yoblink = rt_thread_create("task_yoblink", \
        task_entry, \
        RT_NULL, \
        512, \
        4, \
        5);
    
    if (m_task_yoblink)
        rt_thread_startup(m_task_yoblink);
}
MSH_CMD_EXPORT(yoblink_example, start yoblink example);
