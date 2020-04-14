#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "boltzmann.h"
#include "stats_functions.h"
#include "matrix.h"


struct layer *
create_layer (struct parameters * param)
{
    struct layer * layer_bm = malloc(sizeof (*layer_bm) * (param->num_layers));
    if (!layer_bm)
    {
        printf("create_layer: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < param->num_layers; i++)
    {
        layer_bm[i].num_nodes = param->num_nodes_array[i];
        layer_bm[i].node = malloc(sizeof (struct node) * (layer_bm[i].num_nodes));
        if (!layer_bm[i].node)
        {
            printf("create_layer: malloc: node: %s\n", strerror(errno));
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
                    printf("create_layer: malloc: node %d: weight: %s \n", j,  strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param->num_nodes_array[i+1]; k++)
                {
//                    printf("i - %d \t j - %d \t k - %d \t", i, j, k);
                    layer_bm[i].node[j].weight[k] = (double) (10 * (j+1)) + (k+1);
//                    printf("weight_%d%d - %f \n", (j+1), (k+1), layer_bm[i].node[j].weight[k]);
                }
            } else
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param->num_nodes_array[i-1]));
                if (!layer_bm[i].node[j].weight)
                {
                    printf("create_layer: malloc: node %d: weight: %s\n", j, strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param->num_nodes_array[i-1]; k++)
                {
//                    printf("i - %d \t j - %d \t k - %d \t", i, j, k);
                    layer_bm[i].node[j].weight[k] = layer_bm[i-1].node[k].weight[j];
//                    printf("weight_%d%d - %f \n", (j+1), (k+1), layer_bm[i].node[j].weight[k]);
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return layer_bm;
} /* end of create_layer */


struct network *
create_network (struct parameters * param)
{
    struct network * network_bm = malloc(sizeof (*network_bm));
    if (!network_bm) {
        printf("create_network: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    network_bm->num_layers = param->num_layers;
    network_bm->layer = create_layer(param);
    return network_bm;
} /* end of create_network */


struct matrix *
allocate_dataset (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
}


int
main(int argc, char *argv[])
{


    char * parameters_file = "in_parameters.dat";
    char * dataset_file = "dataset/three_node_test.csv";
    struct parameters * param = parameters_input(parameters_file, dataset_file);

    print_parameters(param);

    struct network * network_bm = create_network(param);
    print_network_status(network_bm);

    struct matrix* dataset = allocate_dataset(param->dataset_file, param->dataset_rows, param->dataset_cols);
    printf("\n\nInput dataset display\n\n");
    matrix_print(dataset);


    return 0;
}
