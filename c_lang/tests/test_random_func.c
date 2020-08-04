#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_func.h"

int
main (int argc, char * argv[])
{
    if (argc < 3)
    {
        fprintf(stdout, "Usage: %s FILENAME_OUT NUMBER_OF_ITERATIONS\n", argv[0]);
        fprintf(stdout, "Example: %s random_numbers.dat 30000000\n", argv[0]);
        return 1;
    }

    char filename[256];
    strncpy(filename, argv[1], 255);
    size_t iterations = atoi(argv[2]);

    printf("iterations - %zu\n", iterations);
    
    double * random_numbers = malloc(sizeof (double) * (iterations * 4));
    if (!random_numbers) 
    {
        fprintf(stderr, "malloc random_test: %s %d\n", __FILE__, __LINE__);
        exit(2);
    }

    random_seed(true);
    
    for (int i = 0; i < iterations; i++)
    {
        random_numbers[(i * 4) + 0] = random_bounded(5);
        random_numbers[(i * 4) + 1] = random_activation();
        random_numbers[(i * 4) + 2] = random_0to1();
        random_numbers[(i * 4) + 3] = random_in_range(1., 10.);
    }
    
    printf("Criamos o dados\n");
    for (int i = 0; i < iterations; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("\t%f", random_numbers[(i * 4) + j]);
        }
        printf("\n");
    }

    size_t data_size = sizeof(double) * (iterations * 4);//sizeof(random_numbers);
    size_t elem_size = sizeof(random_numbers[0]);
    size_t num_elem = data_size/elem_size;

    printf("\t%zu \t%zu \t%zu\n", data_size, elem_size, num_elem);

    FILE *dados = fopen(filename, "wb");
    if (dados)
    {
        fwrite(random_numbers, elem_size, num_elem, dados);
        fclose(dados);
        dados = NULL;
    } else
    {
        fprintf(stderr, "Could not open file to write\n");
        dados = NULL;
    }

    free(random_numbers);
    return 0;
}
