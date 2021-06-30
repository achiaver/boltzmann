#ifndef __DATASET_H__
#define __DATASET_H__

#include <stdio.h>
#include "matrix.h"

matrix *dataset_allocate            (char   *filename,
                                     size_t  rows,
                                     size_t  cols);
matrix *dataset_example             (size_t  rows, 
                                     size_t  cols,
                                     double  example[rows][cols]);
void            dataset_destroy     (matrix *m);

void            dataset_dump        (matrix *m);

#endif /* __DATASET_H__ */
