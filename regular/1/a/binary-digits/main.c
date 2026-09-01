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

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
/**
 * @brief Reverses char, reason, big endian byte order
 *
 * @param s
 * @return char*
 */
static char *reverse(char *s)
{
    char *rv = (char *)malloc(strlen(s) + 1);

    if (rv == NULL)
    {
        printf("An error occured!\n");
        printf("Error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < strlen(s); i++)
    {
        rv[i] = s[strlen(s) - 1 - i];
    }

    rv[strlen(s)] = '\0';
    return rv;
}

/**
 * @brief Converts a character to a 8 bit binary
 *
 * @param c
 * @return char*
 */
static char *AsciiToBianry(char c)
{
    char *rv = (char *)calloc(9, 1);

    if (rv == NULL)
    {
        printf("An error occured!\n");
        printf("Error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 8; ++i)
    {
        rv[i] = (c & (1 << i) ? '1' : '0');
    }

    return reverse(rv);
}

static void wait_nanosleep(double time)
{
    time_t seconds = (time_t)time;
    long nanoseconds = (long)((time - (double)seconds) * 1e9);
    struct timespec t1 = {seconds, nanoseconds};

    if (nanosleep(&t1, NULL) == -1)
    {
        printf("Error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
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
    FILE *INPUTFILE;
    FILE *OUTPUTFILE = stdout;
    if (OUTPUTFILE == NULL)
    {
        printf("Error (%d): %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int opt;

    bool optionDelay = false;
    double seconds;

    bool optionOutput = false;

    while ((opt = getopt(argc, argv, "d:o::")) != -1)
    {
        switch (opt)
        {
        /**
         *
         * @brief d flag gets the time in nanoseconds, with wait_nanosleep()
         * If the option -d is given, the program should wait the specified amount of seconds after each digit written.
         *   The delay may be specified as an ASCII represented decimal floating point number, e.g. 0.5.
         * also use the strtod() function to convert the time
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
         * @brief If the option -o is given, the output is written to the specified file outfile. Otherwise, the output
         *  is written to stdout
         *
         */
        case 'o':

            /**
             * @todo implement error handling
             *
             */
            optionOutput = true;
            OUTPUTFILE = fopen(optarg, "w+");
            break;

        /**
         * @brief basic error and wrong input handeling
         *
         */
        case ':':
            printf("Error no Arguments given");
            fprintf(stderr, "Usage: %s [-d DELAY] [-o OUTPUTFILE] [FILE]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;

        case '?':
            printf("ERROR: unknown arg %c\n", optopt);
            fprintf(stderr, "Usage: %s [-d DELAY] [-o OUTPUTFILE] [FILE]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // Nasty, mit opind mit >= geht nicht 
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
            INPUTFILE = fopen(argv[optind], "r");

        int c;
        while ((c = fgetc(INPUTFILE)) != EOF)
        {
            if (optionOutput)
                fputs(AsciiToBianry((char)c), OUTPUTFILE);
            else
                fputs(AsciiToBianry((char)c), OUTPUTFILE);

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
