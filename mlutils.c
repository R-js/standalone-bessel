#include <math.h>



#include <math.h>

#define ME_RANGE_NAN 0x7FF8000000000001


static double myfmod(double x1, double x2)
{
    double q = x1 / x2;
    return x1 - floor(q) * x2;
}

double R_pow(double x, double y) /* = x ^ y */
{
    if(x == 1. || y == 0.)
	return(1.);
    if(x == 0.) {
	if(y > 0.) return(0.);
	/* y < 0 */return(INFINITY);
    }
    if (isfinite(x) && isfinite(y))
	return(pow(x,y));
    if (isnan(x) || isnan(y)) {
	return(x + y);
    }
    if(!isfinite(x)) {
	if(x > 0)		/* Inf ^ y */
	    return((y < 0.)? 0. : INFINITY);
	else {			/* (-Inf) ^ y */
	    if(isfinite(y) && y == floor(y)) /* (-Inf) ^ n */
		return((y < 0.) ? 0. : (myfmod(y,2.) ? x  : -x));
	}
    }
    if(!isfinite(y)) {
	if(x >= 0) {
	    if(y > 0)		/* y == +Inf */
		return((x >= 1)? INFINITY : 0.);
	    else		/* y == -Inf */
		return((x < 1) ? INFINITY : 0.);
	}
    }
    return(ME_RANGE_NAN);		/* all other cases: (-Inf)^{+-Inf,
				   non-int}; (neg)^{+-Inf} */
}

double R_pow_di(double x, int n)
{
    double pow = 1.0;

    if (isnan(x)) return x;
    if (n != 0) {
	if (!isfinite(x)) return R_pow(x, (double)n);
	if (n < 0) { n = -n; x = 1/x; }
	for(;;) {
	    if(n & 01) pow *= x;
	    if(n >>= 1) x *= x; else break;
	}
    }
    return pow;
}