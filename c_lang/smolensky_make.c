#include "main.h"

// 0 to disable it
#define DEBUG 1
#define PERCENT_ERROR_ENERGY 1.E-10

typedef double welem_t;

struct layer *
visible_to_hidden_s (struct layer * visible, struct network * net, double T)
{
    struct layer * hidden = layer_create(net->hidden.num_nodes);
    for (int h = 0 ; h < hidden->num_nodes; h++)
    {
        double sum = 0.;
        for (int v = 0; v < visible->num_nodes; v++)
        {
            sum += node_get_activation(visible->nodes, v) * matrix_get(net->weights, v, h);
        }
        sum -= 0.7;
        node_set_nprob(hidden->nodes, h, func_sigmoid(sum, T));
        if (node_get_nprob(hidden->nodes, h) >= random_0to1())
        {
            node_set_activation(hidden->nodes, h, 1.);
        } else
        {
            node_set_activation(hidden->nodes, h, 0.);
        }
    }
    return hidden;
}

struct layer *
hidden_to_visible_s (struct layer * hidden, struct network * net, double T)
{
    struct layer * visible = layer_create(net->visible.num_nodes);
    for (int v = 0; v < visible->num_nodes; v++)
    {
        double sum = 0.;
        for (int h = 0; h < hidden->num_nodes; h++)
        {
           sum += node_get_activation(hidden->nodes, h) * matrix_get(net->weights, v, h);
        }
        sum *= 2.;
        node_set_nprob(visible->nodes, v, func_sigmoid(sum, T));
        if (node_get_nprob(visible->nodes, v) >= random_0to1())
        {
            node_set_activation(visible->nodes, v, 1.);
        } else
        {
            node_set_activation(visible->nodes, v, -1.);
        }
    }
    return visible;
} /* end visible_from_hidden */


void
layer_copy_layer (struct layer * lay_1, struct layer * lay_2)
{
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        if (node_get_activation(lay_1->nodes, i) == 0.)
        {
            double act = random_activation();
            if (act == 1.)
                node_set_activation(lay_2->nodes, i, 1.);
            else
                node_set_activation(lay_2->nodes, i, -1.);
        } else
        {
            node_set_activation(lay_2->nodes, i, node_get_activation(lay_1->nodes, i));
        }
    }
}


struct layer *
simulated_annealing (struct network * net, struct layer * input, struct parameters * param)
{
    int crystalized = 0; // False
    double infinite_energy = (net->hidden.num_nodes + net->visible.num_nodes) * (net->hidden.num_nodes + net->visible.num_nodes) * 1.0E5;
    double last_mean_energy = 0.;

//    struct layer * input_aux = layer_create(input->num_nodes);
//    struct layer * hidden = layer_create(net->hidden.num_nodes);
//    layer_copy_layer(input, input_aux);
    layer_copy_layer(input, &net->visible);
//    layer_print(input_aux, 0);
//    printf("\n");
    double n_iterations_mean_energy = param->tries_per_node * net->num_nodes_total;
    double convergence_error = 0.;

    double temp_current = param->temp_start;
//    double temp_end = 0.01;
    while ((!crystalized) && (temp_current >= param->temp_end))
    {
        int iteration = 0;
        int thermalized = 0; // False
        double last_temp_mean_energy = infinite_energy;
        double mean_energy = 0.;
        
        while ((!thermalized) && (iteration < net->num_nodes_total))
        {
            for (int i_mean = 0; i_mean < param->tries_per_node; i_mean++)
            {
                
                // Update hidden units
                for (int h = 0; h < net->hidden.num_nodes; h++)
                {
                    double delta_energy = 0.;
                    for (int v = 0; v < net->visible.num_nodes; v++)
                    {
                        delta_energy = delta_energy + node_get_activation(net->visible.nodes, v) * matrix_get(net->weights, v, h);
                    }
                    delta_energy = delta_energy + node_get_bias(net->hidden.nodes, h);
                    
                    node_set_nprob(net->hidden.nodes, h, func_sigmoid(delta_energy, temp_current));
                    if (node_get_nprob(net->hidden.nodes, h) >= random_0to1())
                    {
                        node_set_activation(net->hidden.nodes, h, 1.);
                    } else
                    {
                        node_set_activation(net->hidden.nodes, h, -1.);
                    }
                    mean_energy = mean_energy + func_energy(net->weights, &net->visible, &net->hidden);    
                }
                // Update visible units
                for (int v = 0; v < net->visible.num_nodes; v++)
                {
                    double delta_energy = 0.;
                    for (int h = 0; h < net->hidden.num_nodes; h++)
                    {
                        delta_energy = delta_energy + node_get_activation(net->hidden.nodes, h) * matrix_get(net->weights, v, h);
                    }
                    delta_energy = delta_energy + node_get_bias(net->visible.nodes,v);
                    
                    node_set_nprob(net->visible.nodes, v, func_sigmoid(delta_energy, temp_current));
                    if (node_get_nprob(net->visible.nodes, v) >= random_0to1())
                    {
                        node_set_activation(net->visible.nodes, v, 1.);
                    } else
                    {
                        node_set_activation(net->visible.nodes, v, -1.);
                    }
                    mean_energy = mean_energy + func_energy(net->weights, &net->visible, &net->hidden);
                }
            }
            mean_energy = mean_energy / n_iterations_mean_energy;
            convergence_error = (mean_energy - last_mean_energy) / mean_energy;
            convergence_error = fabs(convergence_error);
            if (convergence_error < PERCENT_ERROR_ENERGY)
            {
                thermalized = 1;
            } else
            {
                last_mean_energy = mean_energy;
            }
            iteration += 1;
        }

        if (mean_energy >= last_temp_mean_energy)
        {
            crystalized = 0;
        } else
        {
            last_temp_mean_energy = mean_energy;
            temp_current = param->temp_update * temp_current;
        }

        printf("T - %f\n", temp_current);
    }
//    layer_delete(hidden, 0);
    return &net->visible;
} /* end simulated_annealing*/


int
main(int argc, char *argv[])
{
    random_seed(true);

//    if (argc < 4)
//    {
//        fprintf(stdout, "Usage:   %s FILENAME COLS ROWS\n", argv[0]);
//        fprintf(stdout, "Example: %s dados.bin 4 9\n", argv[0]);
//        return 1;
//    }
//
//    char filename_weights[256];
//    strncpy(filename_weights, argv[1], 255);
//    size_t weight_cols = atoi(argv[2]);
//    size_t weight_rows = atoi(argv[3]);
//
//    printf("weight columns size - %zu\n", weight_cols);
//    printf("weight rows size - %zu\n", weight_rows);

    double example[1][9] = {{ 1,-1,-1, 1,-1, 1,-1, 1,-1}};    // MAKE

    double weights[9][4] = {{ 0.2,   0.,   0.,  0.},
                            {-0.2,   0.,   0.,  0.},
                            {-0.2,   0.,   0.,  0.},
                            { 0.2, 0.25,   0.,  0.},
                            {-0.2,-0.25,   0.,  0.},
                            {  0., 0.25, 0.25,-0.25},
                            {  0.,-0.25,-0.25, 0.25},
                            {  0.,   0., 0.25,-0.25},
                            {  0.,   0.,-0.25,-0.25}};

    struct matrix * dataset = dataset_example(1, 9, example);
    dataset_dump(dataset);

    struct parameters * param = parameters_create();
    param->dataset_file = "no_file";
    param->temp_start = 5.995;
    param->temp_end = 0.001;
    param->temp_update = 0.95;
    param->tries_per_node = 20;
    param->num_layers = 2;
    param->num_visible = 9;
    param->num_hidden = 4;

    struct network * net = network_create(param);
    struct matrix * weights_m = dataset_example(9, 4, weights);
    printf("\n");
    matrix_copy(weights_m, net->weights);
    matrix_destroy(weights_m);
    printf("\n");
    network_dump(net, 0, 1, 0);

    printf("- SIMULATED ANNEALING -\n");
 // Select a test example, where activation as 0 means that these unit will be randomly initialized.
    double test_example[1][9] = {{ 1,-1,-1, 0,-1, 1, 0, 1,-1}};
    struct matrix * test_m = dataset_example(1, 9, test_example);
    struct layer * test_l = layer_create(net->visible.num_nodes);
    layer_copy_from_array(test_l, test_m, 0);
    layer_print(test_l, 0);
    printf("\n");
    printf("delete test_m\n");
    matrix_destroy(test_m);

    struct layer * simu = simulated_annealing(net, test_l, param);
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

    printf("detele network\n");
    network_delete(net);

    printf("delete paramenters\n");
    parameters_delete(param);

    return 0;
}



