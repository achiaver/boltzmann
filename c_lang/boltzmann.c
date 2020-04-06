#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "boltzmann.h"
#include "stats_functions.h"

struct layer * create_layer (struct parameters param) 
{
    struct layer * layer_bm = malloc(sizeof (*layer_bm) * (param.num_layers));
    if (!layer_bm) 
    {
        printf("create_layer: malloc: layer: %s \n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < param.num_layers; i++) 
    {
        printf("i - %d \n", i);
        layer_bm[i].num_nodes = param.num_nodes_array[i];
        layer_bm[i].node = malloc(sizeof (struct node) * (layer_bm[i].num_nodes));
        if (!layer_bm[i].node) 
        {
            printf("create_layer: malloc: node: %s\n", strerror(errno));
            exit(2);
        }

        for (int j = 0; j < layer_bm[i].num_nodes; j++) 
        {
            printf("i - %d \t j - %d \n", i, j);
            layer_bm[i].node[j].activation = 0;
            layer_bm[i].node[j].bias = 0;
            layer_bm[i].node[j].h_in = 0;

            if (i == 0) 
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param.num_nodes_array[i+1]));
                if (!layer_bm[i].node[j].weight)
                {
                    printf("create_layer: malloc: node %d: weight: %s \n", j,  strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param.num_nodes_array[i+1]; k++) 
                {
                    printf("i - %d \t j - %d \t k - %d \t", i, j, k);
                    layer_bm[i].node[j].weight[k] = (double) (10 * (j+1)) + (k+1);
                    printf("weight_%d%d - %f \n", (j+1), (k+1), layer_bm[i].node[j].weight[k]);
                }
            } else 
            {
                layer_bm[i].node[j].weight = malloc(sizeof (double *) * (param.num_nodes_array[i-1]));
                if (!layer_bm[i].node[j].weight)
                {
                    printf("create_layer: malloc: node %d: weight: %s\n", j, strerror(errno));
                    exit(2);
                }

                for (int k = 0; k < param.num_nodes_array[i-1]; k++) 
                {
                    printf("i - %d \t j - %d \t k - %d \t", i, j, k);
                    layer_bm[i].node[j].weight[k] = layer_bm[i-1].node[k].weight[j];
                    printf("weight_%d%d - %f \n", (j+1), (k+1), layer_bm[i].node[j].weight[k]);
                }
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return layer_bm;
}

struct network * create_network (struct parameters param) 
{
    struct network * network_bm = malloc(sizeof (*network_bm));
    if (!network_bm) {
        printf("create_network: malloc: network: %s \n", strerror(errno));
        exit(2);
    }
    network_bm->num_layers = param.num_layers;
    network_bm->layer = create_layer(param);
    return network_bm;
}


void read_csv(struct parameters param, double * dataset) 
{
    char buffer[1024];
    char * record, * line;
    FILE * fp = fopen("dataset/dataset.csv", "r");

    if (!fp)
    {
        printf("read_csv : %s \n", strerror(errno));
        exit(2);
    }

    for (int i = 0; i < param.dataset_rows; i++)
    {
        line = fgets(buffer, sizeof(buffer), fp);
        record = strtok(line, ",");
        
        for (int j = 0; i < param.dataset_cols; j++)
        {
        
        }

    }
    while (!(line = fgets(buffer, sizeof(buffer), fp))) 
    {
        record = strtok(line, ",");
        while (!record) 
        {
            datavar[rows][cols] = atof(record);
            cols = cols + 1;
        }
        rows = rows + 1;
        cols_aux = cols;
        cols = 0;
    }


}

double * allocate_dataset (struct parameters param)
{
    double * dataset = malloc(sizeof (*dataset) * (param.dataset_rows));
    if (!dataset)
    {
        printf("allocate_dataset : dataset %s \n", strerror(errno));
        exit(2);
    }
    for (int i = 0; i < param.dataset_rows; i++) 
    {
        dataset[i] = malloc(sizeof (dataset) * (param.dataset_cols));
        if (!dataset[i])
        {
            printf("allocate_dataset : dataset[%d] %s \n", i, strerror(errno));
            exit(2);
        }
    }

    read_csv(param, dataset);



    return dataset;
}


int main(int argc, char *argv[])
{
    /* Boltzmann Machine parameters */
//    struct parameters param = {
//        .N = 0,
//        .M = 0,
//        .epsilonw = 0.0,
//        .epsilonvb = 0.0,
//        .epsilonhb = 0.0,
//        .maxepochs = 0
//    };

    struct parameters parameters_bm;
    input_parameters(&parameters_bm);
    print_parameters(parameters_bm);

    struct network * network_bm = create_network(parameters_bm);
    print_network_status(*network_bm);

    double * dataset = allocate_dataset(parameters_bm);



    /* Sample of initial values for training */
    double sample_input[][3] = {
        { 1.f, 1.f, 0.f },
        { 1.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 1.f },
    };

//    size_t data_count = sizeof sample_input / sizeof *sample_input;
//    double *tmp_input;
//    tmp_input = &sample_input;
//
//
//    for(size_t j=0; j < data_count; j++) {
//        for (size_t i=0; i< param.N; i++) {
//            printf("%1.0f ", *tmp_input++);
//        }
//        printf("\n");
//    }
//
    return 0;
}
