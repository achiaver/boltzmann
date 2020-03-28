#include <stdio.h>
#include <string.h>
#include <errno.h>
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

struct layer * create_layer(struct parameters parameters_bm) {
    struct layer * layer_bm = malloc(sizeof (*layer_bm) * (parameters_bm.num_layers));
    if (!layer_bm) 
    {
        perror("create_layer: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < parameters_bm.num_layers; i++) 
    {
        printf("i - %d \n", i);
        layer_bm[i].num_nodes = parameters_bm.num_nodes_array[i];
        layer_bm[i].node = malloc(sizeof (struct node) * (layer_bm[i].num_nodes));
        if (!layer_bm[i].node) 
        {
            perror("create_layer: malloc: node: %s\n", strerror(errno));
            exit(2);
        }

        for (int j = 0; j < layer_bm[i].num_nodes; j++) 
        {
            printf("i - %d \t j - %d \n", i, j);
            layer_bm[i].node[j].activation = 0;
            layer_bm[i].node[j].bias = 0;
            layer_bm[i].node[j].h_in = 0;
//            if (&layer[i+1] != NULL) {AQUI PRECISO PENSAR EM COMO CONSIDERAR MAIS DE DUAS CAMADAS
//                                      ACHO QUE A OPCAO MAIS FACIL VAI SER USANDO DOIS VETORES DE PESOS
//                                      UM PARA OS PESOS DA ENTRADA W^(I) E UM PARA PESOS DA SAIDA W^(O)}
//          NAO ESTA ENTRANDO NESSE IF...???
//            i = 0;
            if (i == 0) 
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double) * (layer_bm[i+1].num_nodes));
                if (!layer_bm[i].node[j].weight)
                {
                    perror("create_layer: malloc: node %d: weight: %s \n", j,  strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < layer_bm[i+1].num_nodes; k++) 
                {
                    printf("i - %d \t j - %d \t k - %d \n", i, j, k);
                    layer_bm[i].node[j].weight[k] = 2.2; //(double) (10 * (j+1)) + (k+1);
                    printf("%f \n", layer_bm[i].node[j].weight[k]);
                }
            } else 
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double) * (layer_bm[i-1].num_nodes));
                if (!layer_bm[i].node[j].weight)
                {
                    perror("create_layer: malloc: node %d: weight: %s\n", j, strerror(errno));
                    exit(2);
                }

                for (int k = ; k < layer_bm[i-1].num_nodes; k++) 
                {
                    printf("i - %d \t j - %d \t k - %d \n", i, j, k);
                    layer_bm[i].node[j].weight[k] = 42; //layer[i-1].node[k].weight[j];
                    printf("%f \n", layer_bm[i].node[j].weight[k]);
                }
            }
        }
    }
    printf("\n");
    return layer_bm;
}

struct network * create_network(struct parameters parameters_bm) {
    struct network * network_bm = malloc(sizeof (*network_bm));
    if (!network_bm) {
        perror("create_network: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    network_bm->num_layers = parameters_bm.num_layers;
    network_bm->layer = create_layer(parameters_bm);
    return network_bm;
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

    struct parameters parameters_bm;
    input_parameters(&parameters_bm);
    print_parameters(parameters_bm);

    struct network * network_bm = create_network(parameters_bm);
    print_network_status(*network_bm);



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
