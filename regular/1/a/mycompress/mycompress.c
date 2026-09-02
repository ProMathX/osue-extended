/**
 * @file mycompress.c
 * @author kernrkaftwerk(kernkraftdev@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2026-09-02
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define error(msg, program_name)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s\n", msg);                                                                                           \
        fprintf(stderr, "Usage: %s [-o outfile] [file]\n", program_name);                                              \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

void compress(char *);

int main(int argc, char **argv)
{

    FILE *INPUTFILE = stdin;
    assert(INPUTFILE != NULL);
    FILE *OUTPUTFILE = stdout;
    assert(OUTPUTFILE != NULL);

    bool optionOutput = false;

    int option;
    while ((option = getopt(argc, argv, "o:")))
    {
        switch (option)
        {
        case 'o':
            optionOutput = true;
            OUTPUTFILE = fopen(optarg, "w");
            assert(OUTPUTFILE != NULL);
            break;

        case ':':
            error("Error no Arguments given", argv[0]);
            break;

        case '?':
            error("unknown arg", argv[0]);
            break;
        }
    }

    if (optind > argc)
    {
        error("to many arguments", "in main");
    }

    int xs[128];

    for (int i = 0; i < 128; i++)
    {
        xs[i] = 0;
    }

    /**
    * wegen dem index von jenem char einfach
    while ((c = getchar()) != EOF) {
      array[c]++;
      }
      *
      */
    // int *p = &xs[97];
    // printf("%c%d\n", (char)(p - xs), *p);

    do
    {
        if ((argc - optind) == 0)
            INPUTFILE = stdin;
        else
            INPUTFILE = fopen(argv[optind], "r");

        if (INPUTFILE == NULL)
            error("Couldnt allocate input", "main");

        ssize_t read = 0;
        char *line = NULL;
        size_t line_buffer_len = 0;

        while ((read = getline(&line, &line_buffer_len, INPUTFILE)) != -1)
        {
            compress(line);
            free(line);
        }
        fflush(OUTPUTFILE);
        free(line);
        line = NULL;
        line_buffer_len = 0;
        assert(feof(INPUTFILE));
        fclose(INPUTFILE);
    } while (++optind < argc);

    fflush(OUTPUTFILE);
    assert(feof(INPUTFILE));
    fclose(INPUTFILE);
    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}

void compress(char *s)
{
}
