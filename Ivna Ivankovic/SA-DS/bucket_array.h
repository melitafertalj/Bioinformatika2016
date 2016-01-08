#ifndef BUCKET_ARRAY_H

#define BUCKET_ARRAY_H

#include "bucket.h"

typedef struct
{
    bucket_t *buckets;
    size_t size;
} bucket_array_t;

bucket_array_t *create_bucket_array(const size_t size);

void free_bucket_array(bucket_array_t *bucket_array);

bucket_t *bucket_at(bucket_array_t *bucket_array, const size_t index);

int adjust_bucket_array_size(bucket_array_t *bucket_array, const size_t new_size);

void sort_bucket_array(bucket_array_t *bucket_array);

#endif // BUCKET_ARRAY_H
