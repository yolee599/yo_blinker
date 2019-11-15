#ifndef __YOBLINK_PORT_H
#define __YOBLINK_PORT_H

#define YOBLINK_TICK_PER_SECOND 100

/* lock */
#define YOBLINK_LOCK()
#define YOBLINK_UNLOCK()

/* lock for tick */
#define YOBLINK_LOCK_TICK()
#define YOBLINK_UNLOCK_TICK()

/* assert */
#define YOBLINK_ASSERT(EX)

#endif /* __YOBLINK_PORT_H */
