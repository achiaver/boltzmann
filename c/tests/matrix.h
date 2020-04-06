// MATRIX_H
#ifndef __MATRIX_H__
#define __MATRIX_H__

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

