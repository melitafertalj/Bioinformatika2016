#include "sort_data.h"

void initialize_attribute(attribute_t *attribute)
{
    attribute->type = s_type;
    attribute->lms_marker = false;
    attribute->d_critical_marker = false;
}

sort_data_t *create_sort_data(const int size)
{
    sort_data_t *sort_data = (sort_data_t *) malloc(sizeof(sort_data_t));
    if (sort_data == NULL) return NULL;

    sort_data->attributes = (attribute_t *) malloc(size * sizeof(attribute_t));
    if (sort_data->attributes == NULL) { free(sort_data); return 0; }

    for (int i = 0; i < size; ++i)
    {
        initialize_attribute(&sort_data->attributes[i]);
    }

    sort_data->length = size;
    return sort_data;
}

void free_sort_data(sort_data_t *sort_data)
{
    if (sort_data == NULL) return;

    free(sort_data->attributes);
}

attribute_t *attributes_at(sort_data_t *sort_data, const int index)
{
    return &sort_data->attributes[index];
}