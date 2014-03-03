#include <math.h>
#include <stdint.h>
const uint64_t    target=5 ;      /* 5   ms as per the spec */
const uint64_t  interval=100;     /* 100 ms as per the spec */
const uint16_t maxpacket=1500;    /* MTU of the link */

/* CoDel state variables */
uint64_t first_above_time;
uint64_t drop_next;
uint32_t count;
int     dropping;
uint32_t drop_count = 0;

typedef struct DelayedPacket
{
  uint64_t release_time;
  uint32_t size;
} DelayedPacket;

typedef struct {
     DelayedPacket p; 
     int ok_to_drop;
} dodeque_result; 

//uint64_t control_law (uint64_t t) { return t + interval/sqrt(count);}

dodeque_result dodeque () 
{
    uint64_t now=timestamp();
    dodeque_result r = { _pdp_deq(), 0 };
    if (r.p.size == 0 ) {
          first_above_time = 0;
    } else {
          uint64_t sojourn_time = now - r.p.release_time;
          if (sojourn_time < target || bytes() < maxpacket) {
                // went below so we'll stay below for at least interval
                first_above_time = 0;
          } else {
                if (first_above_time == 0) {
                      // just went above from below. if we stay above
                      // for at least interval we'll say it's ok to drop
                      first_above_time = now + interval;
                } else if (now >= first_above_time) {
                      r.ok_to_drop = 1;
                }
          }
    }
    return r; 
}

DelayedPacket deque()
{
      uint64_t now = timestamp();
      dodeque_result r = dodeque();
      if (r.p.size == 0 ) {
            // an empty queue takes us out of dropping state
            dropping = 0;
            return r.p;
      }
      if (dropping) {
            if (! r.ok_to_drop) {
                  // sojourn time below target - leave dropping state
                  dropping = 0;
            } else if (now >= drop_next) {
                  if (now >= drop_next && dropping) { /* Should have been while */
                        drop(r.p);
                        ++count;
                        r = dodeque();
                        if (! r.ok_to_drop)
                              // leave dropping state
                              dropping = 0;
                        else
                              // schedule the next drop.
                              drop_next = control_law(drop_next);
                        /* execute exactly once on limbo queue */
                  }
            }
      } else if (r.ok_to_drop &&
                         ((now - drop_next < interval) ||
                          (now - first_above_time >= interval))) {
                   drop(r.p);
                   r = dodeque();
                   dropping = 1;
                   // If we're in a drop cycle, the drop rate that controlled the queue
                   // on the last cycle is a good starting point to control it now.
                   if (now - drop_next < interval)
                           count = count>2? count-2 : 1;
                   else
                           count = 1;
                   drop_next = control_law(now);
             }
             return (r.p);
}
