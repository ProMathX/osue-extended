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
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define ASCII 256
#define error(msg, program_name)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s\n", msg);                                                                                           \
        fprintf(stderr, "Usage: %s [-o outfile] [file]\n", program_name);                                              \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

void compress(FILE *INPUTFILE, FILE *OUTPUTFILE)
{
    if (INPUTFILE == NULL)
        error("invalid input, empty String", "compress()");

    if (OUTPUTFILE == NULL)
        error("Ouput is not specified", "compress()");

    int readChar;
    int count = 1;
    char currenChar = (char)fgetc(INPUTFILE);

    while ((readChar = fgetc(INPUTFILE)) != EOF)
    {
        if (readChar == currenChar)
        {
            count++;
        }

        else
        {
            fprintf(OUTPUTFILE, "%c%d", currenChar, count);
            currenChar = (char)readChar;
            count = 1;
        }
    }
    fprintf(OUTPUTFILE, "%c%d", currenChar, count);
}

int main(int argc, char **argv)
{

    FILE *INPUTFILE = stdin;
    assert(INPUTFILE != NULL);
    FILE *OUTPUTFILE = stdout;
    assert(OUTPUTFILE != NULL);

    // bool optionOutput = false;

    int option;
    while ((option = getopt(argc, argv, "o:")) != -1)
    {
        switch (option)
        {
        case 'o':
            // optionOutput = true;
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

        compress(INPUTFILE, OUTPUTFILE);

        fflush(OUTPUTFILE);
        assert(feof(INPUTFILE));
        fclose(INPUTFILE);
    } while (++optind < argc);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}
