#include "main.h"

// 0 to disable it
#define DEBUG 1
#define PERCENT_ERROR_ENERGY 1.E-10

//layer *
//visible_to_hidden_j (layer *visible, network *net, double T)
//{
//    layer *hidden = layer_create(net->hidden.num_nodes);
//    for (int h = 0 ; h < hidden->num_nodes; h++)
//    {
//        double sum = 0.;
//        for (int v = 0; v < visible->num_nodes; v++)
//        {
//            sum += node_get_activation(visible->nodes, v) * matrix_get(net->weights, v, h);
//        }
//        sum += node_get_bias(net->hidden.nodes, h);
//        node_set_nprob(hidden->nodes, h, func_sigmoid(sum, T));
//        if (node_get_nprob(hidden->nodes, h) >= random_0to1())
//        {
//            node_set_activation(hidden->nodes, h, 1.);
//        } else
//        {
//            node_set_activation(hidden->nodes, h, 0.);
//        }
//    }
//    return hidden;
//}
//
//layer *
//hidden_to_visible_j (layer *hidden, network *net, double T)
//{
//    layer *visible = layer_create(net->visible.num_nodes);
//    for (int v = 0; v < visible->num_nodes; v++)
//    {
//        double sum = 0.;
//        for (int h = 0; h < hidden->num_nodes; h++)
//        {
//           sum += node_get_activation(hidden->nodes, h) * matrix_get(net->weights, v, h);
//        }
//        sum += node_get_bias(net->visible.nodes, v);
//        node_set_nprob(visible->nodes, v, func_sigmoid(sum, T));
//        if (node_get_nprob(visible->nodes, v) >= random_0to1())
//        {
//            node_set_activation(visible->nodes, v, 1.);
//        } else
//        {
//            node_set_activation(visible->nodes, v, 0.);
//        }
//    }
//    return visible;
//} /* end visible_from_hidden */


void
layer_copy_layer_j (layer *lay_1, layer *lay_2)
{
    for (int i = 0; i < lay_1->num_nodes; i++)
    {
        if (node_get_activation(lay_1->nodes, i) == -1.)
        {
            double act = random_activation();
            if (act == 1.)
                node_set_activation(lay_2->nodes, i, 1.);
            else
                node_set_activation(lay_2->nodes, i, 0.);
        } else
        {
            node_set_activation(lay_2->nodes, i, node_get_activation(lay_1->nodes, i));
        }
    }
}


layer *
simulated_annealing_j (network *net, layer *input, parameters *param)
{
    int crystalized = 0; // False
    double infinite_energy = (net->hidden.num_nodes + net->visible.num_nodes) * (net->hidden.num_nodes + net->visible.num_nodes) * 1.0E5;
    double last_mean_energy = 0.;

    layer_copy_layer_j(input, &net->visible);
    layer_print(&net->visible, 0);
    printf("\n");
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
//                    delta_energy = delta_energy + node_get_bias(net->hidden.nodes, h);
                    delta_energy = delta_energy + matrix_get(net->biases, h, 1);
                    
                    node_set_nprob(net->hidden.nodes, h, func_sigmoid(delta_energy, temp_current));
                    if (node_get_nprob(net->hidden.nodes, h) >= random_0to1())
                    {
                        node_set_activation(net->hidden.nodes, h, 1.);
                    } else
                    {
                        node_set_activation(net->hidden.nodes, h, 0.);
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
//                    delta_energy = delta_energy + node_get_bias(net->visible.nodes,v);
                    delta_energy = delta_energy + matrix_get(net->biases, v, 0);

                    node_set_nprob(net->visible.nodes, v, func_sigmoid(delta_energy, temp_current));
                    if (node_get_nprob(net->visible.nodes, v) >= random_0to1())
                    {
                        node_set_activation(net->visible.nodes, v, 1.);
                    } else
                    {
                        node_set_activation(net->visible.nodes, v, 0.);
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

 //       printf("T - %f\n", temp_current);
    }
//    layer_delete(hidden);
    return &net->visible;
} /* end simulated_annealing*/


int
main(int argc, char *argv[])
{
    random_seed(true);

    printf("\nBegin Restricted Boltzmann Machine demo\n");
    printf("Films: Alien, Inception, Spy, Eurotrip, Gladiator, Spartacus\n\n");

    double example[43][6] = {{ 1, 1, 0, 0, 0, 0 },   // A
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
                             { 0, 0, 1, 0, 1, 1 },   // noisy C
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 1, 1, 0, 0, 0, 0 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 0, 0, 1, 1 },
                             { 0, 0, 1, 1, 0, 0 },
                             { 0, 0, 1, 1, 0, 0 },
                             { 0, 0, 1, 1, 0, 0 },
                             { 0, 0, 1, 1, 0, 0 },
                             { 0, 0, 1, 1, 0, 0 },
                             { 1, 0, 0, 0, 0, 0 },
                             { 1, 0, 0, 0, 0, 0 },
                             { 1, 0, 0, 0, 0, 0 },
                             { 1, 0, 1, 1, 0, 0 },
                             { 1, 0, 1, 1, 0, 0 },
                             { 1, 0, 1, 1, 0, 0 },};




    matrix *dataset = dataset_example(43, 6, example);

    printf("Film like-dislike data is: \n");
    dataset_dump(dataset);

    parameters *param = parameters_create();
    if (param)
    {
        param->temp_start = 5.995;
        param->temp_end = 0.001;
        param->temp_step = 0.95;
        param->tries_per_node = 20;
        param->num_layers = 2;
//        param->num_visible = 6;
//        param->num_hidden = 3;
        param->nodes_per_layer[0] = 6;
        param->nodes_per_layer[1] = 3;
        param->dataset_rows = 43;
        param->dataset_cols = 6;
        param->epsilonw = 0.01;
        param->epsilonvb = 0.01;
        param->epsilonhb = 0.01;
        param->maxepochs = 1000;
    }
    printf("Creating a RBM ... \n");
    printf("Setting number of visible nodes = %zu \n", param->nodes_per_layer[0]);
    printf("Setting number of hidden nodes = %zu \n\n", param->nodes_per_layer[1]);

    network *net = network_create(param);


    printf("Training RBM using CD1 algorithm \n");
    printf("Setting learning rate (weights and biases) = %f \n", param->epsilonw);
    printf("Setting maximum amount of epochs = %zu \n\n", param->maxepochs);

    network_training(net, param, dataset);
    printf("Training complete. \n");

    printf("Trained machine's weights and biases are: \n");
    network_dump(net, 0, 1, 1);

    printf("\n- Using the network -\n");
    printf("- NOT Simulated Annealing -\n");

    layer *visible = layer_create(net->visible.num_nodes);
    layer_copy_from_array(visible, dataset, 0);
    printf("visible = ");
    layer_print(visible, 0);

    layer *hidden = hidden_from_visible(net, visible, 1);
    printf(" -> ");
    layer_print(hidden, 1);


    layer *visible_computed = visible_from_hidden(net, hidden, 1);
    printf("hidden = ");
    layer_print(hidden, 0);
    printf(" -> ");
    layer_print(visible_computed, 1);
    printf("\n");

    printf("- SIMULATED ANNEALING TEST -\n");
    // Select a test example, where activation as -1 means that these unit will be randomly initialized.
//    double test_example[1][6] = {{1,-1,0,-1,0,0}};
//    matrix *test_m = dataset_example(1, 6, test_example);
    layer *test_l = layer_create(net->visible.num_nodes);
    layer_copy_from_array(test_l, dataset, 1);
    layer_print(test_l, 0);
    printf("\n");
//    printf("delete test_m\n");
//    matrix_destroy(test_m);

    layer *simu = simulated_annealing_j(net, test_l, param);
    printf("\n");
    layer_print(simu, 0);
    printf("\n");

    printf("\n\n\n");
    printf("delete simu\n");
    layer_delete(simu);

    printf("delete test_l\n");
    layer_delete(test_l);

    printf("delete dataset\n");
    matrix_destroy(dataset);

    printf("delete visible\n");
    layer_delete(visible);

    printf("delete hidden\n");
    layer_delete(hidden);

    printf("delete visible_computed\n");
    layer_delete(visible_computed);

    printf("detele network\n");
    network_delete(net);

    printf("delete paramenters\n");
    parameters_delete(param);

    printf("\n\n- - - END OF RBM TEST BASED ON JAMES - - -\n\n");

    return 0;
}



