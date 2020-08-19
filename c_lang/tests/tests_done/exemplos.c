#include <stdio.h>
#include <string.h>
#include <errno.h>


struct layer *
create_layer(struct parameters params)
{
    // Mudaria para isso,e nao usaria o nome da variavel com o mesmo nome da struct
    struct layer * layer_new = malloc( sizeof (*layer_new) * param.num_layers);
    if (!layer_new)
    {
        perror("create_layer: malloc: layer: %s\n", strerror(errno));
        exit(2); // Sempre saia com erro 2 no caso de falha por malloc
    }

    for (int i = 0; i < param.num_layers; i++)
    {
        printf("i - %d \n", i);
        layer_new[i].num_nodes = param.num_nodes_array[i];

        layer_new[i].node = malloc(sizeof (struct layer) * (layer_new[i].num_nodes));
        if (!layer_new[i].node)
        {
            perror("create_layer: malloc: node: %s\n", strerror(errno));
            exit(2); // Sempre saia com erro 2 no caso de falha por malloc
        }

        for (int j = 0; j < layer_new[i].num_nodes; j++) 
        {
            printf("i - %d \t j - %d \n", i, j);
            layer_new[i].node[j].activation = 0;
            layer_new[i].node[j].bias = 0;
            layer_new[i].node[j].h_in = 0;

            if (i == 0) {
                layer_new[i].node[j].weight = malloc(sizeof (double) * (layer_new[i+1].num_nodes));
                if (!layer_new[i].node[j].weight)
                {
                    perror("create_layer: malloc: node %d: weight:  %s\n", j, strerror(errno));
                    exit(2); // Sempre saia com erro 2 no caso de falha por malloc
                }

                for (int k = 0; k < layer_new[i+1].num_nodes; k++) {
                    printf("i - %d \t j - %d \t k - %d \n", i, j, k);
                    layer_new[i].node[j].weight[k] = 2.2; //(double) (10 * (j+1)) + (k+1);
                    printf("%f \n", layer_new[i].node[j].weight[k]);
                }
            } else {
                layer_new[i].node[j].weight = malloc(sizeof (double) * (layer_new[i-1].num_nodes));
                if (!layer_new[i].node[j].weight)
                {
                    perror("create_layer: malloc: node %d: weight:  %s\n", j, strerror(errno));
                    exit(2); // Sempre saia com erro 2 no caso de falha por malloc
                }

                for (int k = 0; k < layer_new[i-1].num_nodes; k++) {
                    printf("i - %d \t j - %d \t k - %d \n", i, j, k);
                    layer_new[i].node[j].weight[k] = 42; //layer_new[i-1].node[k].weight[j];
                    printf("%f \n", layer_new[i].node[j].weight[k]);
                }
            }
        }
        printf("\n");
    }


    return layer_new;
}
