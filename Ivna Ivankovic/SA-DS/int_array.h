#ifndef INT_ARRAY_H

#define INT_ARRAY_H

typedef int element_t;

typedef struct
{
    element_t *elements;
    int size;
} int_array_t;

int_array_t *create_int_array(const int size);

void free_int_array(int_array_t *int_array);

element_t *element_at(int_array_t *int_array, const int index);

#endif // INT_ARRAY_H
