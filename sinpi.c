
#include <math.h>

#define ME_RANGE_NAN 0x7FF8000000000001

double sinpi(double x) {
    if (isnan(x)) return x;

    if(!isfinite (x)) return ME_RANGE_NAN;

    x = fmod(x, 2.); // sin(pi(x + 2k)) == sin(pi x)  for all integer k
    // map (-2,2) --> (-1,1] :
    if(x <= -1) x += 2.; else if (x > 1.) x -= 2.;
    if(x == 0. || x == 1.) return 0.;
    if(x ==  0.5)	return  1.;
    if(x == -0.5)	return -1.;
    // otherwise
    return sin(M_PI * x);
}