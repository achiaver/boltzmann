#include "main.h"

// 0 to disable it
#define DEBUG 1

void
layer_copy_layer (struct layer * lay_1, struct layer * lay_2)
{
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        if (node_get_activation(lay_1->nodes, i) == -1.)
        {
            node_set_activation(lay_2->nodes, i, (double)rand() / (double)RAND_MAX);
        } else
        {
            node_set_activation(lay_2->nodes, i, node_get_activation(lay_1->nodes, i));
        }
    }
}


struct layer *
simulated_annealing (struct network * net, struct layer * input)
{
    struct layer * lay = layer_create(input->num_nodes);
    struct layer * hidden = layer_create(net->hidden.num_nodes);
    layer_copy_layer(input, lay);
    layer_print(lay, 0);
    printf("\n");

    double T = 100.0;
    while (T >= 1)
    {
        hidden = hidden_from_visible(net, lay, T);
        layer_print(hidden, 0);
        printf("\n");
        lay = visible_from_hidden(net, hidden, T); 
        layer_print(lay, 0);
        printf("\n");

        T = T * 0.95;
    }
    layer_delete(hidden, 0);
    return lay;
} /* end simulated_annealing*/


int
main(int argc, char *argv[])
{
    initialize_seed();

    printf("\nBegin Restricted Boltzmann Machine demo\n");
    printf("Films: Alien, Inception, Spy, Eurotrip, Gladiator, Spartacus\n\n");

    double example[2][9] = {{ 1, 0, 0, 1, 0, 1, 0, 1, 0 },   // MAKE
                            { 0, 0, 1, 0, 1, 1, 0, 1, 0}};  // Nonsense


    struct matrix * dataset = dataset_example(example, 2, 9);

    printf("Film like-dislike data is: \n");
    dataset_dump(dataset);

    struct parameters * param = parameters_create();
    param->num_layers = 2;
    param->num_visible = 9;
    param->num_hidden = 4;

    printf("Creating a RBM ... \n");
    printf("Setting number of visible nodes = %zu \n", param->num_visible);
    printf("Setting number of hidden nodes = %zu \n\n", param->num_hidden);

    struct network * net = network_create(param);

    param->dataset_rows = 2;
    param->dataset_cols = 9;
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

    printf("\nUsing trained machine - NOT Simulated Annealing -\n");

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

    printf("- SIMULATED ANNEALING TEST -\n");
    // Select a test example, where activation as -1 means that these unit will be randomly initialized.
    double test_example[1][6] = {{1,-1,0,-1,0,0}};
    struct matrix * test_m = dataset_example(test_example, 1, 6);
    struct layer * test_l = layer_create(net->visible.num_nodes);
    layer_copy_from_array(test_l, test_m, 0);
    layer_print(test_l, 0);
    printf("\n");
    printf("delete test_m\n");
    matrix_destroy(test_m);

    struct layer * simu = simulated_annealing(net, test_l);
    printf("\n");
    layer_print(simu, 0);
    printf("\n");

    printf("\n\n\n");
    printf("delete simu\n");
    layer_delete(simu, 1);

    printf("delete test_l\n");
    layer_delete(test_l, 0);

    printf("delete dataset\n");
    matrix_destroy(dataset);

    printf("delete visible\n");
    layer_delete(visible, 0);

    printf("delete hidden\n");
    layer_delete(hidden, 0);

    printf("delete visible_computed\n");
    layer_delete(visible_computed, 0);

    printf("detele network\n");
    network_delete(net);

    printf("delete paramenters\n");
    parameters_delete(param);

    printf("\n\n- - - END OF RBM TEST BASED ON JAMES - - -\n\n");

    return 0;
}



