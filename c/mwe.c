#include <stdio.h>

struct parameters {
    size_t N;
    size_t M;
};

int main(int argc, char *argv[])
{

    /* Boltzmann Machine parameters */
    struct parameters params = {
        .N = 3,
        .M = 6
    };

    /* Sample of initial values for training */
    double sample_input[][3] = {
        { 1.f, 1.f, 0.f },
        { 1.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 1.f },
    };

    size_t data_count = sizeof sample_input / sizeof *sample_input;
    double *tmp_input;
    tmp_input = &sample_input;

    for(size_t j=0; j < data_count; j++) {
        for (size_t i=0; i< params.N; i++) {
            printf("%1.0f ", *tmp_input++);
        }
        printf("\n");
    }

    return 0;
}
