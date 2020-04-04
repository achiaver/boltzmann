#include <stdio.h>
#include <stdlib.h>


struct matrix {
    size_t dim1;
    size_t dim2;
    size_t dim3;
    double * element;
};


int main (int args, char * argv[]) {
    size_t dim1 = 1;
    size_t dim2 = 3;
    size_t dim3 = 2;

    double valor[] = {1, 2, 3, 4, 5, 6};

    struct matrix * matriz = malloc (sizeof  (*matriz));
    if(!matriz)
    {
        printf("Deu pau!\n");
        exit(2);
    }

    matriz->dim1 = dim1;
    matriz->dim2 = dim2;
    matriz->dim3 = dim3;

    for (size_t i = 0; i < matriz->dim2; i++) 
    {
        matriz->element[i] = malloc (sizeof (*double) * (matriz->dim3)); // ISSO AQUI NAO COMPILA, PQ?
        for (size_t j = 0; j < matriz->dim3; j++)
        {
            matriz->element[i][j] = valor[i + j];
        }
    }



    return 0;
}
