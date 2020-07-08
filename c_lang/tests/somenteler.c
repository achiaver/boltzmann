#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NEURONS 13
#define MINIBATCHSIZE 5

typedef uint8_t feio_t;

int main(int argc, char *argv[]) { 

    FILE *dados_fd2 = fopen("dados.bin",  "rb");
    if(!dados_fd2) {
        fprintf(stderr, "Could not open file. Exiting\n");
        return 1;
    }

    size_t nmemb = NEURONS * MINIBATCHSIZE;
    feio_t *minibatchzim = calloc(nmemb, sizeof(feio_t));

    size_t count = 0;
    size_t bytes_lidos;
    while(bytes_lidos = fread(minibatchzim, sizeof(feio_t), nmemb, dados_fd2))
    {
        fprintf(stdout, "\n\nBATCH numero %zu\n", count++);
        size_t linhas = bytes_lidos / (sizeof(feio_t)*NEURONS);
        imprime_padroes(minibatchzim, linhas, NEURONS);
    }

    free(minibatchzim);
    fclose(dados_fd2);

    return 0;
}

void imprime_padroes(feio_t *padrao, size_t padroes, size_t neuronios)
{
    for(size_t i = 0; i < padroes; i++) {
        for(size_t j =0; j < neuronios; j++) {
                fprintf(stdout, "%u", *padrao++);
        }
        fprintf(stdout,"\n");
    }
}
