#include <stdint.h>
#include <math.h>
const uint64_t  interval=100;     /* 100 ms as per the spec */
uint32_t count;
uint64_t control_law (uint64_t t) { return t + interval/sqrt(count);}

