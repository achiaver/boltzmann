#include <stdio.h>
#include "parameters.h"

int
main (int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout, "Usage: %s PARAMETERS_FILENAME\n", argv[0]);
        fprintf(stdout, "Example: %s in_parameters.dat\n", argv[0]);
        return 1;
    }

//    char *parameters_filename = "in_smolensky_make.dat";
    fprintf(stdout, "parameters filename : %s\n", argv[1]);

//    parameters *param = parameters_create();
    parameters *param = parameters_input(argv[1]);
    parameters_print(param);

    parameters_delete(param);

    return 0;
}
