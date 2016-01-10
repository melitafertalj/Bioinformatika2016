#include "bucket_array.h"

#include <stdlib.h>

bucket_t *create_bucket(int *substring, const int length)
{
    bucket_t *bucket = (bucket_t *) malloc(sizeof(bucket));

    if (bucket == NULL) return NULL;

    bucket->substring = substring;
    bucket->length = length;

    return bucket;
}

int bucket_compare(const void *first, const void *second)
{
    bucket_t left = *((bucket_t *) first);
    bucket_t right = *((bucket_t *) second);
    for (int i = 0; i < right.length; i++)
    {
        if (left.substring[i] < right.substring[i]) return -1;
        else if (left.substring[i] > right.substring[i]) return 1;
    }

    return 0;
}

bucket_array_t *create_bucket_array(const int size)
{
    bucket_array_t *bucket_array = (bucket_array_t *) malloc(sizeof(bucket_array_t));

    if (bucket_array == NULL) return NULL;

    bucket_array->buckets = (bucket_t *) calloc(size, sizeof(bucket_t));

    if (bucket_array->buckets == NULL) { free(bucket_array); return NULL; }

    bucket_array->size = size;

    return bucket_array;
}

void free_bucket_array(bucket_array_t *bucket_array)
{
    if (bucket_array == NULL) return;

    free(bucket_array->buckets);
}

bucket_t *bucket_at(bucket_array_t *bucket_array, const int index)
{
    return bucket_array->buckets + index;
}

int adjust_bucket_array_size(bucket_array_t *bucket_array, const int new_size)
{
    if (bucket_array == NULL) return 0;

    bucket_array->buckets = (bucket_t *) realloc(bucket_array->buckets, new_size * sizeof(bucket_t));
    bucket_array->size = new_size;

    return bucket_array->buckets != NULL;
}

void sort_bucket_array(bucket_array_t *bucket_array)
{
    if (bucket_array == NULL) return;

    qsort(bucket_array->buckets, bucket_array->size, sizeof(bucket_t), bucket_compare);
}