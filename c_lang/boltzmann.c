#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "boltzmann.h"
#include "parameters.h"
#include "matrix.h"
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


void
node_randomize_bias (struct node * n)
{
    for (size_t i = 0; i < n->num_nodes; i++)
    {
        n->bias[i] = drand48();
    }
} /* end of node_randomize_bias */


double
node_get_bias (struct node * n, size_t node)
{
    return n->bias[node];
} /* end node_get_bias */


void
node_print(struct node * n, int option)
{
    if (option == 0) // print only units activation
    {
        for (size_t node = 0; node < n->num_nodes; node++)
        {
            printf("- activation \t %f \n", node_get_activation(n, node));
            printf("- bias \t %f \n", node_get_bias(n, node));
        }
        printf("\n");
    } else if (option == 1)
    {
        for (size_t node = 0; node < n->num_nodes; node++)
        {
            printf("%f \t", node_get_activation(n, node));
        }
        printf("\n");
    } else 
    {
        printf("Not a node_print option\n");
    }
} /* end node_print */


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


void
node_copy (struct node * n1, struct node * n2)
{
    *n2 = *n1;
} /* end of node_copy */


struct node *
node_create (size_t num_nodes) 
{
    struct node * n = malloc( sizeof (*n));
    if (!n)
    {
        printf("node_create: nodes: malloc: %s\n", strerror(errno));
        exit(2);
    }

    n->num_nodes = num_nodes;
    n->activation = malloc( sizeof (n->activation) * num_nodes);
    if(!n->activation)
    {
        printf("node_create: nodes->activation: malloc: %s\n", strerror(errno));
        exit(2);
    }

    n->bias = malloc( sizeof (n->bias) * num_nodes);
    if(!n->bias)
    {
        printf("node_create: nodes->bias: malloc: %s\n", strerror(errno));
        exit(2);
    }

    n->sum_info = malloc( sizeof (n->sum_info) * num_nodes);
    if(!n->sum_info)
    {
        printf("node_create: nodes->info: malloc: %s\n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < num_nodes; i++)
    {
        n->activation[i] = 0.;
//        n->bias[i] = 0.;
        node_randomize_bias(n);
        n->sum_info[i] = 0.;
    }

    return n;
} /* end of node_create */


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
    printf("\n---- WEIGHTS VALUES ----\n");
    matrix_print(net->weights);
    printf("\n");
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


double
state_energy (struct matrix * weights, struct node * visible, struct node * hidden)
{
    double energy = 0.;
    for (int i = 0; i < visible->num_nodes; i++)
    {
        for (int j = 0; j < hidden->num_nodes; j++)
        {
            energy -= node_get_activation(visible, i) * \
                      matrix_get(weights, i, j) * \
                      node_get_activation(hidden, j);
        }
    }
    energy /= 2; 
    return energy;
}


double
network_energy (struct network * net)
{
    return state_energy (net->weights, net->nvisible, net->nhidden);
}



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
    printf("\n\n");
    network_print(net);

    struct node * visible = node_create(param->nodes_per_layer[0]);
    struct node * hidden = node_create(param->nodes_per_layer[1]);
    for (int i = 0; i < visible->num_nodes; i++)
    {
        node_set_activation(visible, i, matrix_get(dataset, 0, i));
    }
    node_print(visible, 1);
    printf("\n");

    double energy = state_energy(net->weights, visible, hidden); // holds the initial energy of the state of the system
    double energy_compare = 0.; // after any update, energy_compare will have the immediate energy of the state
    // energy and energy_compare will be compared, if there is a decrease in energy, update is kept,
    // otherwise the previous value of the unit remains.

    printf("\n---- UPDATE HIDDEN UNITS ----\n");
    for (int i = 0; i < hidden->num_nodes; i++)
    {
        double sum = 0.;
        double expo = 0.;
        for (int j = 0; j < visible->num_nodes; j++)
        {
            sum += node_get_activation(visible, j) * matrix_get(net->weights, i, j); 
        }
        sum += node_get_bias(visible, i);
        expo = sigmoid(sum, 1);
        if (expo < 0.5)
        {
            hidden->activation[i] = 0.;
        } else
        {
            hidden->activation[i] = 1.;
        }
    energy_compare = state_energy(net->weights, visible, hidden);
    printf("state energy 1 -> %f \t state energy 2 -> %f \n", energy, energy_compare);
    }
    node_print(hidden, 1);

    return 0;
}
