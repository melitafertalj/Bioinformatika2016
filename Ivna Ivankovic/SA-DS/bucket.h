#ifndef BUCKET_H

#define BUCKET_H

#include <stdint.h>

typedef struct
{
    char *substring;
    size_t length;
} bucket_t;

bucket_t *create_bucket(char *substring, const size_t length);

int bucket_compare(bucket_t *first, bucket_t *second);

#endif // BUCKET_H
