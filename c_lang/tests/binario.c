#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NEURONS 13

#define MINIBATCHSIZE 5

typedef uint8_t feio_t;

void imprime_padroes(feio_t *padrao, size_t padroes, size_t neuronios);

int main(int argc, char *argv[]) { 

    feio_t patterns[][NEURONS] = {
        { 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 2, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},       // 2
        { 4, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 5, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0},
        { 6, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0},
        { 7, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
        { 8, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
        { 9, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
        { 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0},
        { 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0},
        { 2, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0},
        { 3, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
        { 4, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 5, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0},
        { 6, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0},
        { 7, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
        { 8, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
        { 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},       // 18
        { 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 2, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 3, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 4, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 5, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 6, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
        { 7, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    };

    size_t data_size = sizeof(patterns);
    size_t elem_size = sizeof(patterns[0][0]);
    size_t num_elem  = data_size/elem_size;

    fprintf(stdout, "DataSz: %zu, ElemSz: %zu, #Elem: %zu, #rows: %zu, #cols: %zu\n",
            data_size, elem_size, num_elem, num_elem/NEURONS, NEURONS);


    feio_t *ptr = &patterns[0][0];
    imprime_padroes(ptr, 20, NEURONS);

    // ESCREVER A MATRIZ para o arquivo em formato binario;
    FILE *dados_fd = fopen("dados.bin", "wb");
    if(dados_fd) {
        fwrite(patterns, elem_size, num_elem, dados_fd);
        fclose(dados_fd);
        dados_fd = NULL;
    } else {
        fprintf(stderr, "Could not open file to write\n");
        dados_fd = NULL;
    }

    // Agora vamos ler o arquivo e gravar em outro lugar ignorando
    // completamente todas as informações acima.
    // Usando outro nome so para nao ficar achando que estou mutretando
    FILE *dados_fd2 = fopen("dados.bin",  "rb");
    if(!dados_fd2) {
        fprintf(stderr, "Could not open file. Exiting\n");
        return 1;
    }

    // o numero de neuronios em cada padrao.
    size_t nmemb = NEURONS * MINIBATCHSIZE;
    
    // Reservamos a memoria para o numero de membros do tipo feio_t.
    feio_t *minibatchzim = calloc(nmemb, sizeof(feio_t));

    size_t count = 0;

    // Abrimos o arquivo agora vamos ler por batches...
    size_t bytes_lidos;
    while(bytes_lidos = fread(minibatchzim, sizeof(feio_t), nmemb, dados_fd2))
    {
        // GUARDANDO os primeiros dados em minibatchzim
        fprintf(stdout, "\n\nBATCH numero %zu\n", count++);

        // Verifica se nao leu menos linhas, neste caso, imprime menos padroes.
        size_t linhas = bytes_lidos / (sizeof(feio_t)*NEURONS);

        // Imprime as linhas lidas do minibatchzim
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
