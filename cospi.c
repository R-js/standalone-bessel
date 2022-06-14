#include <math.h>

#define ME_RANGE_NAN 0x7FF8000000000001

double cospi(double x) {
    if (isnan(x))
    {
        return x;
    }

    if (!isfinite(x)){
        return ME_RANGE_NAN;
    }
      
    // reduction
    x = fmod(fabs(x), 2.); // cos() symmetric; cos(pi(x + 2k)) == cos(pi x) for all integer k
    
    // shortcuts
    if(fmod(x, 1.) == 0.5) return 0.; // sin(nPi) = 0
    if( x == 1.)	return -1.;
    if( x == 0.)	return  1.;

    // otherwise
    return cos(M_PI * x);
}
