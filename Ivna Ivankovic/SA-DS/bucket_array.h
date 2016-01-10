#ifndef BUCKET_ARRAY_H

#define BUCKET_ARRAY_H

typedef struct
{
    char *substring;
    int length;
} bucket_t;

bucket_t *create_bucket(char *substring, const int length);

int bucket_compare(bucket_t *first, bucket_t *second);

typedef struct
{
    bucket_t *buckets;
    int size;
} bucket_array_t;

bucket_array_t *create_bucket_array(const int size);

void free_bucket_array(bucket_array_t *bucket_array);

bucket_t *bucket_at(bucket_array_t *bucket_array, const int index);

int adjust_bucket_array_size(bucket_array_t *bucket_array, const int new_size);

void sort_bucket_array(bucket_array_t *bucket_array);

#endif // BUCKET_ARRAY_H
