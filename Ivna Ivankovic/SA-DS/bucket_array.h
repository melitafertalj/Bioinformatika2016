#ifndef BUCKET_ARRAY_H

#define BUCKET_ARRAY_H

#include "bucket.h"

typedef struct
{
    bucket_t *buckets;
    size_t size;
} bucket_array_t;

bucket_array_t *create_bucket_array(const size_t size);

bucket_t *bucket_at(bucket_array_t *bucket_array, const size_t index);

void free_bucket_array(bucket_array_t *bucket_array);

#endif // BUCKET_ARRAY_H
