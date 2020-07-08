#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t feio_t;

void do_something(feio_t *data, size_t rows, size_t columns);

int main(int argc, char *argv[]) { 
    if(argc < 4)
    {
        fprintf(stdout, "Usage:   %s FILENAME NUM_NEURONS SIZE_OF_MINIBATCH\n", argv[0]);
        fprintf(stdout, "Example: %s dados.bin 13 5\n", argv[0]);
        return 1;
    }

    char filename[256];
    strncpy(filename, argv[1], 255);
    size_t neurons = atoi(argv[2]);
    size_t minibatch_size = atoi(argv[3]);
    size_t nmemb = neurons * minibatch_size;

    FILE *fd = fopen(filename,  "rb");
    if(!fd) {
        fprintf(stderr, "Could not open file %s. Exiting\n", filename);
        return 1;
    }

    feio_t *data = calloc(nmemb, sizeof(feio_t));

    size_t batchnum = 0;
    size_t bytes_read;
    size_t patterns_read;
    while((bytes_read = fread(data, sizeof(feio_t), nmemb, fd)))
    {
        patterns_read = bytes_read / (sizeof(feio_t)*neurons);

        // Aqui voce chama o que quer fazer com o seu "minibatch"
        fprintf(stdout, "\n\nStarting batch %zu\n", batchnum++);
        do_something(data, patterns_read, neurons);
    }

    free(data);
    fclose(fd);

    return 0;
}

void do_something(feio_t *data, size_t rows, size_t columns)
{
    for(size_t i = 0; i < rows; i++) 
    {
        for(size_t j =0; j < columns; j++) 
        {
//            fprintf(stdout, "%u", *data++);
            fprintf(stdout, "%u", data[i * columns + j]);
        }
        fprintf(stdout,"\n");
    }
}
