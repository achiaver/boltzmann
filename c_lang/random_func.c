#include "random_func.h"

void
random_seed (bool nondeterministic_seed)
{
    int address_1 = 42;
    int address_2 = 420;
    if (nondeterministic_seed)
    {
        pcg32_srandom(time(NULL) ^ (intptr_t)&address_1, (intptr_t)&address_2);
    } else
    {
        pcg32_srandom(42u, 54u);
    }
}

double
random_activation (void)
{
    return (double)pcg32_boundedrand(2);
}

double
random_0to1 (void)
{
    return ldexp(pcg32_random(), -32);
}

double
random_in_range (double min, double max)
{
    return 0.;
}

