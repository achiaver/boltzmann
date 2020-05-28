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
    double activation;
    double bias;
    double z_in;
};

void          node_set_activation   (struct node *, double);
void          node_set_bias         (struct node *, double);
void          node_set_z_in         (struct node *, double);
void          node_randomize_bias   (struct node *);
double        node_get_activation   (struct node *);
double        node_get_bias         (struct node *);
double        node_get_z_in         (struct node *);
void          node_print            (struct node *, int);
void          node_copy             (struct node *, struct node *);
void          node_create           (struct node *);



#endif
