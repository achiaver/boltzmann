#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "random_func.h"
#include "parameters.h"
#include "matrix.h"

/*  =================================================
    Node structure.
    =================================================
    activation - vector of activation of the nodes; valeu 0 (off) or 1 (on);
    bias - vector of bias value of each node;
    nprob - weighted sum of the information each node is receiving from other nodes connected to it;
    =================================================
 */
typedef struct NODE {
    double activation;
    double bias;
    double nprob;
} node;


/*  =================================================
    Layer structure.
    =================================================
    num_nodes - number of nodes in this layer.
    nodes - weighted sum of the information each node is receiving from other nodes connected to it;
    =================================================
 */
typedef struct LAYER {
    size_t  num_nodes;
    node   *nodes;
} layer;


/*  =================================================
    Network structure.
    =================================================
    num_layers - number of layers of the network;
    nodes_per_layer - vector with the number of nodes per each layer;
    visible - struct of nodes for the visible units (both input and output);
    hidden - struct of nodes for the hidden units;
    weights - pointer to weights between two adjacent layers;
    =================================================
 */
typedef struct NETWORK {
    size_t  num_layers;
    size_t  num_nodes_total;
    layer   visible;
    layer   hidden;
    matrix *weights;
} network;


/*  =================================================
    Function Related to Node structure.
    =================================================
 */
void         node_set_activation    (node *,
                                     int,
                                     double);
void         node_set_bias          (node *,
                                     int,
                                     double);
void         node_set_nprob         (node *,
                                     int,
                                     double);
void         node_randomize_bias    (node *,
                                     int);
double       node_get_activation    (node *,
                                     int);
double       node_get_bias          (node *,
                                     int);
double       node_get_nprob         (node *,
                                     int);
void         node_print             (node *,
                                     int,
                                     int);
void         node_copy              (node *,
                                     node *);
void         node_create            (node *,
                                     int);


/*  =================================================
    Function Related to Layer structure.
    =================================================
 */
layer       *layer_create           (size_t);
void         layer_delete           (layer *);
void         layer_print            (layer *, int);
void         layer_copy_from_array  (layer *,
                                     matrix *, 
                                     int);

layer       *hidden_from_visible    (network *net,
                                     layer   *visible,
                                     double   T);
layer       *visible_from_hidden    (network *net,
                                     layer   *hidden,
                                     double   T);


/*  =================================================
    Function Related to Network structure.
    =================================================
 */
network     *network_create         (parameters *param);
void         network_delete         (network    *net);
void         network_print          (network    *net,
                                     int         to_prototype_is_to_document_check_the_difference);
void         network_dump           (network    *net,
                                     int         show_values,
                                     int         show_weights, 
                                     int         show_biases);
void         network_training       (network    *net,
                                     parameters *param,
                                     matrix     *data);



double       func_sigmoid           (double,
                                     double);
double       func_energy            (matrix *, 
                                     layer *, 
                                     layer *);

#endif /* __BOLTZMANN_H__ */
