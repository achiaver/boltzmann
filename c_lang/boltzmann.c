#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "parameters.h"
#include "boltzmann.h"
#include "matrix.h"
#include <time.h>
#include <math.h>


void
node_set_activation (struct node * n, int node, double value)
{
    n[node].activation = value;
} /* end node_set_activation */


void
node_set_bias (struct node * n, int node, double value)
{
    n[node].bias = value;
} /* end node_set_bias */


void
node_set_z_in (struct node * n, int node, double value)
{
    n[node].z_in = value;
} /* end node_set_z_in */


void
node_randomize_bias (struct node * n)
{
    n->bias = drand48();
} /* end of node_randomize_bias */


double
node_get_activation (struct node * n, int node)
{
    return n[node].activation;
} /* end node_get_activation */


double
node_get_bias (struct node * n, int node)
{
    return n[node].bias;
} /* end node_get_bias */


double
node_get_z_in (struct node * n, int node)
{
    return n[node].z_in;
} /* end node_get_z_in */


void
node_print(struct node * n, int node, int option)
{
    if (option == 0) // print all node status
    {
        printf("\t- activation \t %f \n", node_get_activation(n, node));
        printf("\t- bias \t %f \n", node_get_bias(n, node));
        printf("\t- z_in \t %f \n", node_get_z_in(n, node));
        printf("\n");
    } else if (option == 1) // print only node activation
    {
        printf("\t- activation \t %f \n", node_get_activation(n, node));
        printf("\n");
    } else
    {
        printf("Not a node_print option\n");
        exit(1);
    }
} /* end node_print */


void
node_copy (struct node * n1, struct node * n2)
{
    *n2 = *n1;
} /* end of node_copy */


void
node_create (struct node * n, int node)
{
    node_set_activation(n, node, 0.);
    node_set_bias(n, node, 0.);
    node_set_z_in(n, node, 0.);
} /* end of node_create */


struct layer *
layer_create (size_t num_nodes)
{
    struct layer * l = malloc(sizeof (*l));
    if (!l) {
        printf("layer_create: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }

    l->num_nodes = num_nodes;
    l->nodes = malloc(sizeof(l->nodes) * l->num_nodes);
    for (int i = 0; i < l->num_nodes; i++)
    {
        node_create(l->nodes, i);
    }

    return l;
}


void
layer_print (struct layer * l, int option)
{
    for (int i = 0; i < l->num_nodes; i++)
    {
        printf("Node %d\n", i);
        node_print(l->nodes, i, option);
    }
}


void
layer_delete (struct layer * l)
{
    free(l->nodes);
}

void
network_print (struct network * net, int option)
{
    printf("---- NETWORK STATUS ----\n");
    printf("NUMBER OF LAYERS ---> %zu\n", net->num_layers);
    for (int i = 0; i < net->num_layers; i++)
    {
        printf("Layer %2d \n", i);
        if (i == 0)
        {
            layer_print(&net->visible, option);
        } else
        {
            layer_print(&net->hidden, option);
        }
    }

    printf("\n---- WEIGHTS VALUES ----\n");
    matrix_print(net->weights);
    printf("\n");
} /* end network_print*/


void
network_delete (struct network * net)
{
    layer_delete(&net->visible);
    layer_delete(&net->hidden);
    free(net);
}

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
    struct network * net = malloc(sizeof (*net));
    if (!net) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }

    net->num_layers = param->num_layers;
    printf("NUMBER OF LAYERS (1) ---> %zu \n", net->num_layers);
    net->visible.num_nodes = param->nodes_per_layer[0];
    net->visible.nodes = malloc(sizeof (net->visible.nodes) * net->visible.num_nodes);
    for (int i = 0; i < net->visible.num_nodes; i++)
    {
        node_create(net->visible.nodes, i);
    }
    printf("NUMBER OF LAYERS (2) ---> %zu \n", net->num_layers);

    net->hidden.num_nodes = param->nodes_per_layer[1];
    net->hidden.nodes = malloc(sizeof (net->hidden.nodes) * net->hidden.num_nodes);
    for (int i = 0; i < net->hidden.num_nodes; i++)
    {
        node_create(net->hidden.nodes, i);
    }
    printf("NUMBER OF LAYERS (3) ---> %zu \n", net->num_layers);

    net->num_layers = param->num_layers;
    net->weights = weight_create(net->visible.num_nodes, net->hidden.num_nodes);
    printf("NUMBER OF LAYERS (4) ---> %zu \n", net->num_layers);

    return net;
} /* end of network_create */


struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end of dataset_allocate*/


double
sigmoid (double expoent, double temp)
{
    return (1 / (1 + exp(-expoent/temp)));
} /* end of sigmoid*/


//double
//state_energy (struct matrix * weights, struct node * visible, struct node * hidden)
//{
//    double energy = 0.;
//    for (int i = 0; i < visible.num_nodes; i++)
//    {
//        for (int j = 0; j < hidden.num_nodes; j++)
//        {
//            energy -= node_get_activation(visible, i) * \
//                      matrix_get(weights, i, j) * \
//                      node_get_activation(hidden, j);
//        }
//        energy -= node_get_bias(visible, i);
//    }
//    energy /= 2;
//    return energy;
//}
//
//
//double
//network_energy (struct network * net)
//{
//    return state_energy (net->weights, net->visible, net->hidden);
//}



int
main(int argc, char *argv[])
{
    srand48(time(NULL));

    char * parameters_file = "in_parameters.dat";
    char * dataset_file = "dataset/three_node_test.csv";
    struct parameters * param = parameters_input(parameters_file, dataset_file);
    parameters_print(param);

    struct matrix * dataset = dataset_allocate(param->dataset_file, param->dataset_rows, param->dataset_cols);
    printf("\n\nInput dataset display\n\n");
    matrix_print(dataset);

    struct network * net = network_create(param);
    printf("%zu\n", net->num_layers);
    printf("\n\n");
    network_print(net, 0);

    struct layer * visible = layer_create(param->nodes_per_layer[0]);
//    struct layer * visible_aux = layer_create(param->nodes_per_layer[0]);
//    struct layer * hidden = layer_create(param->nodes_per_layer[1]);
    for (int i = 0; i < visible->num_nodes; i++)
    {
        node_set_activation(visible->nodes, i, matrix_get(dataset, 0, i));
    }
    layer_print(visible, 1);
    printf("\n");

//    double energy = state_energy(net->weights, visible, hidden); // holds the initial energy of the state of the system
//    double energy_compare = 0.; // after any update, energy_compare will have the immediate energy of the state
//    // energy and energy_compare will be compared, if there is a decrease in energy, update is kept,
//    // otherwise the previous value of the unit remains.
//    double sum = 0.;
//    double expo = 0.;
//
//    printf("\n---- UPDATE HIDDEN UNITS ----\n");
//    for (size_t i = 0; i < hidden->num_nodes; i++)
//    {
//        for (size_t j = 0; j < visible->num_nodes; j++)
//        {
////            printf("%zu \t %zu\n", i, j);
////            printf("%f\n", matrix_get(net->weights, j, i));
//            sum += node_get_activation(visible, j) * matrix_get(net->weights, j, i);
//        }
//        sum += node_get_bias(visible, i);
//        expo = sigmoid(sum, 1);
//        if (expo < 0.5)
//        {
//            hidden->activation[i] = 0.;
//        } else
//        {
//            hidden->activation[i] = 1.;
//        }
//    energy_compare = state_energy(net->weights, visible, hidden);
//    printf("state energy 1 -> %f \t state energy 2 -> %f \n", energy, energy_compare);
//    }
//    node_print(hidden, 1);
//
//    printf("\n---- UPDATE VISIBLE UNITS ----\n");
//    for (size_t i = 0; i < visible_aux->num_nodes; i++)
//    {
//        sum = 0.;
//        expo = 0.;
//        for (size_t j = 0; j < hidden->num_nodes; j++)
//        {
////            printf("%zu \t %zu\n", i, j);
////            printf("%f\n", matrix_get(net->weights, i, j));
//            sum += node_get_activation(hidden, j) * matrix_get(net->weights, i, j);
//        }
//        sum += node_get_bias(net->visible, i);
//        expo = sigmoid(sum, 1);
//        if (expo < 0.5)
//        {
//            visible_aux->activation[i] = 0.;
//        } else
//        {
//            visible_aux->activation[i] = 1.;
//        }
//    energy_compare = state_energy(net->weights, visible, hidden);
//    printf("state energy 1 -> %f \t state energy 2 -> %f \n", energy, energy_compare);
//    }
//    node_print(visible_aux, 1);

    layer_delete(visible);
    network_delete(net);
    return 0;
}
