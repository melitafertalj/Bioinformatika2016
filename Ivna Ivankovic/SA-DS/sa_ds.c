#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sa_ds.h"

#include "bucket.h"
#include "bucket_array.h"

int d_ = 2;

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
    int length;
} sort_data_t;

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

// Implementation of SA-DS
int sa_ds_impl(char *string, suffix_array_t *suffix_array);

// Deduces if the symbol is L or S type.
void deduce_type(const char *string, sort_data_t *sort_data);

// Deduces positions of lms markers.
void deduce_lms_markers(const char *string, sort_data_t *sort_data);

// Deduces positions of d-critical markers.
int deduce_d_critical_markers(const char *string, sort_data_t *sort_data, int *d_critical_indices);

// Creates unique buckets from d-critical substrings.
int create_unique_buckets(char *string, int *d_critical_indices, int size, bucket_array_t *bucket_array);

// Creates the shortend string.
char *create_new_string(char *string, int *d_critical_indices, int size, bucket_array_t *bucket_array);

// Induces SA (first_array) from SA1 (second_array).
int induce_suffix_array(suffix_array_t *first_array, suffix_array_t *second_array);

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
int sa_ds(char *string, suffix_array_t *suffix_array, int d)
{
    if (string == NULL) return 0;
    if (suffix_array == NULL) return 0;

    d_ = d;

    return sa_ds_impl(string, suffix_array);
}

int sa_ds_impl(char *string, suffix_array_t *suffix_array)
{
    sort_data_t *sort_data = create_sort_data(strlen(string));
    if (sort_data == NULL) return 0;

    deduce_type(string, sort_data);
    deduce_lms_markers(string, sort_data);

    int *d_critical_indices = (int *) malloc(strlen(string) * sizeof(int));
    if (d_critical_indices == NULL)
    {
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    int indices_count = deduce_d_critical_markers(string, sort_data, d_critical_indices);

    d_critical_indices = (int *) realloc(d_critical_indices, indices_count * sizeof(int));
    if (d_critical_indices == NULL)
    {
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    bucket_array_t *bucket_array = create_bucket_array(indices_count);
    if (bucket_array == NULL)
    {
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    if (!create_unique_buckets(string, d_critical_indices, indices_count, bucket_array))
    {
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    sort_bucket_array(bucket_array);

    char *new_string = create_new_string(string, d_critical_indices, indices_count, bucket_array);
    if (new_string == NULL)
    {
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    int new_size = strlen(new_string);
    suffix_array_t *new_suffix_array = create_suffix_array(new_size);
    if (new_size == bucket_array->size)
    {
        for (int i = 0; i < new_size; ++i)
        {
            *suffix_at(new_suffix_array, new_string[i]) = i;
        }
    }
    else if (!sa_ds_impl(new_string, new_suffix_array))
    {
        free_suffix_array(new_suffix_array);
        free(new_suffix_array);
        free(new_string);
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    int result = induce_suffix_array(suffix_array, new_suffix_array);

    // housekeeping
    free_suffix_array(new_suffix_array);
    free(new_suffix_array);
    free(new_string);
    free_bucket_array(bucket_array);
    free(bucket_array);
    free_sort_data(sort_data);
    free(sort_data);

    return result;
}

void deduce_type(const char *string, sort_data_t *sort_data)
{
    // Last symbol is default initalized as S-type
    //int length = strlen(string);
    //sort_data[length - 1].type = s_type;

    for (int i = strlen(string) - 2;; --i)
    {
        if (string[i] < string[i + 1])  attributes_at(sort_data, i)->type = s_type;
        else if (string[i] == string[i + 1]) attributes_at(sort_data, i)->type = attributes_at(sort_data, i + 1)->type;
        else attributes_at(sort_data, i)->type = l_type;

        if (i == 0) break;
    }
}

void deduce_lms_markers(const char *string, sort_data_t *sort_data)
{
    for (int i = 0; i < strlen(string) - 1; ++i)
    {
        if (attributes_at(sort_data, i)->type == l_type && attributes_at(sort_data, i + 1)->type == s_type)
        {
            attributes_at(sort_data, i + 1)->lms_marker = true;
            ++i; // No need to check the marked symbol.
        }
    }
}

int deduce_d_critical_markers(const char *string, sort_data_t *sort_data, int *d_critical_indices)
{
    int j = 0;
    for (int i = -1; i < (int) (strlen(string) - 1);)
    {
        int h = -1;
        bool is_lms = false;
        for (h = 2; h <= d_ + 1; ++h)
        {
            if (attributes_at(sort_data, i + h)->lms_marker)
            {
                is_lms = true;
                break;
            }
        }

        if (j == 0 && !is_lms)
        {
            i += d_;
            continue;
        }

        i += is_lms ? h : d_;
        d_critical_indices[j] = i;
        ++j;
    }
    return j;
}

int create_unique_buckets(char *string, int *d_critical_indices, int size, bucket_array_t *bucket_array)
{
    int bucket_count = 0;
    for (int i = 0; i < size; ++i)
    {
        bool is_duplicate = false;
        bucket_t bucket = { string + d_critical_indices[i], d_ + 2 };

        for (int j = 0; j < bucket_count; ++j)
        {
            if (bucket_compare(&bucket, bucket_at(bucket_array, j)) == 0)
            {
                is_duplicate = true;
                break;
            }
        }

        if (is_duplicate) { continue; }

        bucket_array->buckets[bucket_count++] = bucket;
    }

    return adjust_bucket_array_size(bucket_array, bucket_count);
}

char *create_new_string(char *string, int *d_critical_indices, int size, bucket_array_t *bucket_array)
{
    char *new_string = (char *) malloc(size * sizeof(char));
    if (new_string == NULL) return NULL;

    for (int i = 0; i < size; ++i)
    {
        bucket_t bucket = { string + d_critical_indices[i], d_ + 2 };

        for (int j = 0; j < bucket_array->size; ++j)
        {
            if (bucket_compare(&bucket, bucket_at(bucket_array, j)) == 0)
            {
                new_string[i] = (char) (j + '0');
                break;
            }
        }
    }
    new_string[size] = '\0';

    return new_string;
}

int induce_suffix_array(suffix_array_t *first_array, suffix_array_t *second_array)
{
    return 0;
}