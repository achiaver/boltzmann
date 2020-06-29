#include "main.h"

// 0 to disable it
#define DEBUG 1

int
main(int argc, char *argv[])
{
    initialize_seed();

    char * parameters_file = "in_parameters.dat";
    char * dataset_file = "dataset/training_dataset_2.csv";
    struct parameters * param = parameters_input(parameters_file, dataset_file);
    parameters_print(param);

    struct matrix * dataset = dataset_allocate(param->dataset_file, param->dataset_rows, param->dataset_cols);

#if DEBUG
    printf("\n\nInput dataset display\n\n");
    matrix_print(dataset, 0);
#endif

    struct network * net = network_create(param);

#if DEBUG
    printf("\n\nNetwork dump display\n\n");
    network_dump(net, 0, 1, 1);
#endif

    printf("Training RBM using CD1 algorithm \n");
    printf("Setting learning rate (weights and biases) = %f \n", param->epsilonw);
    printf("Setting maximum amount of epochs = %d \n\n", param->maxepochs);

    network_training(net, param, dataset);
    printf("Training complete. \n");

    printf("Trained machine's weights and biases are: \n");
    network_dump(net, 0, 1, 1);

    printf("\nUsing trained machine... \n");

    struct layer * visible = layer_create(net->visible.num_nodes);
    layer_copy_from_array(visible, dataset, 0);
    printf("visible = ");
    layer_print(visible, 0);

    struct layer * hidden = hidden_from_visible(net, visible, 1);
    printf(" -> ");
    layer_print(hidden, 1);


    struct layer * visible_computed = visible_from_hidden(net, hidden, 1);
    printf("hidden = ");
    layer_print(hidden, 0);
    printf(" -> ");
    layer_print(visible_computed, 1);
    printf("\n");


    layer_delete(visible, 0);
    layer_delete(hidden, 0);
    layer_delete(visible_computed, 0);
    network_delete(net);
    dataset_destroy(dataset);
    parameters_delete(param);
    return 0;
} /* end main */

