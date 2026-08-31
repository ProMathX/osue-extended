/**
 * @file main.c
 * @author kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief  Write a C-program that visualizes data in its binary represnation.
 * @version 0.1
 * @date 2026-08-28
 *
 * @copyright Copyright (c) 2026
 *
 * Allways follo the UNIX-Philosophy modularise everything make small functions to complete the big one, divide et
 * impera
 *
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define error(msg, program_name)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s\n", msg);                                                                                           \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        fprintf(stderr, "Usage: %s [-d DELAY] [-o OUTPUTFILE] [FILE]\n", program_name);                                \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

/**                                                                                                    \
 * @brief Reverses char, reason, big endian byte order                                                 \
 *                                                                                                     \
 * @param s                                                                                            \
 * @return char*                                                                                       \
 */
void reverse(char *s, size_t first, size_t last)
{
    if (first >= last)
    {
        return;
    }

    char tmp = s[first];
    s[first] = s[last];
    s[last] = tmp;

    reverse(s, first + 1, last - 1);
}

/**
 * @brief Converts a character to a 8 bit binary
 *
 * @param c
 * @return char*
 */
static char *AsciiToBinary(char c)
{
    char *rv = (char *)calloc(1, 9);

    if (rv == NULL)
        error("Failed to allocate memory", "AsciiToBinary(char c)");

    for (int i = 0; i < 8; ++i)
    {
        rv[i] = (c & (1 << i) ? '1' : '0');
    }

    size_t last = strlen(rv) - 1;

    reverse(rv, 0, last);

    return rv;
}

static void wait_nanosleep(double time)
{
    time_t seconds = (long)time;
    time_t nanoseconds = (long)((time - (double)seconds) * 1e9);
    struct timespec t1 = {seconds, nanoseconds};

    if (nanosleep(&t1, NULL) == -1)
        error("failed to construct the timer", "wait_nanosleep");
}

/**
 * @brief main function
 * @todo  implement the command line parsing big question mark in my mind
 * @param argc
 * @param argv
 * @return int EXIT_SUCCESS || EXIT_FAILURE
 */
int main(int argc, char **argv)
{
    FILE *INPUTFILE = stdin;
    int opt;

    bool optionDelay = false;
    double seconds;

    // bool optionOutput = false;
    FILE *OUTPUTFILE = stdout;

    while ((opt = getopt(argc, argv, "d:o:")) != -1)
    {
        switch (opt)
        {
        /**
         *
         * @brief d flag gets the time in nanoseconds, with wait_nanosleep()
         * If the option -d is given, the program should wait the specified amount of seconds after each digit
         * written. The delay may be specified as an ASCII represented decimal floating point number, e.g. 0.5. also
         * use the strtod() function to convert the time
         */
        case 'd':
            /**
             * @todo write error handling
             *
             */
            optionDelay = true;
            seconds = strtof(optarg, NULL);
            break;

        /**
         * @brief If the option -o is given, the output is written to the specified file outfile. Otherwise, the
         * output is written to stdout
         *
         */
        case 'o':

            // optionOutput = true;
            OUTPUTFILE = fopen(optarg, "w+");
            if (OUTPUTFILE == NULL)
                error("failed to allocate the OUTPUTFILE", "main()");
            break;

        /**
         * @brief basic error and wrong input handeling
         *
         */
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
            INPUTFILE = stdin;

        else
            INPUTFILE = fopen(argv[optind], "r+");

        if (INPUTFILE == NULL)
            error("failed to allocate the INPUTFILE", "main()");

        int c;
        while ((c = fgetc(INPUTFILE)) != EOF)
        {
            char *binary = AsciiToBinary((char)c);
            fputs(binary, OUTPUTFILE);
            free(binary);

            if (optionDelay)
            {
                fflush(OUTPUTFILE);
                wait_nanosleep(seconds);
            }
        }
        fprintf(OUTPUTFILE, "\n");
        fclose(INPUTFILE);
    } while (++optind < argc);

    fclose(OUTPUTFILE);

    exit(EXIT_SUCCESS);
}
