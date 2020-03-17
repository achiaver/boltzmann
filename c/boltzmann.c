#include "boltzmann.h"
#include "inout.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{
    /* Boltzmann Machine parameters */
    struct parameters param = {
        .N = 0,
        .M = 0,
        .epsilonw = 0.0,
        .epsilonvb = 0.0,
        .epsilonhb = 0.0,
        .maxepochs = 0
    };

    input_parameters(&param);
    print_parameters(param);



    /* Sample of initial values for training */
    double sample_input[][3] = {
        { 1.f, 1.f, 0.f },
        { 1.f, 1.f, 0.f },
        { 1.f, 0.f, 0.f },
        { 1.f, 0.f, 1.f },
    };

//    size_t data_count = sizeof sample_input / sizeof *sample_input;
//    double *tmp_input;
//    tmp_input = &sample_input;
//
//
//    for(size_t j=0; j < data_count; j++) {
//        for (size_t i=0; i< param.N; i++) {
//            printf("%1.0f ", *tmp_input++);
//        }
//        printf("\n");
//    }
//
    return 0;
}
