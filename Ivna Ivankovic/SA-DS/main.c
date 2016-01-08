#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "suffix_array.h"
#include "sa_ds.h"

#define DEBUG

#define INITIAL_SIZE 1000

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

    char *string = (char *) calloc(INITIAL_SIZE, sizeof(char));
    if (string == NULL)
    {
        fclose(output_file);
        fclose(input_file);
        report_error_and_exit("Insufficient resources for string allocation!", EXIT_FAILURE);
    }

    if (read_input_string(input_file, string) == 0)
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

    sa_ds(string, suffix_array);

    for (size_t i = 0; i < suffix_array->size; ++i)
    {
#ifdef DEBUG
        printf("%s\n", at_index(suffix_array, i));
#endif // DEBUG

        fprintf(output_file, "%s\n", at_index(suffix_array, i));
        if (ferror(output_file))
        {
            free_suffix_array(suffix_array);
            free(suffix_array);
            free(string);
            fclose(output_file);
            report_error_and_exit("IO error happend while saving results!", EXIT_FAILURE);
        }
    }

    // housekeeping
    free_suffix_array(suffix_array);
    free(suffix_array);
    free(string);

    return EXIT_SUCCESS;
}

void report_error_and_exit(const char *message, const int value)
{
    fprintf(stderr, message);
    exit(value);
}

int read_input_string(FILE * file, char ** string)
{
    return 0;
}