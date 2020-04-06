#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"


struct matrix *
matrix_create( size_t rows, size_t cols)
{
    struct matrix * m = malloc (sizeof (*m));
    if(!m)
    {
        fprintf(stderr, "malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    m->rows = rows;
    m->cols = cols;

    m->elem = malloc((m->rows * m->cols) * sizeof (m->elem));
    if(!m->elem)
    {
        fprintf(stderr, "malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    return m;
} /* matrix_create */


void
matrix_destroy(struct matrix *m)
{
    if(m) {
        free(m->elem);
        free(m);
    }
} /* matrix_destroy */


void
matrix_zero(struct matrix *m)
{
    for (size_t e = 0; e < (m->rows * m->cols); e++)
        m->elem[e] = 0.;
} /* matrix_zero */


void
matrix_print(struct matrix *m)
{
    for (size_t row = 0; row < m->rows; row++) 
    {
        for( size_t col = 0; col < m->cols; col++) 
        {
            printf("%f, \t", matrix_get(m, row, col));
        }
        printf("\n");
    }
} /* matrix_print */


void
matrix_print_line(struct matrix *m)
{
    for (size_t row = 0; row < m->rows; row++) 
        for( size_t col = 0; col < m->cols; col++) 
            printf("%f, \t", matrix_get(m, row, col));
    printf("\n");
} /* matrix_print_line */


void
matrix_shape(struct matrix *m)
{
    printf("[%zu, %zu]\n", m->rows, m->cols);
} /* matrix_shape */


void
matrix_set(struct matrix *m, size_t row, size_t col, double value)
{
    m->elem[(row*m->cols) + col] = value;
} /* matrix_set */


double
matrix_get(struct matrix *m, size_t row, size_t col)
{
    return m->elem[(row*m->cols) + col];
} /* matrix_get */


struct matrix *
matrix_read_data (char * filename, size_t rows, size_t cols)
{
    struct matrix * m = matrix_create(rows, cols);

    char buffer[1024];
    int row = 0;

    FILE * fp = fopen(filename, "r");

    if (!fp)
    {
        fprintf(stderr, "fopen: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    while (fgets(buffer, sizeof(buffer), fp) && (row < rows))
    {
        char *line = strdup(buffer);
        int col;
        const char * data;

        for (data = strtok(buffer, ","); data && *data; col++, data = strtok(NULL, ",\n"))
        {
            matrix_set(m, row, col, atof(data));
        }
        free(line);
    }

    fclose(fp);
    return m;
} /* matrix_read_data */


int main (int args, char * argv[]) {


    struct matrix * data = matrix_read_data("../dataset/example.csv", 4, 5);
    matrix_shape(data);

    matrix_print(data);


//    double res;
//    size_t cols = 3, rows = 4;

//    struct matrix *my_matrix = matrix_create(rows, cols);
//    matrix_zero(my_matrix);

//    printf("\n");
//    matrix_shape(my_matrix);
//    printf("\n");
//    matrix_print_line(my_matrix);
//    printf("\n");
//    matrix_print(my_matrix);
//    printf("\n");
//
//    matrix_set(my_matrix, 1, 2, 42.42);
//    matrix_set(my_matrix, 1, 3, 42);
//    res = matrix_get(my_matrix, 1, 2);
//
//    printf("\n");
//    printf("%f\n", res);
//    printf("\n");
//    matrix_print_line(my_matrix);
//    printf("\n");
//    matrix_print(my_matrix);
//    printf("\n");
//    matrix_destroy(my_matrix);

    matrix_destroy(data);


    return 0;
}
