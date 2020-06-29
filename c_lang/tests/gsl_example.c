#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


int
main (int argc, char *argv[])
{
    gsl_rng *r = gsl_rng_alloc(gsl_rng_mt19937);
    
    double x = atof(argv[1]);
    double y = gsl_sf_bessel_J0 (x);
    printf ("J0(%g) = %.18e\n", x, y);

    int i, n;
    double gauss, gamma;

    n = atoi(argv[1]);
    for (i = 0; i < n; i++)
    {
        gauss = gsl_ran_gaussian(r, 2.0);
        gamma = gsl_ran_gamma(r, 2.0, 3.0);
        printf("\t %2.4f \t %2.4f\n", gauss, gamma);
    }
  return(0);
}

