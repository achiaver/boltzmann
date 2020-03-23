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

struct node {
    double activation;
    double *weight;
    double *bias;
    double h_in;
};

struct layer {
    size_t num_nodes;
//    int kind;
    struct node *node;
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
    size_t N;           // Number of units in Visible layer
    size_t M;           // Number of units in Hidden layer
};



#endif /* __BOLTZMANN_H__ */
