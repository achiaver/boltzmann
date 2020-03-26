#include <stdio.h>
#include <string.h>
#include "boltzmann.h"
#include "stats_functions.h"


//struct layer* create_layer(int kind) {
//    struct layer* layer = malloc(sizeof (struct layer));
//    if (layer != NULL) {
//        layer->num_nodes = 0;
////        layer->kind = kind;
//        layer->node = NULL;
//    }
//    return layer;
//}

// PRECISO ACABAR DE ARRUMAR AS FUNÇÕES PARA CRIAR CADA CAMADA E CADA NODE.

struct layer* create_layer(struct parameters param) {
    struct layer* layer = malloc(sizeof (struct layer) * (param.num_layers));
    if (layer != NULL) {
        for (int i = 0; i < param.num_layers; i++) {
            layer[i].num_nodes = param.num_nodes_array[i];
            layer[i].node = malloc(sizeof (struct layer) * (layer[i].num_nodes));
            if (layer[i].node != NULL) {
                for (int j = 0; j < layer[i].num_nodes; j++) {
                    layer[i].node[j].activation = 0;
                    layer[i].node[j].bias = 0;
                    layer[i].node[j].h_in = 0;
                    if (&layer[i+1] != NULL) {
                        layer[i].node[j].weight = malloc(sizeof (double) * (layer[i+1].num_nodes));
                        for (int k = 0; k < layer[i+1].num_nodes; k++) {
                            layer[i].node[j].weight[k] = 0.0;
                        }
                    }
                }
            }
        }
    }
    return layer;
}

struct network* create_network(struct parameters param) {
    struct network* network = malloc(sizeof (struct network));
    if (network != NULL) {
        network->num_layers = param.num_layers;
        network->layer = create_layer(param);
    }
    return network;
}



int main(int argc, char *argv[])
{
    /* Boltzmann Machine parameters */
//    struct parameters param = {
//        .N = 0,
//        .M = 0,
//        .epsilonw = 0.0,
//        .epsilonvb = 0.0,
//        .epsilonhb = 0.0,
//        .maxepochs = 0
//    };

    struct parameters param;
    input_parameters(&param);
    print_parameters(param);

    struct network* network = create_network(param);
    print_network_status(network);



    /* Sample of initial values for training */
    double sample_input[][3] = {
        { 1.f, 1.f, 0.f },
        { 1.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 1.f },
    };

//    size_t data_count = sizeof sample_input / sizeof *sample_input;
//    double *tmp_input;
//    tmp_input = &sample_input;
//
//
//    for(size_t j=0; j < data_count; j++) {
//        for (size_t i=0; i< param.N; i++) {
//            printf("%1.0f ", *tmp_input++);
//        }
//        printf("\n");
//    }
//
    return 0;
}
