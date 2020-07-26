#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "parameters.h"


struct parameters *
parameters_create ()
{
    struct parameters * param = malloc (sizeof (*param));
    if (!param)
    {
        fprintf(stderr, "parameters_create: malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    param->dataset_file = "";
    param->dataset_rows = 0;
    param->dataset_cols = 0;
    param->temp_start = 0.;
    param->temp_end = 0.;
    param->epsilonw = 0.;
    param->epsilonvb = 0.;
    param->epsilonhb = 0.;
    param->maxepochs = 0;
    param->num_layers = 0;
    param->num_visible = 0;
    param->num_hidden = 0;

    return param;
} /* end parameters_create */


void
parameters_delete (struct parameters * param)
{
    free(param);
    printf("----> Parameters deleted! \n");
} /* end parameters_delete */


struct parameters *
parameters_input (char * parameters_file, char * data_file)
{
    FILE * fp = fopen(parameters_file, "r");

    if (!fp)
    {
        fprintf(stderr, "parameters_input: fopen: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    struct parameters * param = parameters_create();

    param->dataset_file = data_file;
    fscanf(fp, "%*s%zu%*s \
                %*s%zu%*s \
                %*s%lf%*s \
                %*s%lf%*s \
                %*s%lf%*s \
                %*s%d%*s \
                %*s%zu%*s \
                %*s%zu%*s \
                %*s%zu%*s",
                &param->dataset_rows,
                &param->dataset_cols,
                &param->epsilonw,
                &param->epsilonvb,
                &param->epsilonhb,
                &param->maxepochs,
                &param->num_layers,
                &param->num_visible,
                &param->num_hidden);

//    param->nodes_per_layer =  malloc (sizeof (param->nodes_per_layer) * (param->num_layers));
//    if (!param->nodes_per_layer)
//    {
//        fprintf(stderr, "parameters_input: malloc: %s %d", __FILE__, __LINE__);
//        exit(2);
//    }
//
//    for (int i = 0; i < param->num_layers; i++)
//    {
//        fscanf(fp, "%*s%zu%*s", &param->nodes_per_layer[i]);
//    }
    fclose(fp);

    return param;
} /* end parameters_input */


void
parameters_print (struct parameters * param)
{
    printf("---- IN PARAMETERS ----\n");
    printf("\n \
    \t Number of examples in the dataset: \t %zu \n \
    \t Dimension size of the dataset: \t %zu \n \
    \t Learning rate of the weights: \t %lf \n \
    \t Learning rate of the visible units biases: \t %lf \n \
    \t Learning rate of the hidden units biases: \t %lf \n \
    \t Maximum number of epochs: \t %d \n \
    \t Number of layers of the network: \t %zu \n \
    \t Number of visible nodes: \t %zu \n \
    \t Number of hidden nodes: \t %zu \n",
    param->dataset_rows,
    param->dataset_cols,
    param->epsilonw,
    param->epsilonvb,
    param->epsilonhb,
    param->maxepochs,
    param->num_layers,
    param->num_visible,
    param->num_hidden);

//    for (int i = 0; i < param->num_layers; i++)
//    {
//        printf("\t Number of nodes at layer %d: \t %zu \n", i, param->nodes_per_layer[i]);
//    }

    printf("\n");
} /* end parameters_print */
