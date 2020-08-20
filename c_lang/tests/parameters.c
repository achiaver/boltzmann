#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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
    param->dataset_filename = "";
    param->dataset_rows = 0;
    param->dataset_cols = 0;
    param->weights_filename = "";
    param->weights_rows = 0;
    param->weights_cols = 0;
    param->biases_filename = "";
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
parameters_input (char *parameters_file)
{
    FILE *fp = fopen(parameters_file, "r");

    if (!fp)
    {
        fprintf(stderr, "parameters_input: fopen: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    parameters *param = parameters_create();

    fscanf(fp, "%*s %s %*[^\n]  \
                %*s %zu %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %s %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %s %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %lf %*[^\n] \
                %*s %zu %*[^\n] \
                %*s %zu %*[^\n] ",
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
        fscanf(fp, "%*s%zu%*s", &param->nodes_per_layer[i]);
    }
    fclose(fp);

    return param;
} /* end parameters_input */


void
parameters_print (parameters *param)
{
    fprintf(stdout, "---- INPUT PARAMETERS ----\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Dataset filename \t: %s\nDataset number of examples \t: %zu\nDataset number of dimensions \t: %zu\n",
                     param->dataset_filename,
                     param->dataset_rows,
                     param->dataset_cols);
    fprintf(stdout, "\t Weights filename \t: %s\n \
                     \t Weights number of rows \t: %zu\n \
                     \t Weights number of columns \t: %zu\n",
                     param->weights_filename,
                     param->weights_rows,
                     param->weights_cols);
    fprintf(stdout, "\t Biases filename \t: %s\n \
                     \t Biases number of rows \t: %zu\n \
                     \t Biases number of columns \t: %zu\n",
                     param->biases_filename,
                     param->biases_rows,
                     param->biases_cols);
    fprintf(stdout, "\t Number of tries per node \t: %zu\n \
                     \t Initial temperature \t: %lf\n \
                     \t Final temperature \t: %lf\n \
                     \t Step of temperature update \t: %lf\n",
                     param->tries_per_node,
                     param->temp_start,
                     param->temp_end,
                     param->temp_step);
    fprintf(stdout, "\t Learning rate for weights \t: %lf\n \
                     \t Learning rate for visible biases \t: %lf\n \
                     \t Learning rate for hidden biases \t: %lf\n \
                     \t Maximum number of epochs \t: %zu\n",
                     param->epsilonw,
                     param->epsilonvb,
                     param->epsilonhb,
                     param->maxepochs);
    fprintf(stdout, "\t Number of layers \t: %zu\n",
                     param->num_layers);
    for (int i = 0; i < param->num_layers; i++)
    {
        fprintf(stdout, "\t Number of nodes at layer %d: %zu\n", i, param->nodes_per_layer[i]);
    }

    printf("\n");
} /* end parameters_print */
