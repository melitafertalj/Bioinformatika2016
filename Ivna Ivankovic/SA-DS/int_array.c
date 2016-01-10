#include <stdlib.h>

#include "int_array.h"

int_array_t *create_int_array(const int size)
{
    int_array_t *int_array = (int_array_t *) malloc(sizeof(int_array_t));
    if (int_array == NULL) return NULL;

    int_array->elements = (int *) calloc(size, sizeof(int));
    if (int_array->elements == NULL) { free(int_array); return NULL; }

    int_array->size = size;

    return int_array;
}

element_t *element_at(int_array_t *int_array, const int index)
{
    return &int_array->elements[index];
}