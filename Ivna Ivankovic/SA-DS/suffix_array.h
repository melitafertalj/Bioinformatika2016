#ifndef SUFFIX_ARRAY_H

#define SUFFIX_ARRAY_H

#include <stdlib.h>

typedef int suffix_t;

typedef struct
{
    suffix_t *elements;
    int size;
} suffix_array_t;

// Creates suffix array of specified size.
suffix_array_t *create_suffix_array(const int size);

// Clears the resources of the suffix array.
void free_suffix_array(suffix_array_t *suffix_array);

// Returns the element at specified index of the suffix array.
suffix_t *suffix_at(suffix_array_t *suffix_array, const int index);

#endif // SUFFIX_ARRAY_H
