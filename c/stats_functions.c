#include <stdio.h>
#include <stddef.h>
#include "boltzmann.h"
#include "stats_functions.h"

void print_parameters(struct parameters param) {
    printf("\n \
            Number of visible units at visible layer: \t\t\t\t\t %zu \n \
            Number of hidden units at hidden layer: \t\t\t\t\t %zu \n \
            Learning rate for weights: \t\t\t\t\t %lf \n \
            Learning rate for visible units biases: \t\t\t\t\t %lf \n \
            Learning rate for hidden units biases: \t\t\t\t\t %lf \n \
            Maximum number of epochs: \t\t\t\t\t %d \n\n",
            param.N,
            param.M,
            param.epsilonw,
            param.epsilonvb,
            param.epsilonhb,
            param.maxepochs);
} /* end print_parameters */

void input_parameters(struct parameters* param) {
    FILE *infile;
    infile = fopen("in_parameters.dat", "r");

    fscanf(infile, "%*s%lf%*s \
                    %*s%lf%*s \
                    %*s%lf%*s \
                    %*s%d%*s \
                    %*s%zu%*s \
                    %*s%zu%*s \
                    %*s%zu%*s",
                    &param->epsilonw,
                    &param->epsilonvb,
                    &param->epsilonhb,
                    &param->maxepochs,
                    &param->num_layers,
                    &param->N,
                    &param->M);

    fclose(infile);
} /* end input_parameters*/
