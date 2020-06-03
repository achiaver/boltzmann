#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void
node_set_activation (struct node * n, double value)
{
    n->activation = value;
} /* end node_set_activation */


void
node_set_bias (struct node * n, double value)
{
    n->bias = value;
} /* end node_set_bias */


void
node_set_z_in (struct node * n, double value)
{
    n->z_in = value;
} /* end node_set_z_in */



void
node_randomize_bias (struct node * n)
{
    n->bias = drand48();
} /* end of node_randomize_bias */


double
node_get_activation (struct node * n)
{
    return n->activation;
} /* end node_get_activation */


double
node_get_bias (struct node * n)
{
    return n->bias;
} /* end node_get_bias */


double
node_get_z_in (struct node * n)
{
    return n->z_in;
} /* end node_get_z_in */


void
node_print(struct node * n, int option)
{
    if (option == 0) // print all node status
    {
        printf("- activation \t %f \n", node_get_activation(n));
        printf("- bias \t %f \n", node_get_bias(n));
        printf("- z_in \t %f \n", node_get_z_in(n));
        printf("\n");
    } else if (option == 1) // print only node activation
    {
        printf("- activation \t %f \n", node_get_activation(n));
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
node_create (struct node * n) 
{
    node_set_activation(n, 0.);
    node_set_bias(n, 0.);
    node_set_z_in(n, 0.);
} /* end of node_create */


