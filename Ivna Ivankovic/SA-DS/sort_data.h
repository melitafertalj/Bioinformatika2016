#ifndef SORT_DATA_H

#define SORT_DATA_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum { l_type = 0, s_type = 1 } type_t;

typedef struct
{
    type_t type;
    bool lms_marker;
    bool d_critical_marker;
} attribute_t;

void initialize_attribute(attribute_t *attribute);
typedef struct
{
    attribute_t *attributes;
    int length;
} sort_data_t;

sort_data_t *create_sort_data(const int size);

void free_sort_data(sort_data_t *sort_data);

attribute_t *attributes_at(sort_data_t *sort_data, const int index);
#endif // SORT_DATA_H
