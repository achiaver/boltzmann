#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__

#include <stddef.h>

struct visible {
    size_t length;
    double b[];
};

struct hidden {
    double activation;
    double strength;
    double kvec[];
};

struct parameters {
    size_t N;           // Size of Representation layer
    size_t M;           // Size of Knowledge layer
};

#endif /* __BOLTZMANN_H__ */
