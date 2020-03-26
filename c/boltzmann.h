#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__

#include <stddef.h>
#include <stdlib.h>

//struct weights {
//    double w;
//};
//
//struct visible {
//    size_t length;
////    double b[];
////    struct weights *vw;
//};
//
//struct hidden {
//    size_t lenght;
//    double activation;
//    double strength;
//    double kvec[];
////    struct weights *hw;
//};

/*
    Node data structure.
    =================================================
    activation - 0 (off) or 1 (on); indicates node status
    weight - connection value between one node and the other nodes
    bias - bias value of one node
    h_in - weighted sum of information on node is receiving
    =================================================
 */
struct node {
    double activation;
    double* weight;
    double bias;
    double h_in;
};


/*
    Layer data structure.
    =================================================
    num_nodes - number of nodes in one layer
    node - pointer to initial node of one layer
    =================================================
 */
struct layer {
    size_t num_nodes;
//    int kind;
    struct node* node;
};


/*
    Network data structure.
    =================================================
    num_layers - number of layers in the network
    layer - pointer to initial layer of the network
    =================================================
 */
struct network {
    size_t num_layers;
    struct layer* layer;
};

/*
    Parameters of the whole network which will not be
    changed during training and testing.
    =================================================
    N - number of visible units in visible layer
    M - number of hidden units in hidden layer
    epsilon__ - learning rate
    =================================================
    w - refers to weights
    vb - refers to visible units biases
    hb - refers to hidden units biases
 */
struct parameters {
    double epsilonw;    // Learning rate for weights
    double epsilonvb;   // Learning rate for biases of visible units (Hinton ref.)
    double epsilonhb;   // Learning rate for biases of hidden units (Hinton ref.)
    int maxepochs;      // Maximum number of epochs
    size_t num_layers;  // Number of layers
//    size_t N;           // Number of units in Visible layer
//    size_t M;           // Number of units in Hidden layer
    size_t* num_nodes_array;
};



#endif /* __BOLTZMANN_H__ */
