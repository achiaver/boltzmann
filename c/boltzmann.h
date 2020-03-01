#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__

#include <stddef.h>

struct weights {
    double w;
};

struct visible {
    size_t length;
//    double b[];
//    struct weights *vw;
};

struct hidden {
    size_t lenght;
    double activation;
    double strength;
    double kvec[];
//    struct weights *hw;
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
    size_t N;           // Size of Visible layer
    size_t M;           // Size of Hidden layer
    double epsilonw;    // Learning rate for weights
    double epsilonvb;   // Learning rate for biases of visible units (Hinton ref.)
    double epsilonhb;   // Learning rate for biases of hidden units (Hinton ref.)
    int maxepochs;
};

#endif /* __BOLTZMANN_H__ */
