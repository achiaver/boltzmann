#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "boltzmann.h"
#include "stats_functions.h"
#include "matrix.h"
#include "ran3.h"
#include <time.h> 
#include <math.h>

struct node *
node_create (size_t num_nodes) 
{
    struct node * nodes = malloc( sizeof (*nodes) * (num_nodes));
    if (!nodes)
    {
        printf("node_create: malloc: node: %s\n", strerror(errno));
        exit(2);
    }
    
    for (int i = 0; i < num_nodes; i++)
    {
        nodes[i].activation = 0.;
        nodes[i].bias = 0.;
        nodes[i].sum_info = 0.;
    }

    return nodes;
}


struct layer *
//layer_create (struct parameters * param)
layer_create (struct parameters * param)
{
    struct layer * layer_bm = malloc( sizeof (*layer_bm) * (param->num_layers));
    if (!layer_bm)
    {
        printf("layer_create: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < param->num_layers; i++)
    {
        layer_bm[i].num_nodes = param->num_nodes_array[i];
        layer_bm[i].nodes = node_create(layer_bm[i].num_nodes);
       // layer_bm[i].nodes = malloc( sizeof (struct node) * (layer_bm[i].num_nodes));
//        if (!layer_bm[i].nodes)
//        {
//            printf("layer_create: malloc: node: %s\n", strerror(errno));
//            exit(2);
//        }
//
//        for (int j = 0; j < layer_bm[i].num_nodes; j++)
//        {
//            layer_bm[i].nodes[j].activation = 0;
//            layer_bm[i].nodes[j].bias = 0;
//            layer_bm[i].nodes[j].sum_info = 0;
//
//            if (i == 0)
//            {
//                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param->num_nodes_array[i+1]));
//                if (!layer_bm[i].node[j].weight)
//                {
//                    printf("layer_create: malloc: node %d: weight: %s \n", j,  strerror(errno));
//                    exit(2);
//                }
//
//                for (int k = 0; k < param->num_nodes_array[i+1]; k++)
//                {
//                    layer_bm[i].node[j].weight[k] = drand48();
//                }
//            } else
//            {
//                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param->num_nodes_array[i-1]));
//                if (!layer_bm[i].node[j].weight)
//                {
//                    printf("layer_create: malloc: node %d: weight: %s\n", j, strerror(errno));
//                    exit(2);
//                }
//
//                for (int k = 0; k < param->num_nodes_array[i-1]; k++)
//                {
//                    layer_bm[i].node[j].weight[k] = layer_bm[i-1].node[k].weight[j];
//                }
//            }
//        }
    }
    return layer_bm;
} /* end of layer_create */


struct matrix *
weight_create(struct parameters * param)
{
    struct matrix * weight = matrix_create(param->num_nodes_array[0], param->num_nodes_array[1]); 
    matrix_randomize(weight);
    return weight;
} /* end of weight_create*/

struct network *
network_create (struct parameters * param)
{
    struct network * network_bm = malloc(sizeof (*network_bm));
    if (!network_bm) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    network_bm->num_layers = param->num_layers;
    network_bm->layers = layer_create(param);
    network_bm->weights = weight_create(param);
    return network_bm;
} /* end of network_create */


struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end )f dataset_allocate*/


double
sigmoid (double expoent, double temp) 
{
    return (1 / (1 + exp(-expoent/temp)));
} /* end of sigmoid*/


double
node_activation_update (struct network * net, size_t node_to_update, size_t layer_current, size_t layer_other)
{
    double sig = 0.;
    double exp_argument = 0.;

    for (int i = 0; i < net->layers[layer_other].num_nodes; i++)
    {
        exp_argument = exp_argument + \
                       (net->layers[layer_other].nodes[i].activation * matrix_get(net->weights, i, node_to_update));
    }
    exp_argument = exp_argument + net->layers[layer_current].nodes[node_to_update].bias;
    sig = sigmoid(-exp_argument, 1.0);

    return sig;
} /* end of node_update*/


int
main(int argc, char *argv[])
{
    
//    srand48(time(NULL));

    char * parameters_file = "in_parameters.dat";
    char * dataset_file = "dataset/three_node_test.csv";
    struct parameters * param = parameters_input(parameters_file, dataset_file);
    parameters_print(param);

    struct network * net = network_create(param);
    network_print(net);
    matrix_print(net->weights);

    struct matrix* dataset = dataset_allocate(param->dataset_file, param->dataset_rows, param->dataset_cols);
    printf("\n\nInput dataset display\n\n");
    matrix_print(dataset);

    double update = 0.;
    for (int l = 0; l < param->dataset_rows; l++)
    {
        for (int k = 0; k < param->dataset_cols; k++)
        {
            net->layers[0].nodes[k].activation = matrix_get(dataset, l, k);
        }

        for (int k = 0; k < net->layers[1].num_nodes; k++)
        {
            update = node_activation_update(net, k, 1, 0);
            printf("update - %f\n", update);
            if (update < 0.5)
            {
                net->layers[1].nodes[k].activation = 0.;
            } else
            {
                net->layers[1].nodes[k].activation = 1.;
            }
        }
        printf("---- Dataset pattern %d  ----\n", l);
        network_print(net);
        printf("\n\n");
    }

    return 0;
}
