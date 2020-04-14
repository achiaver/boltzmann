#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "boltzmann.h"
#include "stats_functions.h"
#include "matrix.h"
#include <time.h> 

double drand48();

struct layer *
layer_create (struct parameters * param)
{
    struct layer * layer_bm = malloc(sizeof (*layer_bm) * (param->num_layers));
    if (!layer_bm)
    {
        printf("layer_create: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }
    srand48(time(NULL));
    for (int i = 0; i < param->num_layers; i++)
    {
        layer_bm[i].num_nodes = param->num_nodes_array[i];
        layer_bm[i].node = malloc(sizeof (struct node) * (layer_bm[i].num_nodes));
        if (!layer_bm[i].node)
        {
            printf("layer_create: malloc: node: %s\n", strerror(errno));
            exit(2);
        }

        for (int j = 0; j < layer_bm[i].num_nodes; j++)
        {
            layer_bm[i].node[j].activation = 0;
            layer_bm[i].node[j].bias = 0;
            layer_bm[i].node[j].h_in = 0;

            if (i == 0)
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param->num_nodes_array[i+1]));
                if (!layer_bm[i].node[j].weight)
                {
                    printf("layer_create: malloc: node %d: weight: %s \n", j,  strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param->num_nodes_array[i+1]; k++)
                {
//                    layer_bm[i].node[j].weight[k] = (double) (10 * (j+1)) + (k+1);
                    layer_bm[i].node[j].weight[k] = drand48();
                }
            } else
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param->num_nodes_array[i-1]));
                if (!layer_bm[i].node[j].weight)
                {
                    printf("layer_create: malloc: node %d: weight: %s\n", j, strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param->num_nodes_array[i-1]; k++)
                {
                    layer_bm[i].node[j].weight[k] = layer_bm[i-1].node[k].weight[j];
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return layer_bm;
} /* end of layer_create */


struct matrix *
weight_create(struct parameters * param)
{
    struct matrix * weight = matrix_create(param->num_nodes_array[0], param->num_nodes_array[1]); 
    matrix_randomize(weight);
    return weight;
} /* end of weight_create*/

struct network *
network_create (struct parameters * param)
{
    struct network * network_bm = malloc(sizeof (*network_bm));
    if (!network_bm) {
        printf("network_create: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    network_bm->num_layers = param->num_layers;
    network_bm->layer = layer_create(param);
    network_bm->weights = weight_create(param);
    return network_bm;
} /* end of network_create */


struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end of dataset_allocate*/


int
main(int argc, char *argv[])
{
    
    srand48(time(NULL));

    char * parameters_file = "in_parameters.dat";
    char * dataset_file = "dataset/three_node_test.csv";
    struct parameters * param = parameters_input(parameters_file, dataset_file);

    print_parameters(param);

    struct network * net = network_create(param);
    print_network_status(net);

    struct matrix* dataset = dataset_allocate(param->dataset_file, param->dataset_rows, param->dataset_cols);
    printf("\n\nInput dataset display\n\n");
    matrix_print(dataset);

    matrix_print(net->weights);


    return 0;
}
