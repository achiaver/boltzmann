// __MATRIX_H__
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "random_gen.h"

struct matrix {
    size_t rows;
    size_t cols;
    double * elem;
};

struct matrix * matrix_create         (size_t, size_t);
void            matrix_destroy        (struct matrix*);
void            matrix_zero           (struct matrix*);
void            matrix_randomize      (struct matrix*);
void            matrix_print          (struct matrix*, int);
void            matrix_print_line     (struct matrix*, int);
void            matrix_shape          (struct matrix*);
void            matrix_set            (struct matrix*, size_t, size_t, double);
double          matrix_get            (struct matrix*, size_t, size_t);
struct matrix * matrix_read_data      (char *, size_t, size_t);
struct matrix * matrix_sum            (struct matrix*, struct matrix*);
struct matrix * matrix_product        (struct matrix*, struct matrix*);
struct matrix * matrix_product_scalar (struct matrix*, double);
void            matrix_copy           (struct matrix*, struct matrix*);

#endif /* __MATRIX_H__ */

