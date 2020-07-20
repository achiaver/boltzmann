#include <stdio.h>
#include <time.h>
#include "ran3.h"

int main(int argc, char *argv[])
{
    const time_t timer = time(NULL);
    printf("ctime is %s\n", ctime(&timer));
    
    printf("time(NULL) - %ld\n", time(NULL));
    printf("time(NULL) to long int - %ld\n",  (-1) * (long int) &timer);
    printf("timer - %ld\n", timer);
    int seed = (-1) * (int)timer;
    printf("ran3(seed timer) - %f\n", ran3(&seed));
    seed = -42;
    printf("ran3(seed fixed) - %f\n", ran3(&seed));
    return 0;
}
