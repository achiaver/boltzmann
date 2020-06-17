#include "dataset.h"

struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end dataset_allocate */


struct matrix *
dataset_example (double example[12][6], size_t rows, size_t cols)
{
    struct matrix * data = matrix_create(rows, cols);

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            matrix_set(data, row, col, example[row][col]);
        }
    }

    return data;
} /* end dataset_example */


void
dataset_destroy (struct matrix * m)
{
    printf("---- \t DELETING DATASET\n");
    matrix_destroy(m);
    printf("----> \t DATASET DELETED\n\n");
} /* end dataset_destroy */


void
dataset_dump(struct matrix * m)
{
//    int cols = (int) sizeof(data[0]) / sizeof(data[0][0]);
    for (int row = 0; row < m->rows; row++)
    {
        printf("[%2d] - ", row);
        for (int col = 0; col < m->cols; col++)
        {
            printf("%2.0f ", matrix_get(m, row, col));
        }
        printf("\n");
    }
    printf("\n");
} /* end dataset_print */



