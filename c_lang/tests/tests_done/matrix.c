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
} /* end of matrix_create */


void
matrix_destroy(struct matrix *m)
{
    if(m) {
        free(m->elem);
        free(m);
    }
} /* end of matrix_destroy */


void
matrix_zero(struct matrix *m)
{
    for (size_t e = 0; e < (m->rows * m->cols); e++)
        m->elem[e] = 0.;
} /* end of matrix_zero */


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
} /* end of matrix_print */


void
matrix_print_line(struct matrix *m)
{
    for (size_t row = 0; row < m->rows; row++)
        for( size_t col = 0; col < m->cols; col++)
            printf("%f, \t", matrix_get(m, row, col));
    printf("\n");
} /* end of matrix_print_line */


void
matrix_shape(struct matrix *m)
{
    printf("[%zu, %zu]\n", m->rows, m->cols);
} /* end of matrix_shape */


void
matrix_set(struct matrix *m, size_t row, size_t col, double value)
{
    m->elem[(row*m->cols) + col] = value;
} /* enf of matrix_set */


double
matrix_get(struct matrix *m, size_t row, size_t col)
{
    return m->elem[(row*m->cols) + col];
} /* end of matrix_get */


struct matrix *
matrix_read_data (char * filename, size_t rows, size_t cols)
{
    struct matrix * m_data = matrix_create(rows, cols);

    char buffer[1024];
    int row = 0, col = 0;

    FILE * fp = fopen(filename, "r");

    if (!fp)
    {
        fprintf(stderr, "fopen: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    while (fgets(buffer, sizeof(buffer), fp) && (row < rows))
    {
        char *line = strdup(buffer);
        const char * data;

        for (data = strtok(buffer, ","), col = 0; data && *data && (col < cols); col++, data = strtok(NULL, ",\n"))
        {
            matrix_set(m_data, row, col, atof(data));
        }
        row++;
        free(line);
    }
    fclose(fp);
    return m_data;
} /* end of matrix_read_data */


struct matrix *
matrix_sum (struct matrix * A, struct matrix * B)
{
    if ((A->rows != B->rows) || (A->cols != B->cols))
    {
        fprintf(stderr, "matrix_sum: matrices dimension are not compatible: %s %d", __FILE__, __LINE__);
        exit(1);
    }

    struct matrix * m_sum = matrix_create(A->rows, A->cols);

    for (int row = 0; row < A->rows; row++)
    {
        for (int col = 0; col < A->cols; col++)
        {
            matrix_set(m_sum, row, col, matrix_get(A, row, col) + matrix_get(B, row, col));
        }
    }

    return m_sum;
} /* end of matrix_sum */


struct matrix *
matrix_product (struct matrix * A, struct matrix * B)
{
    if ((A->cols != B->rows))
    {
        fprintf(stderr, "matrix_product: matrices dimension are not compatible: %s %d", __FILE__, __LINE__);
        exit(1);
    }

    struct matrix * m_product = matrix_create(A->rows, B->cols);
    matrix_zero(m_product);

    double elem1 = 0., elem2 = 0.;

    for (int row = 0; row < A->rows; row++)
    {
        for (int col1 = 0; col1 < B->cols; col1++)
        {
            for (int col2 = 0; col2 < A->cols; col2++)
            {
                elem1 = matrix_get(m_product, row, col1);
                elem2 = matrix_get(A, row, col2) * matrix_get(B, col2, col1);
                matrix_set(m_product, row, col1, elem1 + elem2);
            }
        }
    }

    return m_product;
} /* end of matrix_product */


struct matrix *
matrix_product_scalar (struct matrix * A, double scalar)
{
    struct matrix * m_prod_scalar = matrix_create(A->rows, A->cols);

    for (int row = 0; row < A->rows; row++)
    {
        for (int col = 0; col < A->cols; col++)
        {
            matrix_set(m_prod_scalar, row, col, matrix_get(A, row, col) * scalar);
        }
    }

    return m_prod_scalar;
} /* end matrix_product_scalar */


int main (int args, char * argv[]) {


    struct matrix * data1 = matrix_read_data("../dataset/example_1.csv", 3, 3);
    struct matrix * data2 = matrix_read_data("../dataset/example_2.csv", 3, 5);

    matrix_shape(data1);
    matrix_print(data1);
    printf("\n\n");

    matrix_shape(data2);
    matrix_print(data2);
    printf("\n\n");


    struct matrix * C = matrix_sum(data1, data1);
    printf("Sum of data matrix\n");
    matrix_print(C);

    printf("\n");
    struct matrix * D = matrix_product(data1, data1);
    printf("Product of data matrix with itself\n");
    matrix_print(D);

    printf("\n");
    struct matrix * E = matrix_product_scalar(data1, 2.);
    printf("Product of data matrix by scalar\n");
    matrix_print(E);

    printf("\n");
    struct matrix * F = matrix_product(data1, data2);
    printf("Product of data1 * data2\n");
    matrix_print(F);

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

    matrix_destroy(data1);
    matrix_destroy(data2);
    matrix_destroy(C);
    matrix_destroy(D);
    matrix_destroy(E);
    matrix_destroy(F);

    return 0;
}
