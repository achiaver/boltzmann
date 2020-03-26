#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "boltzmann.h"
#include "stats_functions.h"

void print_parameters(struct parameters param) {
    printf("\n \
            Learning rate for weights: \t %lf \n \
            Learning rate for visible units biases: \t %lf \n \
            Learning rate for hidden units biases: \t %lf \n \
            Maximum number of epochs: \t %d \n \
            Number of layers of the network: \t %zu \n \
            Number of visible units at visible layer: \t %zu \n \
            Number of hidden units at hidden layer: \t %zu \n\n",
            param.epsilonw,
            param.epsilonvb,
            param.epsilonhb,
            param.maxepochs,
            param.num_layers,
            param.N,
            param.M);
} /* end print_parameters */

void input_parameters(struct parameters* param) {
    FILE *infile;
    infile = fopen("in_parameters.dat", "r");

    fscanf(infile, "%*s%lf%*s \
                    %*s%lf%*s \
                    %*s%lf%*s \
                    %*s%d%*s \
                    %*s%zu%*s ",
                    &param->epsilonw,
                    &param->epsilonvb,
                    &param->epsilonhb,
                    &param->maxepochs,
                    &param->num_layers);

    param->num_nodes_array = malloc(sizeof (size_t) * (param->num_layers));
    if (param->num_nodes_array != NULL) {
        for (int i = 0; i < param->num_layers; i++) {
            fscanf(infile, "%*s%zu%*s", &param->num_nodes_array[i]);
        }
    }

    fclose(infile);
} /* end input_parameters*/

//void network_status(struct layer network) {
//    for (int i = 0; i < network)
//
//    printf("\n \
//            Visible Layer: \n \
//            Number of units -> \t %zu \n \
//            Number of ");
//}
