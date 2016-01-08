#include <stdlib.h>
#include <string.h>

#include "bucket.h"

bucket_t *create_bucket(char *substring, const int length)
{
    bucket_t *bucket = (bucket_t *) malloc(sizeof(bucket));

    if (bucket == NULL) return NULL;

    bucket->substring = substring;
    bucket->length = length;

    return bucket;
}

int bucket_compare(bucket_t *first, bucket_t *second)
{
    return strncmp(first->substring, second->substring, first->length);
}