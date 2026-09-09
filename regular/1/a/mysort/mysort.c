/**
 * @file mysort.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-09-09
 *
 * @copyright Copyright (c) 2026
 *
 * @details it just an implementation of qsort() 1
 * <https://linux.die.net/man/3/qsort> for further details
 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <unistd.h>

#define error(msg, program_name)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s\n", msg);                                                                                           \
        fprintf(stderr, "Usage: %s [-r] [file1]...\n", program_name);                                                  \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

typedef struct options_t
{
    bool reverse;
} options_t;

typedef struct lines_t
{
    char **data;
    size_t count;
    size_t capacity;
} lines_t;

static int cmpstringp(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
       pointers to char", but strcmp(3) arguments are "pointers
       to char", hence the following cast plus dereference */

    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

static int cmpstringp_rev(const void *p1, const void *p2)
{
    return cmpstringp(p2, p1);
}

int readLines(FILE *input, lines_t *lines)
{
    if (input == NULL)
    {
        error("INPUT EMPTY ERROR!", "readLines()");
        return -1;
    }

    char *line = NULL;
    size_t line_buffer_len = 0;
    ssize_t read_size = 0;

    while ((read_size = getline(&line, &line_buffer_len, input)) != -1)
    {
        if (read_size > 0 && line[read_size - 1] == '\n')
            line[read_size - 1] = '\0';

        if (lines->count == lines->capacity)
        {
            // initialize base capacity
            lines->capacity = lines->capacity ? lines->capacity * 2 : 128;
            lines->data = realloc(lines->data, lines->capacity * sizeof(char *));
            if (lines->data == NULL)
                error("realloc failed", "readLines()");
        }

        lines->data[lines->count++] = strdup(line);
    }

    free(line);
    return 0;
}

void sortAndPrint(lines_t *lines, FILE *output, bool reverse)
{
    qsort(lines->data, lines->count, sizeof(char *), reverse ? cmpstringp_rev : cmpstringp);

    for (size_t i = 0; i < lines->count; ++i)
    {
        fprintf(output, "%s\n", lines->data[i]);
        free(lines->data[i]);
    }
    free(lines->data);
}

int main(int argc, char *argv[])
{
    FILE *INPUT, *OUTPUTFILE = stdout;

    if (OUTPUTFILE == NULL)
        error("Failed to allocate OUPUTFILE", "OUTPUTFILE");

    int option;
    bool reverse = false;

    while ((option = getopt(argc, argv, "r")) != -1)
    {
        switch (option)
        {
        case 'r':
            reverse = true;
            break;
        case ':':
            error("Error no Arguments given", argv[0]);
            break;

        case '?':
            error("unknown arg", argv[0]);
            break;
        }
    }

    options_t opts = {.reverse = reverse};
    lines_t lines = {.data = NULL, .count = 0, .capacity = 0};

    do
    {
        if ((argc - optind) == 0)
            INPUT = stdin;
        else
            INPUT = fopen(argv[optind], "r+");

        if (INPUT == NULL)
            error("failed to allocate the INPUTFILE", "main()");

        readLines(INPUT, &lines);

        fclose(INPUT);
    } while (++optind < argc);

    sortAndPrint(&lines, OUTPUTFILE, opts.reverse);
    fflush(OUTPUTFILE);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}