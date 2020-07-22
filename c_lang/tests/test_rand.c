#include <stdio.h>
#include "random_func.h"

int
main (int argc, char * argv[])
{
    random_seed(true);

    for (int i = 0; i < 5; i++)
    {
        printf("random_activation - %f\n", random_activation());
        printf("random_0to1 - %f\n", random_0to1());
        printf("random in range - %f\n", random_in_range(5, 9));
        printf("\n");
    }    
    return 0;
}
