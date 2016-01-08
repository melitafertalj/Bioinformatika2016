#ifndef SUFFIX_ARRAY_H

#define SUFFIX_ARRAY_H

#include <stdlib.h>

typedef struct
{
    char *suffix;
    size_t index;
} suffix_t;

typedef struct
{
    suffix_t *elements;
    size_t size;
} suffix_array_t;

// Creates suffix array of specified size.
suffix_array_t *create_suffix_array(const size_t size);

// Clears the resources of the suffix array.
void free_suffix_array(suffix_array_t *suffix_array);

// Returns the element at specified index of the suffix array.
suffix_t *at_index(suffix_array_t *suffix_array, const size_t index);

#endif // SUFFIX_ARRAY_H
