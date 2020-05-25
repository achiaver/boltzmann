#ifndef __NODE_H__
#define __NODE_H__

/*  =================================================
    Node data structure.
    =================================================
    num_nodes - number of nodes;
    activation - vector of activation of the nodes; valeu 0 (off) or 1 (on);
    bias - vector of bias value of each node;
    sum_info - weighted sum of the information each node is receiving;
    =================================================
 */
struct node {
//    size_t num_nodes;
    double activation;
    double bias;
    double z_in;
};

void node_set_activation (struct nodei, double);
double node_get_activation (struct node, size_t node)

void node_set_bias (struct node * n, size_t node, double value)

void node_randomize_bias (struct node * n)

double node_get_bias (struct node * n, size_t node)

void node_print(struct node * n, int option)

void node_set_z_in (struct node * n, size_t node, double value)

double node_get_z_in (struct node * n, size_t node)

void node_copy (struct node * n1, struct node * n2)

struct node * node_create (size_t num_nodes) 



#endif
