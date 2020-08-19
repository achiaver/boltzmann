#include "random_ran3.h"

double
random_activation (int *iseed)
{
    return (int) (ran3(iseed) + 0.5);
}


double
random_0to1 (int *iseed)
{
    return ran3(iseed);
}


double
random_in_range (int *iseed, double min, double max)
{
    return ((max - min) * ran3(iseed) + min);
}


