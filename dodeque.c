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
