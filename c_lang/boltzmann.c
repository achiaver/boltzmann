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


void
node_set_activation (struct node * n, size_t node, double value)
{
    n->activation[node] = value;
} /* end node_set_activation */


double
node_get_activation (struct node * n, size_t node)
{
    return n->activation[node];
} /* end node_get_activation */


void
node_set_bias (struct node * n, size_t node, double value)
{
    n->bias[node] = value;
} /* end node_set_bias */


double
node_get_bias (struct node * n, size_t node)
{
    return n->bias[node];
} /* end node_get_bias */


void
node_set_sum_info (struct node * n, size_t node, double value)
{
    n->sum_info[node] = value;
} /* end node_set_sum_info */


double
node_get_sum_info (struct node * n, size_t node)
{
    return n->sum_info[node];
} /* end node_get_sum_info */


struct node *
node_create (size_t num_nodes) 
{
    struct node * nodes = malloc( sizeof (*nodes));
    if (!nodes)
    {
        printf("node_create: nodes: malloc: %s\n", strerror(errno));
        exit(2);
    }

    nodes->num_nodes = num_nodes;
    nodes->activation = malloc( sizeof (nodes->activation) * num_nodes);
    if(!nodes->activation)
    {
        printf("node_create: nodes->activation: malloc: %s\n", strerror(errno));
        exit(2);
    }

    nodes->bias = malloc( sizeof (nodes->bias) * num_nodes);
    if(!nodes->bias)
    {
        printf("node_create: nodes->bias: malloc: %s\n", strerror(errno));
        exit(2);
    }

    nodes->sum_info = malloc( sizeof (nodes->sum_info) * num_nodes);
    if(!nodes->sum_info)
    {
        printf("node_create: nodes->info: malloc: %s\n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < num_nodes; i++)
    {
        nodes->activation[i] = 0.;
        nodes->bias[i] = 0.;
        nodes->sum_info[i] = 0.;
    }

    return nodes;
}


void
network_print (struct network * net)
{
    printf("---- NETWORK STATUS ----\n");
    for (int i = 0; i < net->num_layers; i++)
    {
        printf("Layer %2d \n", i);
        for (int j = 0; j < net->nodes_per_layer[i]; j++)
        {
            printf("\t node %2d \n", j);
            if (i == 0)
            {
                printf("\t\t- activation %2f \n", node_get_activation(net->nvisible, j));
                printf("\t\t- bias %f \n", node_get_bias(net->nvisible, j));
            } else if (i == 1)
            {
                printf("\t\t- activation %2f \n", node_get_activation(net->nhidden, j));
                printf("\t\t- bias %f \n", node_get_bias(net->nhidden, j));
            } else if (i == 2)
            {
                printf("\t\t- activation %2f \n", node_get_activation(net->nvisible, net->nodes_per_layer[0] + j));
                printf("\t\t- bias %f \n", node_get_bias(net->nvisible, net->nodes_per_layer[0] + j));
            }
        }
    }
} /* end network_print*/


struct matrix *
weight_create(size_t visible, size_t hidden)
{
    struct matrix * weight = matrix_create(visible, hidden); 
    matrix_randomize(weight);
    return weight;
} /* end of weight_create*/


struct network *
network_create (struct parameters * param)
{
    size_t visible = 0;
    size_t hidden = 0;

    struct network * net = malloc(sizeof (*net));
    if (!net) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    net->num_layers = param->num_layers;
    net->nodes_per_layer = malloc(sizeof (*net->nodes_per_layer) * net->num_layers);
    if(!net->nodes_per_layer)
    {
        printf("network_create: malloc: nodes_per_layer: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < net->num_layers; i++)
    {
        net->nodes_per_layer[i] = param->nodes_per_layer[i];
    }

    if (net->num_layers == 2)
    {
        visible = net->nodes_per_layer[0];
    } else if (net->num_layers == 3)
    {
        visible = net->nodes_per_layer[0] + net->nodes_per_layer[2];
    } else
    {
        printf("network_create: node_create: IMPOSSIBLE number of layers\n");
        exit(1);
    }
    hidden = net->nodes_per_layer[1];
    net->nvisible = node_create(visible);
    net->nhidden = node_create(hidden);
    net->weights = weight_create(visible, hidden);

//    net->layers = layer_create(param);
   
    return net;
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


////double
////node_update_activation (struct network * net, size_t node_to_update, size_t layer_current, size_t layer_other)
//void
//node_update_activation (struct network * net, size_t layer, size_t node_to_update)
//{
//    double sig = 0.;
//    double exp_argument = 0.;
//
//    if (layer == 0)
//    {
//        for (int )
//    }
//    
//    for (int i = 0; i < net->layers[layer_other].num_nodes; i++)
//    {
//        exp_argument = exp_argument + \
//                       (net->layers[layer_other].nodes[i].activation * matrix_get(net->weights, i, node_to_update));
//    }
//    exp_argument = exp_argument + net->layers[layer_current].nodes[node_to_update].bias;
//    sig = sigmoid(exp_argument, 1.0);
//
//    return sig;
//} /* end of node_update*/
//
//
//void
//node_update ( struct network * net, size_t layer, size_t dim)
//{
//    double update = 0.;
//    for (int k = 0; k < net->layers[layer].num_nodes; k++)
//    {
//        net->layers[layer];
//    }
//}


int
main(int argc, char *argv[])
{
    srand48(time(NULL));

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

//    double update = 0.;
//    for (int l = 0; l < param->dataset_rows; l++)
//    {
//        for (int k = 0; k < param->dataset_cols; k++)
//        {
//            net->layers[0].nodes[k].activation = matrix_get(dataset, l, k);
//        }
//
//        for (int k = 0; k < net->layers[1].num_nodes; k++)
//        {
//            update = node_update_activation(net, k, 1, 0);
//            printf("update - %f\n", update);
//            if (update <= 0.5)
//            {
//                net->layers[1].nodes[k].activation = 0.;
//            } else
//            {
//                net->layers[1].nodes[k].activation = 1.;
//            }
//        }
//        printf("---- Dataset pattern %d  ----\n", l);
//        network_print(net);
//        printf("\n\n");
//    }

    return 0;
}
