#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "boltzmann.h"
#include "stats_functions.h"

void print_parameters(struct parameters param) {
    printf("\n \
    \t Learning rate for weights: \t %lf \n \
    \t Learning rate for visible units biases: \t %lf \n \
    \t Learning rate for hidden units biases: \t %lf \n \
    \t Maximum number of epochs: \t %d \n \
    \t Number of layers of the network: \t %zu \n",
    param.epsilonw,
    param.epsilonvb,
    param.epsilonhb,
    param.maxepochs,
    param.num_layers);

    for (int i = 0; i < param.num_layers; i++) {
        printf("\t Number of nodes at layer %d: \t %zu \n", i, param.num_nodes_array[i]);
    }

    printf("\n");
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

void print_network_status(struct network network) {
    printf("---- NETWORK STATUS ----\n");
    for (int i = 0; i < network.num_layers; i++) {
        printf("\t Layer %2d \n", i);
        for (int j = 0; j < network.layer[i].num_nodes; j++) {
            printf("\t node %2d - weights -> ", j);
            for (int k = 0; k < network.layer[i+1].num_nodes; k++) {
                printf("%f ", network.layer[i].node[j].weight[k]);
            }
        }
    }
}


//void network_status(struct layer network) {
//    for (int i = 0; i < network)
//
//    printf("\n \
//            Visible Layer: \n \
//            Number of units -> \t %zu \n \
//            Number of ");
//}
