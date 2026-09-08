/**
 * @file myexpand.c
 * @author kernrkaftwerk (kernrkaftdev@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2026-09-03
 *
 * @copyright Copyright (c) 2026
 *
 */

/**
 * @details I am exhausted, they are always the same, so this is kinda janky
 made mainly for the idea, it works, but no error tests etc implented.
The exercise sheet was horrible to read and made no sense at all, so asked Claude to explain the exercise, no LLM code
was used!

 *
 */

#include <assert.h>
#include <bits/getopt_core.h>
#include <bits/posix2_lim.h>
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
        fprintf(stderr, "Usage: %s [-o outfile] [file]\n", program_name);                                              \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

typedef struct IO
{
    FILE *INPUT1;
    FILE *OUPUT;
} IO;

typedef struct options_t
{
    bool tabStop;
    uint16_t tab;
    bool outfile;
} options_t;

int p(int x, int tabstop)
{
    int p_calc = tabstop * ((x / tabstop) + 1);
    return (p_calc - x);
}

int tabsCompress(IO *FILES, int tabstop)
{
    int c;
    volatile int n = 0;
    int volatile pos = 0;

    if (FILES->INPUT1 == NULL)
    {
        error("INPUT EMTPY ERROR!", "tabsCompress()");
        return -1;
    }

    while ((c = getc(FILES->INPUT1)) != EOF)
    {

        if (c == '\t')
        {
            n = p(pos, tabstop); // default tabstop 8
            fprintf(FILES->OUPUT, "%*c", n, ' ');
            pos += n;
        }
        else if (c == '\n')
        {
            fputc(c, FILES->OUPUT);
            pos = 0;
        }
        else
        {
            fputc(c, FILES->OUPUT);
            pos++;
        }
    }

    // 0 for success
    return 0;
}

int main(int argc, char **argv)
{
    // LINE line_settings = {.s1 = NULL, .s2 = NULL, .characterChase = false};

    FILE *INPUT, *OUTPUTFILE = stdout;

    if (OUTPUTFILE == NULL)
        error("Failed to allocate OUPUTFILE", "OUTPUTFILE");

    int option;

    bool tabStop = false;
    bool optionOutput = true;
    int tabs = 8;

    while ((option = getopt(argc, argv, "to:")) != -1)
    {
        switch (option)
        {
        case 't':
            tabStop = true;
            tabs = (int)strtod(optarg, NULL);
            break;
        case 'o':
            optionOutput = true;
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
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }
    do
    {
        if ((argc - optind) == 0)
            INPUT = stdin;
        else
            INPUT = fopen(argv[optind], "r+");

        if (INPUT == NULL)
            error("failed to allocate the INPUTFILE", "main()");

        IO files = {.INPUT1 = INPUT, .OUPUT = OUTPUTFILE};
        tabsCompress(&files, tabs);

        fprintf(OUTPUTFILE, "\n");
        fclose(INPUT);
        fflush(OUTPUTFILE);
    } while (++optind < argc);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}