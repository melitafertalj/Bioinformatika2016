#include <string.h>

#include "suffix_array.h"

suffix_array_t *create_suffix_array(const int size)
{
    suffix_array_t *sa = (suffix_array_t *) malloc(sizeof(suffix_array_t));

    if (sa == NULL)  return NULL;

    sa->elements = (suffix_t *) calloc(size, sizeof(suffix_t));
    if (sa->elements == NULL) return NULL;

    memset(sa->elements, -1, size * sizeof(suffix_t));

    sa->size = size;

    return sa;
}

void free_suffix_array(suffix_array_t *suffix_array)
{
    if (suffix_array == NULL) return;

    free(suffix_array->elements);
}

suffix_t *suffix_at(suffix_array_t *suffix_array, const int index)
{
    if (suffix_array == NULL) return NULL;

    if (index >= suffix_array->size) return NULL;

    return &suffix_array->elements[index];
}