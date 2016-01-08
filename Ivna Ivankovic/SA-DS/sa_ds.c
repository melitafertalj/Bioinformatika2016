#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sa_ds.h"

#define D_LEN 2

typedef enum { l_type = 0, s_type = 1 } type_t;

typedef struct
{
    type_t type;
    bool lms_marker;
    bool d_critical_marker;
} sort_data_t;

void initialize_sort_data(sort_data_t *sort_data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        sort_data[i].type = s_type;
        sort_data[i].lms_marker = false;
        sort_data[i].d_critical_marker = false;
    }
}

// Deduces if the symbol is L or S type.
void deduce_type(const char *string, sort_data_t *sort_data);

// Deduces positions of lms markers.
void deduce_lms_markers(const char *string, sort_data_t *sort_data);

// Deduces positions of d-critical markers.
size_t deduce_d_critical_markers(const char *string, const size_t d, sort_data_t *sort_data, size_t *d_critical_indices);

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
        if (string[i] < string[i + 1])  sort_data[i].type = s_type;
        else if (string[i] == string[i + 1]) sort_data[i].type = sort_data[i + 1].type;
        else sort_data[i].type = l_type;
    }
}

void deduce_lms_markers(const char *string, sort_data_t *sort_data)
{
    for (size_t i = 0; i < strlen(string) - 1; ++i)
    {
        if (sort_data[i].type == l_type && sort_data[i + 1].type == s_type)
        {
            sort_data[i + 1].lms_marker = true;
            ++i; // No need to check the marked symbol.
        }
    }
}

size_t deduce_d_critical_markers(const char *string, const size_t d, sort_data_t *sort_data, size_t *d_critical_indices)
{
    size_t j = 0;
    for (size_t i = -1; i < strlen(string) - 1;)
    {
        size_t h = -1;
        bool is_lms = false;
        for (h = 2; h <= d + 1; ++h)
        {
            if (sort_data[i + h].lms_marker)
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