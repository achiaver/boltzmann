#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__


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
    size_t num_nodes;
    double * activation;
    double * bias;
    double * z_in;
};


/*  =================================================
    Network data structure.
    =================================================
    num_layers - number of layers of the network;
    nodes_per_layer - vector with the number of nodes per each layer;
    nvisible - struct of nodes for the visible units (both input and output);
    nhidden - struct of nodes for the hidden units;
    weights - pointer to weights between two adjacent layers;
    =================================================
 */
struct network {
    size_t num_layers;
    size_t * nodes_per_layer;
    struct node * nvisible;
    struct node * nhidden;
    struct matrix * weights;
};

#endif /* __BOLTZMANN_H__ */
