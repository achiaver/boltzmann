#include <stdio.h>
#include <stdlib.h>


// MATRIX_H
#ifndef MATRIX_H
#define MATRIX_H

struct matrix {
    size_t rows;
    size_t cols;
    double * elem;
};

struct matrix * matrix_create       (size_t, size_t);
void            matrix_destroy      (struct matrix*);
void            matrix_zero         (struct matrix*);
void            matrix_print        (struct matrix*);
void            matrix_print_line   (struct matrix*);
void            matrix_shape        (struct matrix*);
void            matrix_set          (struct matrix*, size_t, size_t, double);
double          matrix_get          (struct matrix*, size_t, size_t);
#endif /* MATRIX_H */

// END MATRIX_H


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
}

void
matrix_destroy(struct matrix *m)
{
    if(m) {
        free(m->elem);
        free(m);
    }
}

void
matrix_zero(struct matrix *m)
{
    for (size_t e = 0; e < (m->rows * m->cols); e++)
        m->elem[e] = 0.;
}

void
matrix_print(struct matrix *m)
{
    for (size_t row = 0; row < m->rows; row++) 
    {
        for( size_t col = 0; col < m->cols; col++) 
        {
            printf("%f, ", matrix_get(m, row, col));
        }
        printf("\n");
    }
}


void
matrix_print_line(struct matrix *m)
{
    for (size_t row = 0; row < m->rows; row++) 
        for( size_t col = 0; col < m->cols; col++) 
            printf("%f, ", matrix_get(m, row, col));
    printf("\n");
}

void
matrix_shape(struct matrix *m)
{
    printf("[%zu, %zu]\n", m->rows, m->cols);
}


void
matrix_set(struct matrix *m, size_t row, size_t col, double value)
{
    m->elem[(row*m->cols) + col] = value;
}

double
matrix_get(struct matrix *m, size_t row, size_t col)
{
    return m->elem[(row*m->cols) + col];
}


int main (int args, char * argv[]) {

    double res;
    size_t cols = 4, rows = 3;

    struct matrix *my_matrix = matrix_create(cols, rows);
    matrix_zero(my_matrix);

    printf("\n");
    matrix_shape(my_matrix);
    printf("\n");
    matrix_print_line(my_matrix);
    printf("\n");
    matrix_print(my_matrix);
    printf("\n");

    matrix_set(my_matrix, 1, 2, 42.42);
    res = matrix_get(my_matrix, 1, 2);

    printf("\n");
    printf("%f\n", res);
    printf("\n");
    matrix_print_line(my_matrix);
    printf("\n");
    matrix_print(my_matrix);
    printf("\n");

    matrix_destroy(my_matrix);

    return 0;
}
