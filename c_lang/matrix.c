#include "matrix.h"


matrix *
matrix_create(size_t rows, size_t cols)
{
    matrix *m = malloc (sizeof (*m));
    if(!m)
    {
        fprintf(stderr, "malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    m->rows = rows;
    m->cols = cols;

//    m->elem = malloc((m->rows * m->cols) * sizeof (m->elem));
    m->elem = malloc((m->rows * m->cols) * sizeof (double));
    if(!m->elem)
    {
        fprintf(stderr, "malloc: %s %d", __FILE__, __LINE__);
        exit(2);
    }

    return m;
} /* end of matrix_create */


void
matrix_destroy(matrix *m)
{
    if(m) {
        free(m->elem);
        free(m);
        m = NULL;
    }
} /* end of matrix_destroy */


void
matrix_zero(matrix *m)
{
    for (size_t e = 0; e < (m->rows * m->cols); e++)
        m->elem[e] = 0.;
} /* end of matrix_zero */

void
matrix_randomize(matrix *m)
{
    for (size_t e = 0; e < (m->rows * m->cols); e++)
        m->elem[e] = random_0to1();
} /* end of matrix_randomize */


void
matrix_print(matrix *m, int option)
{
    for (size_t row = 0; row < m->rows; row++)
    {
        for( size_t col = 0; col < m->cols; col++)
        {
            if (option == 0) // print no decimals
            {
                printf("%.0f ", matrix_get(m, row, col));
            } else           // print with decimals
            {
                printf("%f \t", matrix_get(m, row, col));
            }
        }
        printf("\n");
    }
} /* end of matrix_print */


void
matrix_print_line(matrix *m, int option)
{
    for (size_t row = 0; row < m->rows; row++)
    {
        for( size_t col = 0; col < m->cols; col++)
        {
            if (option == 0) // print no decimals
            {
                printf("%.0f, \t", matrix_get(m, row, col));
            } else           // print with decimals
                printf("%f, \t", matrix_get(m, row, col));
        }
    printf("\n");
    }
} /* end of matrix_print_line */


void
matrix_shape(matrix *m)
{
    printf("[%zu, %zu]\n", m->rows, m->cols);
} /* end of matrix_shape */


void
matrix_set(matrix *m, size_t row, size_t col, double value)
{
    m->elem[(row*m->cols) + col] = value;
} /* enf of matrix_set */


double
matrix_get(matrix *m, size_t row, size_t col)
{
    return m->elem[(row*m->cols) + col];
} /* end of matrix_get */


matrix *
matrix_read_data(char *filename, size_t rows, size_t cols)
{
    matrix *m_data = matrix_create(rows, cols);

    char buffer[1024];
    int row = 0, col = 0;

    FILE *fp = fopen(filename, "r");

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


matrix *
matrix_sum(matrix *A, matrix *B)
{
    if ((A->rows != B->rows) || (A->cols != B->cols))
    {
        fprintf(stderr, "matrix_sum: matrices dimension are not compatible: %s %d", __FILE__, __LINE__);
        exit(1);
    }

    matrix * m_sum = matrix_create(A->rows, A->cols);

    for (int row = 0; row < A->rows; row++)
    {
        for (int col = 0; col < A->cols; col++)
        {
            matrix_set(m_sum, row, col, matrix_get(A, row, col) + matrix_get(B, row, col));
        }
    }

    return m_sum;
} /* end of matrix_sum */


matrix *
matrix_product(matrix *A, matrix *B)
{
    if ((A->cols != B->rows))
    {
        fprintf(stderr, "matrix_product: matrices dimension are not compatible: %s %d", __FILE__, __LINE__);
        exit(1);
    }

    matrix *m_product = matrix_create(A->rows, B->cols);
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


matrix *
matrix_product_scalar(matrix *A, double scalar)
{
    matrix * m_prod_scalar = matrix_create(A->rows, A->cols);

    for (int row = 0; row < A->rows; row++)
    {
        for (int col = 0; col < A->cols; col++)
        {
            matrix_set(m_prod_scalar, row, col, matrix_get(A, row, col) * scalar);
        }
    }

    return m_prod_scalar;
} /* end matrix_product_scalar */


void
matrix_copy (matrix *m1, matrix *m2)
{
    if (m1->rows != m2->rows || m1->cols != m2->cols)
    {
        exit(1);
    }
    for (size_t i = 0; i < m1->rows; i++)
    {
        for (size_t j = 0; j < m1->cols; j++)
        {
            matrix_set(m2, i, j, matrix_get(m1, i, j));
        }
    }
} /* end matrix_copy */
