#include <math.h>
int avg = 0;
int count = -1;
const int minth = 10;
const int maxth = 100;
const int w = 10;
const float maxp = 0.9;
 
void enque() {
  int q = queue_size();
  if (q == 0) {
    avg = (1 - w) * avg + w * q;
  } else {
    float m = function();
    avg = pow((1 - w), m) * avg;
  }
  if ((minth <= avg) && (avg <= maxth)) {
    count++;
    float pb = ( maxp * (avg - minth) ) / ( maxth - minth );
    float pa = pb / ( 1 - count * pb);
    mark(pa);
    count = 0;
  } else if (maxth <= avg) {
    mark(1);
    count = 0;
  } else {
    count = -1;
  }
}
