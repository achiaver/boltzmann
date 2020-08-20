#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

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
typedef struct PARAMETERS {
    char   *dataset_filename;
    size_t  dataset_rows;
    size_t  dataset_cols;
    char   *weights_filename;
    size_t  weights_rows;
    size_t  weights_cols;
    char   *biases_filename;
    size_t  biases_rows;
    size_t  biases_cols;
    double  temp_start;
    double  temp_end;
    double  temp_step;
    size_t  tries_per_node;
    double  epsilonw;        // Learning rate for weights
    double  epsilonvb;       // Learning rate for biases of visible units (Hinton ref.)
    double  epsilonhb;       // Learning rate for biases of hidden units (Hinton ref.)
    int     maxepochs;       // Maximum number of epochs
    size_t  num_layers;      // Number of layers
    size_t *nodes_per_layer;  // Array, each entry is the number of nodes of the layer 
//    size_t  num_visible;
//    size_t  num_hidden;
} parameters;



parameters *parameters_create (void);
void        parameters_delete (parameters *);
parameters *parameters_input  (char *);
void        parameters_print  (parameters *);

#endif /* __PARAMETERS_H__ */
