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
    double random = (double) rand() / ((double)RAND_MAX + 1);
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
    if (argc <= 2)
    {
        fprintf(stderr, "usage: %s <random option {0, 1, 2} (int)> <number of points (int)>\n", argv[0]);
        exit(2);
    }

    int range[2] = {0, 100};
    int option = atoi(argv[1]);
    int npoints = atoi(argv[2]);
    seed_random();
    struct point * points = point_generate(npoints, range, option);
    free(points);
    return 0;
}
