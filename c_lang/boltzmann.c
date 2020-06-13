#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "parameters.h"
#include "matrix.h"
#include "boltzmann.h"
#include <time.h>
#include <math.h>


#define LEN(arr) ((int) (sizeof (arr)) / sizeof (arr[0]))

void
initialize_seed(void)
{
    srand48(time(NULL));
    srand(time(NULL));
}


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
node_set_nprob (struct node * n, int node, double value)
{
    n[node].nprob = value;
} /* end node_set_nprob */


void
node_randomize_bias (struct node * n, int node)
{
    n[node].bias = drand48();
} /* end node_randomize_bias */


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
node_get_nprob (struct node * n, int node)
{
    return n[node].nprob;
} /* end node_get_nprob */


void
node_print(struct node * n, int node, int option)
{
    if (option == 0) // print all node status
    {
        printf("\t- activation \t %f \n", node_get_activation(n, node));
        printf("\t- bias \t %f \n", node_get_bias(n, node));
        printf("\t- nprob \t %f \n", node_get_nprob(n, node));
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
} /* end node_copy */


void
node_create (struct node * n, int node)
{
    node_set_activation(n, node, 0.);
    node_set_bias(n, node, 0.);
    node_set_nprob(n, node, 0.);
} /* end node_create */


struct layer *
layer_create (size_t num_nodes)
{
    struct layer * l = malloc(sizeof (*l));
    if (!l) {
        printf("layer_create: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }

    l->num_nodes = num_nodes;
    l->nodes = malloc(sizeof (*l->nodes) * (l->num_nodes));
    for (int i = 0; i < l->num_nodes; i++)
    {
        node_create(l->nodes, i);
    }

    return l;
} /* end layer_creat */


void
layer_delete (struct layer * l, int option)
{
    if (l)
    {
        printf("---- \t DELETING LAYER\n");
        if (option == 0)
        {
            free(l->nodes);
            free(l);
        } else
        {
            free(l->nodes);
        }
    }
    printf("----> \t DELETED\n");
} /* end layer_delete */


void
layer_print (struct layer * l, int option)
{
    for (int i = 0; i < l->num_nodes; i++)
    {
        printf("Node %d\n", i);
        node_print(l->nodes, i, option);
    }
} /* end layer_print */


void
network_print (struct network * net, int option)
{
    printf("---- NETWORK STATUS ----\n");
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
} /* end network_print */


void
network_delete (struct network * net)
{
    if (net)
    {
        printf("---- \t DELETING NETWORK\n");
        layer_delete(&net->visible, 1);
        layer_delete(&net->hidden, 1);
        printf("---- \t DELETING WEIGHTS\n");
        matrix_destroy(net->weights);
        printf("----> \t DELETED\n");
        free(net);
    }
    printf("----> \t NETWORK DELETED!\n\n");
} /* end network_delete */


struct matrix *
weight_create(size_t visible, size_t hidden)
{
    struct matrix * weight = matrix_create(visible, hidden);
    matrix_randomize(weight);
    return weight;
} /* end weight_create*/


struct network *
network_create (struct parameters * param)
{
    struct network * net = malloc(sizeof (*net));
    if (!net) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }

    net->num_layers = param->num_layers;

    net->visible.num_nodes = param->num_visible;
    net->visible.nodes = malloc(sizeof (*net->visible.nodes) * (net->visible.num_nodes));
    if (!net->visible.nodes)
    {
        printf("network_create: malloc: network visible nodes: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < net->visible.num_nodes; i++)
    {
        node_create(net->visible.nodes, i);
    }


    net->hidden.num_nodes = param->num_hidden;
    net->hidden.nodes = malloc(sizeof (*net->hidden.nodes) * (net->hidden.num_nodes));
    if (!net->hidden.nodes)
    {
        printf("network_create: malloc: network hidden nodes: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < net->hidden.num_nodes; i++)
    {
        node_create(net->hidden.nodes, i);
    }

    net->weights = weight_create(net->visible.num_nodes, net->hidden.num_nodes);

    return net;
} /* end network_create */


struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end dataset_allocate*/


void
dataset_destroy (struct matrix * m)
{
    printf("---- \t DELETING DATASET\n");
    matrix_destroy(m);
    printf("----> \t DATASET DELETED\n\n");
}


double
sigmoid (double expoent, double temp)
{
    return (1 / (1 + exp(-expoent/temp)));
} /* end sigmoid */


double
state_energy (struct matrix * weights, struct layer * lay_1, struct layer * lay_2)
{
    double energy = 0.;
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        for (int j = 0; j < lay_2->num_nodes; j++)
        {
            energy -= node_get_activation(lay_1->nodes, i) * \
                      matrix_get(weights, i, j) * \
                      node_get_activation(lay_2->nodes, j);
        }
        energy -= node_get_activation(lay_1->nodes, i) * \
                  node_get_bias(lay_1->nodes, i);
    }
    for (int j = 0; j < lay_2->num_nodes; j++)
    {
        energy -= node_get_activation(lay_2->nodes, j) * \
                  node_get_bias(lay_2->nodes, j);
    }

    return energy;
}


double
network_energy (struct network * net)
{
    return state_energy(net->weights, &net->visible, &net->hidden);
}


struct state *
state_create (size_t num_units)
{
    struct state * st = malloc (sizeof (*st));
    if (!st)
    {
        printf("state_create: malloc: state: %s \n", strerror(errno));
        exit(2);
    }

    st->num_units = num_units;

    st->unit = malloc (sizeof (*st->unit) * (st->num_units));
    if (!st->unit)
    {
        printf("state_create: malloc: state units %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < st->num_units; i++)
    {
        st->unit[i] = rand() % 2;
    }

    return st;
}


void
state_destroy (struct state * st)
{
    free(st->unit);
    free(st);
    printf("----> State deleted! \n");
}


void
state_print (struct state * st)
{
    printf("---- STATE STATUS ----\n");
    for (int i = 0; i < st->num_units; i++)
    {
        printf("%f, \t", st->unit[i]);
    }
    printf("\n\n");
}



/* ==========================================
 * ==========================================
 *
 * Functions from RestrictedBoltzmannProgram.cs
 *
 * ==========================================
 * ========================================== */

void
dataset_print(double data[12][6], int num_rows)
{
    int cols = (int) sizeof(data[0]) / sizeof(data[0][0]);
    for (int i = 0; i < num_rows; i++)
    {
        printf("[%2d] - ", i);
        for (int j = 0; j < cols; j++)
        {
            printf("%2.0f ", data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fisher-Yates shuffle algorithm
// swap and shuffle functions
void
swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


void
shuffle(int arr[], int n)
{
    for (int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

struct matrix *
outerproduct (struct layer * lay_1, struct layer * lay_2)
{
    struct matrix * outerprod = matrix_create(lay_1->num_nodes, lay_2->num_nodes);
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        for (int j = 0; j < lay_2->num_nodes; j++)
        {
            matrix_set(outerprod, i, j, node_get_activation(lay_1->nodes, i) * node_get_activation(lay_2->nodes, j));
        }
    }

    return outerprod;
}



void
network_training(struct network * net, struct parameters * param, double data[12][6])
{
    int *indices = malloc(sizeof (int) * param->dataset_rows);
    int indices_len = param->dataset_rows;
    if (!indices)
    {
        printf("network_training: malloc: indices: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < indices_len; i++)
    {
        indices[i] = i;
    }

    int epoch = 0;
    while (epoch < param->maxepochs)
    {
        shuffle(indices, indices_len);

        for (int idx = 0; idx < indices_len; idx++) // for each point in the dataset
        {
            int i = indices[idx];

            // Copy train data i into network visible layer (nodes)
            for (int v = 0; v < net->visible.num_nodes; v++)
            {
                node_set_activation(net->visible.nodes, v, data[i][v]);
            }

            // Compute hidden nodes values
            for (int h = 0; h < net->hidden.num_nodes; h++)
            {
                double sum = 0.;
                for (int v = 0; v < net->visible.num_nodes; v++)
                {
                    sum += node_get_activation(net->visible.nodes, v) * matrix_get(net->weights, v, h);
                }
                sum += node_get_bias(net->hidden.nodes, h);

                node_set_nprob(net->hidden.nodes, h, sigmoid(sum, 1));
                if (node_get_nprob(net->hidden.nodes, h) > drand48())
                {
                    node_set_activation(net->hidden.nodes, h, 1.);
                } else
                {
                    node_set_activation(net->hidden.nodes, h, 0.);
                }
            } // end for compute hidden (1)

            // Compute positive gradiente -> outer product of v and h.
            struct matrix * positive_grad = outerproduct(&net->visible, &net->hidden);
//            printf("positive gradiente %d \n", idx);
//            matrix_print(positive_grad);
//            printf("\n");

            // Reconstruct visible v'


        } // end for idx
//        printf("Done epoch %d \n\n", epoch);
        epoch++;
    }
}

int
main(int argc, char *argv[])
{
    initialize_seed();

    printf("\nBegin Restricted Boltzmann Machine demo\n");
    printf("Films: Alien, Inception, Spy, Eurotrip, Gladiator, Spartacus\n\n");

    double dataset[12][6] = {{ 1, 1, 0, 0, 0, 0 },   // A
                             { 0, 0, 1, 1, 0, 0 },   // B
                             { 0, 0, 0, 0, 1, 1 },   // C
                             { 1, 1, 0, 0, 0, 1 },   // noisy A
                             { 0, 0, 1, 1, 0, 0 },   // B
                             { 0, 0, 0, 0, 1, 1 },   // C
                             { 1, 0, 0, 0, 0, 0 },   // weak A
                             { 0, 0, 1, 0, 0, 0 },   // weak B
                             { 0, 0, 0, 0, 1, 0 },   // weak C
                             { 1, 1, 0, 1, 0, 0 },   // noisy A
                             { 1, 0, 1, 1, 0, 0 },   // noisy B
                             { 0, 0, 1, 0, 1, 1 }};  // noisy C

    printf("Film like-dislike data is: \n");
    dataset_print(dataset, 12);

    struct parameters * param = parameters_create();
    param->num_layers = 2;
    param->num_visible = 6;
    param->num_hidden = 3;

    printf("Creating a RBM ... \n");
    printf("Setting number of visible nodes = %zu \n", param->num_visible);
    printf("Setting number of hidden nodes = %zu \n\n", param->num_hidden);

    struct network * net = network_create(param);
//    network_delete(net);

    param->dataset_rows = 12;
    param->dataset_cols = 6;
    param->epsilonw = 0.01;
    param->epsilonvb = 0.01;
    param->epsilonhb = 0.01;
    param->maxepochs = 2;

    printf("Training RBM using CD1 algorithm \n");
    printf("Setting learning rate (weights and biases) = %f \n", param->epsilonw);
    printf("Setting maximum amount of epochs = %d \n", param->maxepochs);

    network_training(net, param, dataset);
    matrix_print(net->weights);

//    char * parameters_file = "in_parameters.dat";
//    char * dataset_file = "dataset/three_node_test.csv";
//    struct parameters * param = parameters_input(parameters_file, dataset_file);
//    parameters_print(param);

//    struct matrix * dataset = dataset_allocate(param->dataset_file, param->dataset_rows, param->dataset_cols);
//    printf("\n\nInput dataset display\n\n");
//    matrix_print(dataset);

//    struct network * net = network_create(param);
//    printf("\n\n");
//    network_print(net, 0);

//    struct layer * visible = layer_create(param->num_visible);
//    struct layer * visible_aux = layer_create(param->num_visible);
//    struct layer * hidden = layer_create(param->num_hidden);
//    for (int i = 0; i < visible->num_nodes; i++)
//    {
//        node_set_activation(visible->nodes, i, matrix_get(dataset, 0, i));
//    }
//    layer_print(visible, 1);
//    printf("\n");
//
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
//            sum += node_get_activation(visible->nodes, j) * matrix_get(net->weights, j, i);
//        }
//        sum += node_get_bias(visible->nodes, i);
//        expo = sigmoid(sum, 1);
//        if (expo < 0.5)
//        {
//            node_set_activation(hidden->nodes, i, 0.);
//        } else
//        {
//            node_set_activation(hidden->nodes, i, 1.);
//        }
//    energy_compare = state_energy(net->weights, visible, hidden);
//    printf("state energy 1 -> %f \t state energy 2 -> %f \n", energy, energy_compare);
//    }
//    layer_print(hidden, 1);
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
//            sum += node_get_activation(hidden->nodes, j) * matrix_get(net->weights, i, j);
//        }
//        sum += node_get_bias(net->visible.nodes, i);
//        expo = sigmoid(sum, 1);
//        if (expo < 0.5)
//        {
//            node_set_activation(visible_aux->nodes, i, 0.);
//        } else
//        {
//            node_set_activation(visible_aux->nodes, i, 1.);
//        }
//    energy_compare = state_energy(net->weights, visible, hidden);
//    printf("state energy 1 -> %f \t state energy 2 -> %f \n", energy, energy_compare);
//    }
//    layer_print(visible_aux, 1);
//
//    printf("network energy - %f\n\n", network_energy(net));
//
//    struct state * st = state_create(param->num_visible + param->num_hidden);
//    state_print(st);
//    state_destroy(st);
//
//
//    dataset_destroy(dataset);
//    layer_delete(visible, 0);
//    printf("\n");
//    network_delete(net);
//
//
//    parameters_delete(param);

    return 0;
}
