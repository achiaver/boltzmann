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
struct node {
    double activation;
    double bias;
    double nprob;
};


/*  =================================================
    Layer structure.
    =================================================
    num_nodes - number of nodes in this layer.
    nodes - weighted sum of the information each node is receiving from other nodes connected to it;
    =================================================
 */
struct layer {
    size_t num_nodes;
    struct node * nodes;
};


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
struct network {
    size_t num_layers;
    size_t num_nodes_total;
    struct layer visible;
    struct layer hidden;
    struct matrix * weights;
};


/*  =================================================
    Function Related to Node structure.
    =================================================
 */
void          node_set_activation   (struct node *, int, double);
void          node_set_bias         (struct node *, int, double);
void          node_set_nprob        (struct node *, int, double);
void          node_randomize_bias   (struct node *, int);
double        node_get_activation   (struct node *, int);
double        node_get_bias         (struct node *, int);
double        node_get_nprob        (struct node *, int);
void          node_print            (struct node *, int, int);
void          node_copy             (struct node *, struct node *);
void          node_create           (struct node *, int);


/*  =================================================
    Function Related to Layer structure.
    =================================================
 */
struct layer * layer_create (size_t);
void           layer_delete (struct layer *);
void           layer_print  (struct layer *, int);
void           layer_copy_from_array   (struct layer *     layer,
                                         struct matrix *    matrix, 
                                         int                row);

struct layer *  hidden_from_visible (struct network * net,
                                     struct layer   * visible,
                                     double           T);
struct layer *  visible_from_hidden (struct network * net,
                                     struct layer   * hidden,
                                     double           T);


/*  =================================================
    Function Related to Network structure.
    =================================================
 */
struct network * network_create     (struct parameters *    param);
void             network_delete     (struct network *       net);
void             network_print      (struct network *       net,
                                     int                    to_prototype_is_to_document_check_the_difference);
void             network_dump       (struct network *       net,
                                     int                    show_values,
                                     int                    show_weights, 
                                     int                    show_biases);
void             network_training   (struct network *       net,
                                     struct parameters *    param,
                                     struct matrix *        data);



double func_sigmoid (double, double);
double func_energy  (struct matrix * m, 
                     struct layer * lay_1, 
                     struct layer * lay_2);

#endif /* __BOLTZMANN_H__ */
