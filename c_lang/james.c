#include "main.h"

// 0 to disable it
#define DEBUG 1


int
main(int argc, char *argv[])
{
    initialize_seed();

    printf("\nBegin Restricted Boltzmann Machine demo\n");
    printf("Films: Alien, Inception, Spy, Eurotrip, Gladiator, Spartacus\n\n");

    double example[12][6] = {{ 1, 1, 0, 0, 0, 0 },   // A
                             { 0, 0, 1, 1, 0, 0 },   // B
                             { 0, 0, 0, 0, 1, 1 },   // C
                             { 1, 1, 0, 0, 0, 1 },   // noisy A
                             { 0, 0, 1, 1, 0, 0 },   // B
                             { 0, 0, 0, 0, 1, 1 },   // C
                             { 1, 0, 0, 0, 0, 0 },   // weak A
                             { 0, 0, 1, 0, 0, 0 },   // weak B
                             { 0, 0, 0, 0, 1, 0 },   // weak C
                             { 1, 1, 0, 1, 0, 0 },   // noisy A
                             { 1, 0, 1, 1, 0, 0 },   // noisy B
                             { 0, 0, 1, 0, 1, 1 }};  // noisy C


    struct matrix * dataset = dataset_example(example, 12, 6);

    printf("Film like-dislike data is: \n");
    dataset_dump(dataset);

    struct parameters * param = parameters_create();
    param->num_layers = 2;
    param->num_visible = 6;
    param->num_hidden = 3;

    printf("Creating a RBM ... \n");
    printf("Setting number of visible nodes = %zu \n", param->num_visible);
    printf("Setting number of hidden nodes = %zu \n\n", param->num_hidden);

    struct network * net = network_create(param);

    param->dataset_rows = 12;
    param->dataset_cols = 6;
    param->epsilonw = 0.01;
    param->epsilonvb = 0.01;
    param->epsilonhb = 0.01;
    param->maxepochs = 1000;

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

    struct layer * hidden = hidden_from_visible(net, visible);
    printf(" -> ");
    layer_print(hidden, 1);


    struct layer * visible_computed = visible_from_hidden(net, hidden);
    printf("hidden = ");
    layer_print(hidden, 0);
    printf(" -> ");
    layer_print(visible_computed, 1);
    printf("\n");


    layer_delete(visible, 0);
    layer_delete(hidden, 0);
    layer_delete(visible_computed, 0);
    network_delete(net);
    parameters_delete(param);

    printf("\n\n- - - END OF RBM TEST BASED ON JAMES - - -\n\n");

    return 0;
}


