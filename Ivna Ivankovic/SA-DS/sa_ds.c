#include "sa_ds.h"

#include <stdint.h>

#include "bucket_array.h"
#include "int_array.h"
#include "sort_data.h"

#define INPUT_RANGE UINT16_MAX

int d_ = 2;

int sa_ds_int_impl(int_array_t *string, suffix_array_t *suffix_array);

void deduce_type(int_array_t *string, sort_data_t *sort_data);

void deduce_lms_markers(int_array_t *string, sort_data_t *sort_data);

int_array_t *deduce_d_critical_markers(int_array_t *string, sort_data_t *sort_data);

bucket_array_t *deduce_suffix_buckets(int_array_t *string, int_array_t *d_critical_indices);

int_array_t *create_new_string(int_array_t *string, int_array_t *d_critical_indices, bucket_array_t *bucket_array);

int induce_suffix_array(int_array_t *string, sort_data_t *sort_data, int_array_t *d_critical_indices, suffix_array_t *first_array, suffix_array_t *second_array);

int sa_ds(int_array_t *string, suffix_array_t * suffix_array, int d)
{
    if (string == NULL) return 0;
    if (suffix_array == NULL) return 0;

    d_ = d;

    return sa_ds_int_impl(string, suffix_array);
}

int sa_ds_int_impl(int_array_t *string, suffix_array_t *suffix_array)
{
    sort_data_t *sort_data = create_sort_data(string->size);
    if (sort_data == NULL) return 0;

    deduce_type(string, sort_data);
    deduce_lms_markers(string, sort_data);

    int_array_t *d_critical_indices = deduce_d_critical_markers(string, sort_data);
    if (d_critical_indices == NULL)
    {
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    bucket_array_t *bucket_array = deduce_suffix_buckets(string, d_critical_indices);

    int_array_t *new_string = create_new_string(string, d_critical_indices, bucket_array);
    if (new_string == NULL)
    {
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_int_array(d_critical_indices);
        free(d_critical_indices);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    suffix_array_t *new_suffix_array = create_suffix_array(new_string->size);
    if (new_suffix_array == NULL)
    {
        free_int_array(new_string);
        free(new_string);
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_int_array(d_critical_indices);
        free(d_critical_indices);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    if (new_string->size == bucket_array->size)
    {
        for (int i = 0; i < new_string->size; ++i)
        {
            *suffix_at(new_suffix_array, new_string->elements[i]) = i;
        }
    }
    else if (!sa_ds_int_impl(new_string, new_suffix_array))
    {
        free_suffix_array(new_suffix_array);
        free(new_suffix_array);
        free(new_string);
        free_bucket_array(bucket_array);
        free(bucket_array);
        free_int_array(d_critical_indices);
        free(d_critical_indices);
        free_sort_data(sort_data);
        free(sort_data);
        return 0;
    }

    int result = induce_suffix_array(string, sort_data, d_critical_indices, suffix_array, new_suffix_array);

    // housekeeping
    free_suffix_array(new_suffix_array);
    free(new_suffix_array);
    free(new_string);
    free_bucket_array(bucket_array);
    free(bucket_array);
    free_int_array(d_critical_indices);
    free(d_critical_indices);
    free_sort_data(sort_data);
    free(sort_data);

    return result;
}

void deduce_type(int_array_t *string, sort_data_t *sort_data)
{
    // Last symbol is default initalized as S-type
    //int length = strlen(string);
    //sort_data[length - 1].type = s_type;

    for (int i = string->size - 2; i >= 0; --i)
    {
        if (string->elements[i] < string->elements[i + 1])  attributes_at(sort_data, i)->type = s_type;
        else if (string->elements[i] == string->elements[i + 1]) attributes_at(sort_data, i)->type = attributes_at(sort_data, i + 1)->type;
        else attributes_at(sort_data, i)->type = l_type;
    }
}

void deduce_lms_markers(int_array_t *string, sort_data_t *sort_data)
{
    for (int i = 0; i < string->size - 1; ++i)
    {
        if (attributes_at(sort_data, i)->type == l_type && attributes_at(sort_data, i + 1)->type == s_type)
        {
            attributes_at(sort_data, i + 1)->lms_marker = true;
            ++i; // No need to check the marked symbol.
        }
    }
}

int_array_t *deduce_d_critical_markers(int_array_t *string, sort_data_t *sort_data)
{
    int_array_t *d_critical_indices = create_int_array(string->size);
    if (d_critical_indices == NULL) return NULL;

    int j = 0;
    for (int i = -1; i < string->size - 1;)
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
        d_critical_indices->elements[j] = i;
        ++j;
    }

    d_critical_indices->elements = (int *) realloc(d_critical_indices->elements, j * sizeof(int));
    if (d_critical_indices->elements == NULL) { free(d_critical_indices); return NULL; }
    d_critical_indices->size = j;

    return d_critical_indices;
}

bucket_array_t *create_unique_buckets(int_array_t *string, int_array_t *d_critical_indices)
{
    bucket_array_t *suffix_buckets = create_bucket_array(d_critical_indices->size);
    if (suffix_buckets == NULL) { return NULL; }

    int bucket_count = 0;
    for (int i = 0; i < d_critical_indices->size; ++i)
    {
        bool is_duplicate = false;
        bucket_t bucket = { string->elements + d_critical_indices->elements[i], d_ + 2 };

        for (int j = 0; j < bucket_count; ++j)
        {
            if (bucket_compare(&bucket, bucket_at(suffix_buckets, j)) == 0)
            {
                is_duplicate = true;
                break;
            }
        }

        if (is_duplicate) { continue; }

        suffix_buckets->buckets[bucket_count++] = bucket;
    }

    if (!adjust_bucket_array_size(suffix_buckets, bucket_count))
    {
        free_bucket_array(suffix_buckets);
        free(suffix_buckets);
        return NULL;
    }

    return suffix_buckets;
}

bucket_array_t *deduce_suffix_buckets(int_array_t *string, int_array_t *d_critical_indices)
{
    bucket_array_t *suffix_buckets = create_unique_buckets(string, d_critical_indices);
    if (suffix_buckets == NULL) { return NULL; }

    sort_bucket_array(suffix_buckets);

    return suffix_buckets;
}

int_array_t *create_new_string(int_array_t *string, int_array_t *d_critical_indices, bucket_array_t *bucket_array)
{
    int_array_t *new_string = create_int_array(d_critical_indices->size);
    if (new_string == NULL) return NULL;

    for (int i = 0; i < d_critical_indices->size; ++i)
    {
        bucket_t bucket = { string->elements + d_critical_indices->elements[i], d_ + 2 };

        for (int j = 0; j < bucket_array->size; ++j)
        {
            if (bucket_compare(&bucket, bucket_at(bucket_array, j)) == 0)
            {
                new_string->elements[i] = j;
                break;
            }
        }
    }

    return new_string;
}

int_array_t *get_buckets(int_array_t *string, bool is_end)
{
    int_array_t *buckets = create_int_array(INPUT_RANGE + 1);
    if (buckets == NULL) return NULL;

    for (int i = 0; i < string->size; ++i)
    {
        (*element_at(buckets, string->elements[i]))++;
    }

    int sum = 0;
    for (int i = 0; i < INPUT_RANGE + 1; ++i)
    {
        element_t *element = element_at(buckets, i);

        sum += *element;
        *element = is_end ? sum : sum - *element;
    }

    return buckets;
}

int induce_suffix_array(int_array_t *string, sort_data_t *sort_data, int_array_t *d_critical_indices, suffix_array_t *first_array, suffix_array_t *second_array)
{
    int_array_t *buckets = get_buckets(string, true);
    if (buckets == NULL) return 0;

    for (int i = second_array->size - 1; i >= 0; --i)
    {
        int index = d_critical_indices->elements[*suffix_at(second_array, i)];

        if (attributes_at(sort_data, index)->lms_marker)
        {
            element_t *bucket = element_at(buckets, string->elements[index]);
            --(*bucket);
            *suffix_at(first_array, *bucket) = index;
        }
    }

    free_int_array(buckets);
    free(buckets);
    buckets = get_buckets(string, false);
    if (buckets == NULL) return 0;

    for (int i = 0; i < first_array->size; ++i)
    {
        suffix_t *suffix = suffix_at(first_array, i);
        if (*suffix < 0) { continue; }

        int j = *suffix - 1;
        if (attributes_at(sort_data, j)->type == l_type)
        {
            element_t *element = element_at(buckets, string->elements[j]);
            *suffix_at(first_array, *element) = j;
            ++(*element);
        }
    }

    free_int_array(buckets);
    free(buckets);
    buckets = get_buckets(string, true);
    if (buckets == NULL) return 0;

    for (int i = first_array->size - 1; i >= 0; --i)
    {
        suffix_t *suffix = suffix_at(first_array, i);
        if (*suffix < 0) { continue; }

        int j = *suffix - 1;
        if (j >= 0 && attributes_at(sort_data, j)->type == s_type)
        {
            element_t *element = element_at(buckets, string->elements[j]);
            --(*element);
            *suffix_at(first_array, *element) = j;
        }
    }

    free_int_array(buckets);
    free(buckets);
    return 1;
}