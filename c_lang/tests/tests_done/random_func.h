#ifndef __RANDOM_FUNC_H__
#define __RANDOM_FUNC_H__

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include "pcg_basic.h"

void    random_seed         (bool   nondeterministic_seed);
int     random_bounded      (int);
double  random_activation   (void);
double  random_0to1         (void);
double  random_in_range     (double min, 
                             double max);


#endif /* __RANDOM_FUNC_H__*/


