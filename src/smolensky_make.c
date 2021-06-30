#include "main.h"

// 0 to disable it
#define DEBUG 1
#define PERCENT_ERROR_ENERGY 1.E-10

layer *
visible_to_hidden_s (layer *visible, network *net, double T)
{
    layer *hidden = layer_create(net->hidden.num_nodes);
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
} /* end visible_to_hidden_s */


layer *
hidden_to_visible_s (layer *hidden, network *net, double T)
{
    layer *visible = layer_create(net->visible.num_nodes);
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
} /* end hidden_to_visible_s */


void
layer_copy_layer (layer *lay_1, layer *lay_2)
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


layer *
simulated_annealing (network *net, layer *input, parameters *param)
{
    int crystalized = 0; // False
    double infinite_energy = (net->hidden.num_nodes + net->visible.num_nodes) * (net->hidden.num_nodes + net->visible.num_nodes) * 1.0E5;
    double last_mean_energy = 0.;

    layer_copy_layer(input, &net->visible);
    
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
                    mean_energy = mean_energy + func_energy(net->weights, net->biases, &net->visible, &net->hidden);    
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
                    mean_energy = mean_energy + func_energy(net->weights, net->biases, &net->visible, &net->hidden);
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
            temp_current = param->temp_step * temp_current;
        }

        printf("T - %f\n", temp_current);
    }
//    layer_delete(hidden);
    return &net->visible;
} /* end simulated_annealing*/


int
main(int argc, char *argv[])
{
    // true => seed is not fixed!
    // false => seed is fixed!
    random_seed(false);

//    double example[1][9] = {{ 1,-1,-1, 1,-1, 1,-1, 1,-1}};    // MAKE

    double weights[9][4] = {{ 0.2,   0.,   0.,  0.},
                            {-0.2,   0.,   0.,  0.},
                            {-0.2,   0.,   0.,  0.},
                            { 0.2, 0.25,   0.,  0.},
                            {-0.2,-0.25,   0.,  0.},
                            {  0., 0.25, 0.25,-0.25},
                            {  0.,-0.25,-0.25, 0.25},
                            {  0.,   0., 0.25,-0.25},
                            {  0.,   0.,-0.25,-0.25}};

//    matrix *dataset = dataset_example(1, 9, example);
//    dataset_dump(dataset);

    parameters *param = parameters_input("in_smolensky_make.dat", "");
    parameters_print(param);

//    network *net = network_create(param);
//    matrix *weights_m = dataset_example(9, 4, weights);
//    printf("\n");
//    matrix_copy(weights_m, net->weights);
//    matrix_destroy(weights_m);
//    printf("\n");
//    network_dump(net, 0, 1, 0);
//
//    printf("- SIMULATED ANNEALING -\n");
// // Select a test example, where activation as 0 means that these unit will be randomly initialized.
//    double test_example[1][9] = {{ 1,-1,-1, 0,-1, 1, 0, 1,-1}};
//
////  COLOCAR COMENTARIOS, dataset_example -> array_to_matrix
//    matrix *test_m = dataset_example(1, 9, test_example);
//    layer *test_l = layer_create(net->visible.num_nodes);
//    layer_copy_from_array(test_l, test_m, 0);
//    layer_print(test_l, 0);
//    printf("\n");
//    printf("delete test_m\n");
//    matrix_destroy(test_m);
//
//    layer *simu = simulated_annealing(net, test_l, param);
//    printf("\n");
//    layer_print(simu, 0);
//    printf("\n");
//
//    printf("\n\n\n");
//    printf("delete simu\n");
//    layer_delete(simu);
//
//    printf("delete test_l\n");
//    layer_delete(test_l);
//
////    printf("delete dataset\n");
////    matrix_destroy(dataset);
//
//    printf("detele network\n");
//    network_delete(net);

    printf("delete paramenters\n");
    parameters_delete(param);

    return 0;
}



