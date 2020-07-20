#include <stdio.h>
#include <time.h>
#include "ran3.h"

int main(int argc, char *argv[])
{
    const time_t timer = time(NULL);
    printf("ctime is %s\n", ctime(&timer));
    printf("%ld\n", time(NULL));
    printf("%ld\n",  (-1) * (long int) &timer);
    printf("%ld\n", timer);
    int seed = (-1) * (int)timer;
    printf("%f\n", ran3(&seed));
    seed = -42;
    printf("%f\n", ran3(&seed));
    return 0;
}
