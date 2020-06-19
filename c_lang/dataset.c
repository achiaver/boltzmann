/* =============================================================================
 * =============================================================================
 * dataset.c - 
 * contains functions related to dataset management.
 * 
 * dataset is stored into struct matrix:
 *  -> rows = set of inputs, each row is an occurrence of the observed phenomenon
 *  -> cols = set of dimensions, each col is one input to a single 
 *            node (cols = number of visible nodes)
 *
 * struct matrix definitions can be found on matrix.h
 * 
 *
 * =============================================================================
 * =============================================================================
 */

#include "dataset.h"

// dataset_example is used only for james, as a ready to use example;
// this example is based on James McCaffrey tutorial about RBMs.
// dataset_example converts the fixed obvserved data in example[12][6] to a 
// struct matrix.

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


// dataset_allocate read in the dataset from a txt (.csv) file and store the data
// on a struct matrix.
// rows and cols information comes from parameter variable.

struct matrix *
dataset_allocate (char * filename, size_t rows, size_t cols)
{
    struct matrix * data = matrix_read_data(filename, rows, cols);
    return data;
} /* end dataset_allocate */


void
dataset_destroy (struct matrix * m)
{
    matrix_destroy(m);
    printf("----> \t DATASET DELETED\n\n");
} /* end dataset_destroy */


// dataset_dump display the whole dataset matrix in the prompt.

void
dataset_dump(struct matrix * m)
{
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
