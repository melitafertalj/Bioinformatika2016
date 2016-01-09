#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suffix_array.h"
#include "sa_ds.h"

#define DEBUG

#define D_LEN 2

#define BLOCK_SIZE 1000

// Reports the error to stderr and terminates the program.
void report_error_and_exit(const char *message, const int value);

int read_input_string(FILE *file, char **string);

// First parameter is the input file, second is the output file
int main(int argc, char *argv[])
{
    if (argc != 3) report_error_and_exit("Specifiy the IO files!", EXIT_FAILURE);

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        report_error_and_exit("Unable to open input file!", EXIT_FAILURE);
    }

    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        fclose(input_file);
        report_error_and_exit("Unable to open input file!", EXIT_FAILURE);
    }

    char *string = NULL;
    if (read_input_string(input_file, &string) == 0)
    {
        free(string);
        fclose(output_file);
        fclose(input_file);
        report_error_and_exit("IO error happend while reading the string!", EXIT_FAILURE);
    }

    fclose(input_file); // Don't need it anymore.

    suffix_array_t *suffix_array = create_suffix_array(strlen(string));
    if (suffix_array == NULL)
    {
        free(string);
        fclose(output_file);
        report_error_and_exit("Unable to create initial suffix array!", EXIT_FAILURE);
    }

    sa_ds(string, suffix_array, D_LEN);

    string[strlen(string) - 2] = '$'; // Replace the virtual sentinel with a visible character
    for (int i = 0; i < suffix_array->size; ++i)
    {
        suffix_t *element = suffix_at(suffix_array, i);

#ifdef DEBUG
        printf("%-10d %s\n", *element, string + *element);
#endif // DEBUG

        fprintf(output_file, "%d ", *element);
        if (ferror(output_file))
        {
            free_suffix_array(suffix_array);
            free(suffix_array);
            free(string);
            fclose(output_file);
            report_error_and_exit("IO error happend while saving results!", EXIT_FAILURE);
        }
    }
    fprintf(output_file, "\n");

    // housekeeping
    free_suffix_array(suffix_array);
    free(suffix_array);
    fclose(output_file);
    free(string);

    return EXIT_SUCCESS;
}

void report_error_and_exit(const char *message, const int value)
{
    fprintf(stderr, message);
    exit(value);
}

int read_input_string(FILE *file, char **string)
{
    int block_count = 0;
    int length = 0;
    int allocated_size = 0;
    char *local = NULL;
    while (!feof(file))
    {
        char symbol;
        fread(&symbol, sizeof(symbol), 1, file);
        if (ferror(file)) { free(local); return 0; }

        if (symbol == '\n') { continue; } //New lines are ignored.

        if (allocated_size == length)
        {
            local = realloc(local, (block_count + 1) * BLOCK_SIZE * sizeof(char));
            if (local == NULL) { return 0; }

            ++block_count;
        }

        local[length] = symbol;
        ++length;
    }
    local[length] = 1; //Virtual sentinel. This symbol won't appear in the input
    local[length] = '\0';
    *string = local;
    return 1;
}