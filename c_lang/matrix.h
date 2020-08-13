// __MATRIX_H__
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_func.h"

typedef struct MATRIX {
    size_t  rows;
    size_t  cols;
    double *elem;
} matrix;

matrix *matrix_create         (size_t,
                               size_t);
void    matrix_destroy        (matrix *);
void    matrix_zero           (matrix *);
void    matrix_randomize      (matrix *);
void    matrix_print          (matrix *, 
                               int);
void    matrix_print_line     (matrix *,
                               int);
void    matrix_shape          (matrix *);
void    matrix_set            (matrix *,
                               size_t,
                               size_t,
                               double);
double  matrix_get            (matrix *,
                               size_t,
                               size_t);
matrix *matrix_read_data      (char *,
                               size_t,
                               size_t);
matrix *matrix_sum            (matrix *,
                               matrix *);
matrix *matrix_product        (matrix *,
                               matrix *);
matrix *matrix_product_scalar (matrix *,
                               double);
void    matrix_copy           (matrix *,
                               matrix *);

#endif /* __MATRIX_H__ */

