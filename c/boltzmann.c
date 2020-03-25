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
            
        }
    }
    return layer;
}

struct network* create_network(struct parameters param) {
    struct network* network = malloc(sizeof (struct network));
    if (network != NULL) {
        network.num_layers = param.num_layers;
        struct layer* network.layer = create_layer(param);
    }
    return network;
}

// ========================================================
// ========================================================
// ========================================================
    struct layer* network = malloc(sizeof (struct layer) * (param.num_layers));
    if (network != NULL) {
        for (int i = 0; i < param.num_layers; i++) {
            if (i == 0) {
                network[i].num_nodes = param.N;
            }
            if (i == 1) {
                network[i].num_nodes = param.M;
            }
        }

        for (int i = 0; i < param.num_layers; i++) {
            network[i].node = malloc(sizeof (struct node) * (network[i].num_nodes));
            if (network[i].node != NULL) {
                for (int j = 0; j <= network[i].num_nodes; j++) {
                    network[i].node[j].activation = 0;
                    network[i].node[j].weight = 0;
                    network[i].node[j].bias = 0;
                    network[i].node[j].h_in = 0;
                }
            }
        }

//        for (int i = 0; i < )
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
