#include "node.h"

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
node_set_z_in (struct node * n, size_t node, double value)
{
    n->z_in[node] = value;
} /* end node_set_z_in */


double
node_get_z_in (struct node * n, size_t node)
{
    return n->z_in[node];
} /* end node_get_z_in */


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
//    n->activation = malloc( sizeof (n->activation) * num_nodes);
    n->activation = malloc( sizeof (double) * num_nodes);
    if(!n->activation)
    {
        printf("node_create: nodes->activation: malloc: %s\n", strerror(errno));
        exit(2);
    }

//    n->bias = malloc( sizeof (n->bias) * num_nodes);
    n->bias = malloc( sizeof (double) * num_nodes);
    if(!n->bias)
    {
        printf("node_create: nodes->bias: malloc: %s\n", strerror(errno));
        exit(2);
    }

//    n->z_in = malloc( sizeof (n->z_in) * num_nodes);
    n->z_in = malloc( sizeof (double) * num_nodes);
    if(!n->z_in)
    {
        printf("node_create: nodes->info: malloc: %s\n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < num_nodes; i++)
    {
        n->activation[i] = 0.;
//        n->bias[i] = 0.;
        node_randomize_bias(n);
        n->z_in[i] = 0.;
    }

    return n;
} /* end of node_create */

