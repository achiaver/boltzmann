#ifndef __DATASET_H__
#define __DATASET_H__

#include <stdio.h>
#include "matrix.h"

struct matrix * dataset_allocate    (char *             filename,
                                     size_t             rows,
                                     size_t             cols);
struct matrix * dataset_example     (double             example[12][6], 
                                     size_t             rows, 
                                     size_t             cols);
void            dataset_destroy     (struct matrix *    m);

void            dataset_dump        (struct matrix *    m);

#endif /* __DATASET_H__ */
