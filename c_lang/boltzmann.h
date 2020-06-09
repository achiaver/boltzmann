#ifndef __BOLTZMANN_H__
#define __BOLTZMANN_H__


/*  =================================================
    Node structure.
    =================================================
    activation - vector of activation of the nodes; valeu 0 (off) or 1 (on);
    bias - vector of bias value of each node;
    z_in - weighted sum of the information each node is receiving from other nodes connected to it;
    =================================================
 */
struct node {
    double activation;
    double bias;
    double z_in;
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
//    size_t * nodes_per_layer;
    struct layer visible;
    struct layer hidden;
    struct matrix * weights;
};


struct state {
    size_t num_units;
    double *unit;
};




/*  =================================================
    Function Related to Node structure.
    =================================================
 */
void          node_set_activation   (struct node *, int, double);
void          node_set_bias         (struct node *, int, double);
void          node_set_z_in         (struct node *, int, double);
void          node_randomize_bias   (struct node *, int);
double        node_get_activation   (struct node *, int);
double        node_get_bias         (struct node *, int);
double        node_get_z_in         (struct node *, int);
void          node_print            (struct node *, int, int);
void          node_copy             (struct node *, struct node *);
void          node_create           (struct node *, int);


/*  =================================================
    Function Related to Layer structure.
    =================================================
 */
struct layer * layer_create (size_t);
void           layer_delete (struct layer *, int);
void           layer_print  (struct layer *, int);

/*  =================================================
    Function Related to Network structure.
    =================================================
 */
struct network * network_create (struct parameters *);
void             network_delete (struct network *);
void             network_print  (struct network *, int);




#endif /* __BOLTZMANN_H__ */
