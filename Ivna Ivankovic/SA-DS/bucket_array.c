#include "bucket_array.h"

bucket_array_t *create_bucket_array(const size_t size)
{
    bucket_array_t *bucket_array = (bucket_array_t *) malloc(sizeof(bucket_array_t));

    if (bucket_array == NULL) return NULL;

    bucket_array->buckets = (bucket_t *) calloc(size, sizeof(bucket_t));

    if (bucket_array->buckets == NULL) { free(bucket_array); return NULL; }

    bucket_array->size = size;

    return bucket_array;
}

bucket_t *bucket_at(bucket_array_t *bucket_array, const size_t index)
{
    return &bucket_array->buckets[index];
}

void free_bucket_array(bucket_array_t *bucket_array)
{
    if (bucket_array == NULL) return;

    free(bucket_array->buckets);
}