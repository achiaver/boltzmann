#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void
init_seed (int option, int init)
{
    if (option == 0)
    {
        srand(time(NULL));
    } else
    {
        srand(init);
    }
}

int
rand_int (void)
{
    return rand();
}

int 
rand_int_max (int max)
{
    return (int)((double)rand() / ((double)RAND_MAX + 1) * max);
}

int
rand_int_in_range (int min, int max)
{
    return (int)(min + (double)rand() / (double)(RAND_MAX / (max - min + 1) + 1));
}

int
main (int argc, char * argv[])
{
//    init_seed(0, 42); // if first parameter = 0, second parameter has no importance
    srand(time(NULL));

    int min = 4;
    int max = 16;

    printf("rand_int - %d\n", rand_int);
    printf("rand_int_max - %d\n", rand_int_max(2));
    printf("rand_int_max %d - %d\n", max, rand_int_max(max));
    printf("rand_int_in_range [%d, %d] - %d\n", min, max, rand_int_in_range(min, max));

    return 0;
}
