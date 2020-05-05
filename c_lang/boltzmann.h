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
    double * sum_info;
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


/*  =================================================
 *  Parameters data structure.
    =================================================
    Parameters of the whole network which will not be
    changed during training and testing are stored as
    a data structure of type parameters.
    =================================================
    dataset_file - path and name of file where dataset
                   can be found.
    dataset_rows - number of patterns in the dataset
                   corresponds to number of rows.
    dataset_cols - number of dimensions in the dataset,
                   of number of variables in dataset.
    epsilon__ - learning rate
    -------------------------------------------------
                w - refers to weights
                vb - refers to visible units biases
                hb - refers to hidden units biases
    -------------------------------------------------
    num_layers - number of layers in the network
    num_nodes_array - pointer to array where the number
                      of nodes in each layer is stored.
 */
struct parameters {
    char * dataset_file;
    size_t dataset_rows;    // Amount of examples the dataset have, each row is a pattern
    size_t dataset_cols;    // Dimension of dataset (number of features), must be equal to number of nodes in first layer
    double epsilonw;        // Learning rate for weights
    double epsilonvb;       // Learning rate for biases of visible units (Hinton ref.)
    double epsilonhb;       // Learning rate for biases of hidden units (Hinton ref.)
    int maxepochs;          // Maximum number of epochs
    size_t num_layers;      // Number of layers
    size_t * nodes_per_layer;
};



#endif /* __BOLTZMANN_H__ */
