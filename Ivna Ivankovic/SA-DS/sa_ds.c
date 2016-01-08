#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sa_ds.h"

#include "bucket.h"
#include "bucket_array.h"

int d = 2;

typedef enum { l_type = 0, s_type = 1 } type_t;

typedef struct
{
    type_t type;
    bool lms_marker;
    bool d_critical_marker;
} attribute_t;

void initialize_attribute(attribute_t *attribute)
{
    attribute->type = s_type;
    attribute->lms_marker = false;
    attribute->d_critical_marker = false;
}

typedef struct
{
    attribute_t *attributes;
    size_t length;
} sort_data_t;

sort_data_t *create_sort_data(const size_t size)
{
    sort_data_t *sort_data = (sort_data_t *) malloc(sizeof(sort_data_t));

    if (sort_data == NULL) return NULL;

    for (size_t i = 0; i < size; ++i)
    {
        initialize_attribute(&sort_data->attributes[i]);
    }

    return sort_data;
}

void free_sort_data(sort_data_t *sort_data)
{
    if (sort_data == NULL) return;

    free(sort_data->attributes);
}

attribute_t *attributes_at(sort_data_t *sort_data, const size_t index)
{
    return &sort_data->attributes[index];
}

// Deduces if the symbol is L or S type.
void deduce_type(const char *string, sort_data_t *sort_data);

// Deduces positions of lms markers.
void deduce_lms_markers(const char *string, sort_data_t *sort_data);

// Deduces positions of d-critical markers.
size_t deduce_d_critical_markers(const char *string, sort_data_t *sort_data, size_t *d_critical_indices);

// Creates unique buckets from d-critical substrings
int create_unique_buckets(char *string, size_t *d_critical_indices, size_t size, bucket_array_t *bucket_array);

// S is the input string;
// SA is the output suffix array of S;
// t: array[0..n - 1] of boolean
// P1, S1: array[0..n1] of integer; -> n1 = ||S1||
// B: array [0..||Sum(S)|| - 1] of integer
// Scan S once to classify all the characters as L - or S - type into t;
// Scan t once to find all the d-critical substrings in S into P1;
// Bucket sort all the d-critical substrings using P1 and B;
// Name each d-critical substring in S by its bucket index to get a new shortened string S1;
// if ||S1|| = Number of Buckets:
//    Directly compute SA1 from S1;
// else
//    SA-DS(S1, SA1);
// Induce SA from SA1
// Return
int sa_ds(const char *string, suffix_array_t *suffix_array)
{
    return 0;
}

void deduce_type(const char *string, sort_data_t *sort_data)
{
    // Last symbol is default initalized as S-type
    //size_t length = strlen(string);
    //sort_data[length - 1].type = s_type;

    for (size_t i = strlen(string) - 2; i >= 0; --i)
    {
        if (string[i] < string[i + 1])  attributes_at(sort_data, i)->type = s_type;
        else if (string[i] == string[i + 1]) attributes_at(sort_data, i)->type = attributes_at(sort_data, i + 1)->type;
        else attributes_at(sort_data, i)->type = l_type;
    }
}

void deduce_lms_markers(const char *string, sort_data_t *sort_data)
{
    for (size_t i = 0; i < strlen(string) - 1; ++i)
    {
        if (attributes_at(sort_data, i)->type == l_type && attributes_at(sort_data, i + 1)->type == s_type)
        {
            attributes_at(sort_data, i)->lms_marker = true;
            ++i; // No need to check the marked symbol.
        }
    }
}

size_t deduce_d_critical_markers(const char *string, sort_data_t *sort_data, size_t *d_critical_indices)
{
    size_t j = 0;
    for (size_t i = -1; i < strlen(string) - 1;)
    {
        size_t h = -1;
        bool is_lms = false;
        for (h = 2; h <= d + 1; ++h)
        {
            if (attributes_at(sort_data, i + h)->lms_marker)
            {
                is_lms = true;
                break;
            }
        }

        if (j == 0 && !is_lms)
        {
            i += d;
            continue;
        }

        i += is_lms ? h : d;
        d_critical_indices[j] = i;
        ++j;
    }
    return j;
}

int create_unique_buckets(char *string, size_t *d_critical_indices, size_t size, bucket_array_t *bucket_array)
{
    size_t bucket_count = 0;
    for (size_t i = 0; i < size; ++i)
    {
        bool is_duplicate = false;
        bucket_t *bucket = create_bucket(string + d_critical_indices[i], d + 2);

        if (bucket == NULL) return 0;

        for (size_t j = 0; j < size; ++j)
        {
            if (bucket_compare(bucket, bucket_at(bucket_array, j)) == 0)
            {
                is_duplicate = true;
                break;
            }
        }

        if (is_duplicate)
        {
            free(bucket);
            continue;
        }

        bucket_array->buckets[bucket_count].substring = bucket->substring;
        bucket_array->buckets[bucket_count].length = bucket->length;

        free(bucket);
    }

    return 1;
}