#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "suffix_array.h"
#include "sa_ds.h"

#define D_LEN 2

#define BLOCK_SIZE 1000

// Reports the error to stderr and terminates the program.
void report_error_and_exit(const char *message, const int value);

int read_input_string(FILE *file, int_array_t **string);

// First parameter is the input file, second is the output file
int main(int argc, char *argv[])
{
    if (!(argc == 3 || argc == 4)) report_error_and_exit("Specifiy the IO files!", EXIT_FAILURE);

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        report_error_and_exit("Unable to open input file!", EXIT_FAILURE);
    }

    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        fclose(input_file);
        report_error_and_exit("Unable to open output file!", EXIT_FAILURE);
    }

    FILE *suffix_file = NULL;
    if (argc == 4)
    {
        suffix_file = fopen(argv[3], "w");
        if (suffix_file == NULL)
        {
            fclose(output_file);
            fclose(input_file);
            report_error_and_exit("Unable to open suffix output file!", EXIT_FAILURE);
        }
    }

    int_array_t *string = NULL;
    if (read_input_string(input_file, &string) == 0)
    {
        fclose(output_file);
        fclose(input_file);
        report_error_and_exit("IO error happend while reading the string!", EXIT_FAILURE);
    }

    fclose(input_file); // Don't need it anymore.

    suffix_array_t *suffix_array = create_suffix_array(string->size);
    if (suffix_array == NULL)
    {
        free_int_array(string);
        free(string);
        fclose(output_file);
        report_error_and_exit("Unable to create initial suffix array!", EXIT_FAILURE);
    }

    int start = time(NULL);
    sa_ds(string, suffix_array, D_LEN);
    int end = time(NULL);

    printf("%d", end - start);

    string->elements[string->size - 1] = '$'; // Replace the virtual sentinel with a visible character
    for (int i = 0; i < suffix_array->size; ++i)
    {
        suffix_t *element = suffix_at(suffix_array, i);

        if (suffix_file != NULL)
        {
            fprintf(suffix_file, "%-10d", *element);
            for (int j = *element; j < string->size; ++j) { fprintf(suffix_file, "%c", string->elements[j]); }
            fprintf(suffix_file, "\n");
        }

        fprintf(output_file, "%d ", *element);
        if (ferror(output_file))
        {
            free_suffix_array(suffix_array);
            free(suffix_array);
            free_int_array(string);
            free(string);
            fclose(output_file);
            report_error_and_exit("IO error happend while saving results!", EXIT_FAILURE);
        }
    }
    fprintf(output_file, "\n");

    if (argc == 4) fclose(suffix_file);

    // housekeeping
    free_suffix_array(suffix_array);
    free(suffix_array);
    fclose(output_file);
    free_int_array(string);
    free(string);

    return EXIT_SUCCESS;
}

void report_error_and_exit(const char *message, const int value)
{
    fprintf(stderr, message);
    exit(value);
}

int read_input_string(FILE *file, int_array_t **string)
{
    int block_count = 0;
    int length = 0;
    int allocated_size = 0;
    int *local = NULL;
    while (!feof(file))
    {
        char symbol;
        fread(&symbol, sizeof(symbol), 1, file);
        if (ferror(file)) { free(local); return 0; }

        if (symbol == '\n') { continue; } //New lines are ignored.

        ++length;
        if (length >= allocated_size)
        {
            local = realloc(local, (block_count + 1) * BLOCK_SIZE * sizeof(int));
            if (local == NULL) { return 0; }

            allocated_size = (block_count + 1) * BLOCK_SIZE;
            ++block_count;
        }

        local[length - 1] = symbol;
    }
    local[length] = INT_MIN; //Virtual sentinel. This symbol won't appear in the input

    int_array_t *local_array = (int_array_t *) malloc(sizeof(int_array_t));
    if (local_array == NULL) { free(local); return 0; }

    local_array->elements = local;
    local_array->size = length + 1;
    *string = local_array;

    return 1;
}