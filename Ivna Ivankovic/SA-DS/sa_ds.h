#ifndef SA_DS_H

#include "suffix_array.h"

// O(n) suffix array construction
int sa_ds(char *string, suffix_array_t *suffix_array, int d);

int sa_ds_int(int *string, int n, suffix_array_t *suffix_array, int d);

#endif // SA_DS_H
