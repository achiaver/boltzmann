#include "random_gen.h"

double
random_num (void)
{
    const gsl_rng_type * T;
    gsl_rng * r;
    gsl_rng_env_setup();

    struct timeval tv;
    gettimeofday(&tv, 0);

    unsigned long my_seed = tv.tv_sec + tv.tv_usec;

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    gsl_rng_set(r, my_seed);
    double u = gsl_rng_uniform(r);
    gsl_rng_free(r);

    return u;
}
