#ifndef __RANDOM_RAN3_H__
#define __RANDOM_RAN3_H__

#include "ran3.h"

double random_activation (int *iseed);
double random_0to1       (int *iseed);
double random_in_range   (int *iseed, 
                          double min,
                          double max);


#endif /* __RANDOM_RAN3_H__ */
