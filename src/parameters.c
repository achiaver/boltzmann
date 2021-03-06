#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "parameters.h"


parameters *
parameters_create ()
{
    parameters *param = malloc (sizeof (*param));
    if (!param)
    {
        fprintf(stderr, "parameters_create: malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    // Initializing variables of parameters to empty or zero
    param->dataset_rows = 0;
    param->dataset_cols = 0;
    param->weights_rows = 0;
    param->weights_cols = 0;
    param->biases_rows = 0;
    param->biases_cols = 0;
    param->temp_start = 0.;
    param->temp_end = 0.;
    param->temp_step = 0.;
    param->epsilonw = 0.;
    param->epsilonvb = 0.;
    param->epsilonhb = 0.;
    param->maxepochs = 0;
    param->num_layers = 0;

    // nodes_per_layers is an array, each entry is the equivalent to the number
    // of nodes in that layer, considering 0 the fist layer.
    param->nodes_per_layer = malloc (sizeof (param->nodes_per_layer) * param->num_layers);
    if (!param->nodes_per_layer)
    {
        fprintf(stderr, "parameters nodes_per_layer malloc FAIL: %s %d\n", __FILE__, __LINE__);
        exit(2);
    }

    for (int i = 0; i < param->num_layers; i++)
    {
        param->nodes_per_layer[i] = 0;
    }

//    param->num_visible = 0;
//    param->num_hidden = 0;

    return param;
} /* end parameters_create */


void
parameters_delete (parameters *param)
{
    free(param);
    printf("----> Parameters deleted! \n");
} /* end parameters_delete */


parameters *
parameters_input (char *parameters_file, char *dataset_file)
{
    FILE *fp = fopen(parameters_file, "r");

    if (!fp)
    {
        fprintf(stderr, "parameters_input: fopen: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    parameters *param = parameters_create();
   
    fscanf(fp, "%*s%s  \
                %*s%zu \
                %*s%zu \
                %*s%s  \
                %*s%zu \
                %*s%zu \
                %*s%s  \
                %*s%zu \
                %*s%zu \
                %*s%zu \
                %*s%lf \
                %*s%lf \
                %*s%lf \
                %*s%lf \
                %*s%lf \
                %*s%lf \
                %*s%zu \
                %*s%zu ",
                param->dataset_filename,
                &param->dataset_rows,
                &param->dataset_cols,
                param->weights_filename,
                &param->weights_rows,
                &param->weights_cols,
                param->biases_filename,
                &param->biases_rows,
                &param->biases_cols,
                &param->tries_per_node,
                &param->temp_start,
                &param->temp_end,
                &param->temp_step,
                &param->epsilonw,
                &param->epsilonvb,
                &param->epsilonhb,
                &param->maxepochs,
                &param->num_layers); 

    for (int i = 0; i < param->num_layers; i++)
    {
        fscanf(fp, "%*s%zu", &param->nodes_per_layer[i]);
    }
    fclose(fp);

    return param;
} /* end parameters_input */


void
parameters_print (parameters *param)
{
    fprintf(stdout, "---- INPUT PARAMETERS ----\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Dataset filename : %s\nDataset number of examples : %zu\nDataset number of dimensions : %zu\n",
                     param->dataset_filename,
                     param->dataset_rows,
                     param->dataset_cols);
    fprintf(stdout, "Weights filename : %s\nWeights number of rows : %zu\nWeights number of columns : %zu\n",
                     param->weights_filename,
                     param->weights_rows,
                     param->weights_cols);
    fprintf(stdout, "Biases filename : %s\nBiases number of rows : %zu\nBiases number of columns : %zu\n",
                     param->biases_filename,
                     param->biases_rows,
                     param->biases_cols);
    fprintf(stdout, "Number of tries per node : %zu\nInitial temperature : %lf\nFinal temperature : %lf\nStep of temperature update : %lf\n",
                     param->tries_per_node,
                     param->temp_start,
                     param->temp_end,
                     param->temp_step);
    fprintf(stdout, "Learning rate for weights : %lf\nLearning rate for visible biases : %lf\nLearning rate for hidden biases : %lf\nMaximum number of epochs : %zu\n",
                     param->epsilonw,
                     param->epsilonvb,
                     param->epsilonhb,
                     param->maxepochs);
    fprintf(stdout, "Number of layers : %zu\n",
                     param->num_layers);
    for (int i = 0; i < param->num_layers; i++)
    {
        fprintf(stdout, "Number of nodes at layer %2d: %zu\n", i, param->nodes_per_layer[i]);
    }

    printf("\n");
} /* end parameters_print */
