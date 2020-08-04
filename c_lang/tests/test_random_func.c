#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_func.h"


struct random_sample
{
    double bounded;
    double activation;
    double zero_to_one;
    double in_range;
};


int
main (int argc, char * argv[])
{
    if (argc < 5)
    {
        fprintf(stdout, "Usage: %s FILENAME ITERATIONS PRINT_OPTION READ_OPTION\n", argv[0]);
        fprintf(stdout, "Example: %s random_numbers.bin 30000000 1 1\n", argv[0]);
        return 1;
    }

    char filename[256];
    strncpy(filename, argv[1], 255);
    size_t iterations = atoi(argv[2]);
    size_t print_option = atoi(argv[3]);
    size_t read_option = atoi(argv[4]);

    printf("filename - %s\n", filename);
    printf("iterations - %zu\n", iterations);

    struct random_sample sample_out;
    struct random_sample sample_in;
    
    FILE *file_out = fopen(filename, "ab+");
    if (!file_out)
    {
        fprintf(stderr, "Could not open file to write\n");
        exit(1);
    }

    random_seed(true);
    
    for (int i = 0; i < iterations; i++)
    {
        sample_out.bounded = random_bounded(5);
        sample_out.activation = random_activation();
        sample_out.zero_to_one = random_0to1();
        sample_out.in_range = random_in_range(3, 11);
        if (print_option == 1)
        {
            fprintf(stdout, "\t%f \t%f \t%f \t%f\n", sample_out.bounded, sample_out.activation, sample_out.zero_to_one, sample_out.in_range);
        }
        fwrite(&sample_out, sizeof(struct random_sample), 1, file_out);
    }
    fprintf(stdout, "Done with file\n");

    fclose(file_out);
    
    FILE *file_in = fopen(filename, "rb");
    if(!file_in)
    {
        fprintf(stderr, "CANNOT READ file %s\n", filename);
        exit(1);
    }

//    for (int i = 0; i < iterations; i++)
//    {
//        fread(&sample_in, sizeof(struct random_sample), 1, file_in);
//        fprintf(stdout, "\t%f \t%f \t%f \t%f\n", sample_in.bounded, sample_in.activation, sample_in.zero_to_one, sample_in.in_range);
//    }
    if (read_option == 1)
    {
        while(fread(&sample_in, sizeof(struct random_sample), 1, file_in))
        {
            fprintf(stdout, "\t%f \t%f \t%f \t%f\n", sample_in.bounded, sample_in.activation, sample_in.zero_to_one, sample_in.in_range);
        }
        fclose(file_in);
    }
    return 0;
}
