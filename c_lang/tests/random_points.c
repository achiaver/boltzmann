#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct point {
    double x;
    double y;
};


void
seed_random ()
{
    srand(time(NULL));
}

double
random_number (int lower, int upper, int option)
{
    double random = rand() / (double)RAND_MAX;
    if (option == 0)
    {
        return random;
    } else if (option == 1)
    {
        return (upper - lower) * random + lower;
    } else
    {
        return (int)((upper - lower + 1) * random + lower);
    }
}

struct point *
point_generate (size_t npoints, int *range, int option)
{
    struct point * points = malloc( sizeof (*points) * (npoints));
    if (!points)
    {
        exit(2);
    }

    printf("(x, y)\n");
    for (int i = 0; i < npoints; i++)
    {
        printf("%f, %f\n", random_number(range[0], range[1], option), random_number(range[0], range[1], option));
    }

    return points;
}


int
main (int argc, char *argv[])
{
    int range[2] = {0, 100};
    int option = 2;
    seed_random();
    struct point * points = point_generate(10, range, option);
    free(points);
    return 0;
}
