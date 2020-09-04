#include "boltzmann.h"

/* =============================================================================
 * node functions
 */

// node_set_activation assign the given value to the activation of the given node

void
node_set_activation (node *n, int node, double value)
{
    n[node].activation = value;
} /* end node_set_activation */


// node_set_bias assign the given value to the bias of the given node

void
node_set_bias (node *n, int node, double value)
{
    n[node].bias = value;
} /* end node_set_bias */


// node_set_nprob assign the given value to the nprob of the given node
// nprob refers to the probability value of the node.

void
node_set_nprob (node *n, int node, double value)
{
    n[node].nprob = value;
} /* end node_set_nprob */


// node_randomize_bias assigns a random value to bias of give node

void
node_randomize_bias (node *n, int node)
{
    n[node].bias = random_0to1();
} /* end node_randomize_bias */

// node_get_activation retrieves the activation value of the given node

double
node_get_activation (node *n, int node)
{
    return n[node].activation;
} /* end node_get_activation */


// node_get_bias retrieves the bias value of the given node

double
node_get_bias (node *n, int node)
{
    return n[node].bias;
} /* end node_get_bias */


// node_get_nprob retrieves the nprob value of the given node

double
node_get_nprob (node *n, int node)
{
    return n[node].nprob;
} /* end node_get_nprob */


void
node_print(node *n, int node, int option)
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
node_copy (node *n1, node *n2)
{
    *n2 = *n1;
} /* end node_copy */


void
node_create (node *n, int node)
{
    node_set_activation(n, node, 0.);
    node_set_bias(n, node, 0.);
    node_set_nprob(n, node, 0.);
} /* end node_create */


/* =============================================================================
 * layer functions
 * */

// layer_create creates a layer variable

layer *
layer_create (size_t num_nodes)
{
    layer *l = malloc(sizeof (*l));
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


// layer_delete deletes the layer
void
layer_delete (layer *l)
{
    if (l->nodes)
    {
        free(l->nodes);
        l->nodes = NULL;
    }
    l = NULL;
    printf("----> \t layer deleted...\n");
} /* end layer_delete */


void
layer_print (layer *l, int option)
{
    for (int i = 0; i < l->num_nodes; i++)
    {
        printf("%2.0f ", node_get_activation(l->nodes, i));
    }
    if (option == 1)
    {
        printf("\n");
    }
} /* end layer_print */


/* =============================================================================
 * network functions
 * */

void
network_print (network *net, int option)
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
    matrix_print(net->weights, 1);
    printf("\n");
} /* end network_print */


void
network_delete (network *net)
{
    if (net)
    {
        if (net->visible.nodes) 
        {
            layer_delete(&net->visible);
        }
        if (net->hidden.nodes)
        {
            layer_delete(&net->hidden);
        }
        matrix_destroy(net->weights);
        printf("----> \t weights deleted...\n");
        free(net);
        net = NULL;
    }
    printf("----> \t network deleted!\n\n");
} /* end network_delete */


matrix *
weight_create(size_t visible, size_t hidden)
{
    matrix *weight = matrix_create(visible, hidden);
    matrix_randomize(weight);
    return weight;
} /* end weight_create */


network *
network_create (parameters *param)
{
    network *net = malloc(sizeof (*net));
    if (!net) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }

    net->num_layers = param->num_layers;

//    net->visible.num_nodes = param->num_visible;
    net->visible.num_nodes = param->nodes_per_layer[0];
    net->visible.nodes = malloc(sizeof (*net->visible.nodes) * (net->visible.num_nodes));
    if (!net->visible.nodes)
    {
        printf("network_create: malloc: network visible nodes: %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < net->visible.num_nodes; i++)
    {
        node_create(net->visible.nodes, i);
        node_set_bias(net->visible.nodes, i, random_0to1());
    }

//    net->hidden.num_nodes = param->nodes_per_layer[1];
    net->hidden.num_nodes = param->nodes_per_layer[1];
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

    net->num_nodes_total = net->visible.num_nodes + net->hidden.num_nodes;
    net->weights = weight_create(net->visible.num_nodes, net->hidden.num_nodes);

    return net;
} /* end network_create */


double
func_sigmoid (double expoent, double temp)
{
    return (1. / (1. + exp(-expoent/temp)));
} /* end func_sigmoid */


double
func_energy (matrix *weights, layer *lay_1, layer *lay_2)
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
} /* end func_energy */


double
network_energy (network *net)
{
    return func_energy(net->weights, &net->visible, &net->hidden);
} /* end network_energy */


// Fisher-Yates shuffle algorithm
// swap and shuffle functions
void
swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
} /*end swap */


void
shuffle (int arr[], int n)
{
    for (int i = n-1; i > 0; i--)
    {
        int j = random_bounded(i+1); // Pick a random index from 0 to i
//        printf("j - %d\n", j);
        swap(&arr[i], &arr[j]);
    }
} /* end shuffle */


matrix *
outerproduct (layer *lay_1, layer *lay_2)
{
    matrix *outerprod = matrix_create(lay_1->num_nodes, lay_2->num_nodes);
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        for (int j = 0; j < lay_2->num_nodes; j++)
        {
            matrix_set(outerprod, i, j, node_get_activation(lay_1->nodes, i) * node_get_activation(lay_2->nodes, j));
        }
    }
    return outerprod;
} /* end outerproduct */


void
network_training (network *net, parameters *param, matrix *data)
{
    double learning_rate = param->epsilonw;
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
                node_set_activation(net->visible.nodes, v, matrix_get(data, i, v));
            }

            // Compute hidden nodes values on the network
            for (int h = 0; h < net->hidden.num_nodes; h++)
            {
                double sum = 0.;
                for (int v = 0; v < net->visible.num_nodes; v++)
                {
                    sum += node_get_activation(net->visible.nodes, v) * matrix_get(net->weights, v, h);
                }
                sum += node_get_bias(net->hidden.nodes, h);

                node_set_nprob(net->hidden.nodes, h, func_sigmoid(sum, 1));
                if (node_get_nprob(net->hidden.nodes, h) > random_0to1())
                {
                    node_set_activation(net->hidden.nodes, h, 1.);
                } else
                {
                    node_set_activation(net->hidden.nodes, h, 0.);
                }
            } // end for compute hidden (1)

            // Compute positive gradiente -> outer product of v and h.
            matrix *positive_grad = outerproduct(&net->visible, &net->hidden);

            // Reconstruct visible v' from h
            layer *visible_prime = layer_create(net->visible.num_nodes);
            for (int v = 0; v < visible_prime->num_nodes; v++)
            {
                double sum = 0.;
                for (int h = 0; h < net->hidden.num_nodes; h++)
                {
                    sum += node_get_activation(net->hidden.nodes, h) * matrix_get(net->weights, v, h);
                }
                sum += node_get_bias(net->visible.nodes, v);

                node_set_nprob(visible_prime->nodes, v, func_sigmoid(sum, 1));
                if (node_get_nprob(visible_prime->nodes, v) > random_0to1())
                {
                    node_set_activation(visible_prime->nodes, v, 1.);
                } else
                {
                    node_set_activation(visible_prime->nodes, v, 0.);
                }
            }

            // Reconstruct hidden h' from v'
            layer *hidden_prime = layer_create(net->hidden.num_nodes);
            for (int h = 0; h < hidden_prime->num_nodes; h++)
            {
                double sum = 0.;
                for (int v = 0; v < visible_prime->num_nodes; v++)
                {
                    sum += node_get_activation(visible_prime->nodes, v) * matrix_get(net->weights, v, h);
                }
                sum += node_get_bias(net->hidden.nodes, h);

                node_set_nprob(hidden_prime->nodes, h, func_sigmoid(sum, 1));
                if (node_get_nprob(hidden_prime->nodes, h) > random_0to1())
                {
                    node_set_activation(hidden_prime->nodes, h, 1.);
                } else
                {
                    node_set_activation(hidden_prime->nodes, h, 0.);
                }
            }

            // Compute negative gradiente -> outer product of v' and h'
            matrix *negative_grad = outerproduct(visible_prime, hidden_prime);

            // update weights
            for (int row = 0; row < net->visible.num_nodes; row++)
            {
                for (int col = 0; col < net->hidden.num_nodes; col++)
                {
                    matrix_set(net->weights, row, col, (matrix_get(net->weights, row, col) + learning_rate * (matrix_get(positive_grad, row, col) - matrix_get(negative_grad, row, col))));
                }
            }

            // update visible biases
            for (int v = 0; v < net->visible.num_nodes; v++)
            {
                node_set_bias(net->visible.nodes, v, (node_get_bias(net->visible.nodes, v) + learning_rate * (node_get_activation(net->visible.nodes, v) - node_get_activation(visible_prime->nodes, v))));
            }

            //update hidden biases
            for (int h = 0; h < net->hidden.num_nodes; h++)
            {
                node_set_bias(net->hidden.nodes, h, (node_get_bias(net->hidden.nodes, h) + learning_rate * (node_get_activation(net->hidden.nodes, h) - node_get_activation(hidden_prime->nodes, h))));
            }

        } // end for idx
        epoch++;
    }
} /* end network_training */


void
network_dump (network *net, int show_values, int show_weights, int show_biases)
{
    if (show_values == 1)
    {
        for (int i = 0; i < net->visible.num_nodes; i++)
        {
            printf("visible node [%4d] value = %f ", i,node_get_activation(net->visible.nodes, i));
            printf(" - prob = %f \n", node_get_nprob(net->visible.nodes, i));
        }
        printf("\n");

        for (int j = 0; j < net->hidden.num_nodes; j++)
        {
            printf("hidden node [%4d] value = %f ", j, node_get_activation(net->hidden.nodes, j));
            printf(" - prob = %f \n", node_get_nprob(net->hidden.nodes, j));
        }
        printf("\n");
    }

    if (show_weights == 1)
    {
        matrix_print(net->weights, 1);
    }
    printf("\n");

    if (show_biases == 1)
    {
        for (int i = 0; i < net->visible.num_nodes; i++)
        {
            printf("visible bias [%4d] value = %f \n", i, node_get_bias(net->visible.nodes, i));
        }
        printf("\n");

        for (int j = 0; j < net->hidden.num_nodes; j++)
        {
            printf("hidden bias [%4d] value = %f \n", j, node_get_bias(net->hidden.nodes, j));
        }
        printf("\n");
    }
} /* end network_dump */


layer *
visible_from_hidden (network *net, layer *hidden, double T)
{
    layer *visible = layer_create(net->visible.num_nodes);
    for (int v = 0; v < visible->num_nodes; v++)
    {
        double sum = 0.;
        for (int h = 0; h < hidden->num_nodes; h++)
        {
            sum += node_get_activation(hidden->nodes, h) * matrix_get(net->weights, v, h);
        }
        sum += node_get_bias(net->visible.nodes, v);

        node_set_nprob(visible->nodes, v, func_sigmoid(sum, T));
        if (node_get_nprob(visible->nodes, v) > random_0to1())
        {
            node_set_activation(visible->nodes, v, 1.);
        } else
        {
            node_set_activation(visible->nodes, v, 0.);
        }
    }
    return visible;
} /* end visible_from_hidden */


layer *
hidden_from_visible (network *net, layer *visible, double T)
{
    layer *hidden = layer_create(net->hidden.num_nodes);
    for (int h = 0; h < hidden->num_nodes; h++)
    {
        double sum = 0.;
        for (int v = 0; v < visible->num_nodes; v++)
        {
            sum += node_get_activation(visible->nodes, v) * matrix_get(net->weights, v, h);
        }
        sum += node_get_bias(net->hidden.nodes, h);

        node_set_nprob(hidden->nodes, h, func_sigmoid(sum, T));
        if (node_get_nprob(hidden->nodes, h) > random_0to1())
        {
            node_set_activation(hidden->nodes, h, 1.);
        } else
        {
            node_set_activation(hidden->nodes, h, 0.);
        }
    }
    return hidden;
} /* end hidden_from_visible */


void
layer_copy_from_array (layer *l, matrix *m, int row)
{
    for (int i = 0; i < l->num_nodes; i++)
    {
        node_set_activation(l->nodes, i, matrix_get(m, row, i));
    }
} /* end layer_copy_from_array */

