/**
 * @file main.c
 * @author Kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2026-09-01
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
        fprintf(stderr, "Usage: %s [-i ignore case sensitivity] [-s ignore whitespace] [-o OUTPUTFILE] [FILE]\n",      \
                program_name);                                                                                         \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

/**
 * @brief checks simply the program if the given string is a palindrome stolen from geeks for geeks
 *
 * @param s input string
 * @return true if the string is a palindrome
 * @return false if the string is not a palindrome
 */
bool isPalindrome(const char *s, bool ignoreWhiteSpace, bool ignoreCaseSensitivity)
{
    if (s == NULL)
    {
        error("Invalid String", "isPalindrome()");
    }

    int first, last;

    first = 0;
    last = (int)strlen(s) - 1;

    while (first < last)
    {
        if (ignoreWhiteSpace && isspace((unsigned char)s[first]))
        {
            first++;
            continue;
        }

        if (ignoreWhiteSpace && isspace((unsigned char)s[last]))
        {
            last--;
            continue;
        }

        char __a = s[first];
        char __b = s[last];

        if (ignoreCaseSensitivity)
        {
            __a = (char)tolower((unsigned char)s[first]);
            __b = (char)tolower((unsigned char)s[last]);
        }
        if (__a != __b)
        {
            // printf("%s is not a palindrome", s);
            return false;
        }

        first++;
        last--;
    }
    // printf("%s is a palindrome\n", s);
    return true;
}

/**
 * @brief main function
 *
 * @param argc argument counter
 * @param argv argument vector
 * @return int
 */

int main(int argc, char **argv)
{
    FILE *INPUTFILE = stdin;
    FILE *OUTPUTFILE = stdout;
    /**
     * @brief Specify output
     *
     */
    bool optionOutput = false; // case o

    /**
     * @brief  If the option -i is given, the program shall not differentiate between lower and upper case letters
     *
     */
    bool ignoreWhiteSpace = false; // case i

    /**
     * @brief The option -s causes the program to ignore whitespaces when checking whether a line is a palindrom
     *
     */
    bool ignoreCaseSensitivity = false; // case s

    int c;
    /**
     * @todo implement the cases
     */
    while ((c = getopt(argc, argv, "iso:")) != -1)
    {

        switch (c)
        {
        case 'i':
            ignoreWhiteSpace = true;

            break;

        case 's':
            ignoreCaseSensitivity = true;

            break;

        case 'o':
            optionOutput = true;

            OUTPUTFILE = fopen(optarg, "w");

            if (OUTPUTFILE == NULL)
                error("No output, error", argv[0]);
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

    do
    {
        if ((argc - optind) == 0)
            INPUTFILE = stdin;
        else
            INPUTFILE = fopen(argv[optind], "r");

        if (INPUTFILE == NULL)
            error("Couldnt allocate input", "main");
        char *line = NULL;
        size_t line_buffer_len = 0;
        ssize_t read_size = 0;

        while ((read_size = getline(&line, &line_buffer_len, INPUTFILE)) != -1)
        {
            line[read_size - 1] = '\0';
            if (isPalindrome(line, ignoreWhiteSpace, ignoreCaseSensitivity))
            {
                printf("%s is a palindrome\n", line);
                if (OUTPUTFILE != stdout)
                {
                    fputs(line, OUTPUTFILE);
                    fprintf(OUTPUTFILE, "is a palindrome\n");
                }
            }
            else
            {
                printf("%s is not a palindrome\n", line);
                if (OUTPUTFILE != stdout)
                {
                    fputs(line, OUTPUTFILE);
                    fprintf(OUTPUTFILE, "is not a palindrome\n");
                }
            }
            // fprintf(OUTPUTFILE, "\n");
        }
        fflush(OUTPUTFILE);
        free(line);
        line = NULL;
        line_buffer_len = 0;
        assert(feof(INPUTFILE));
        fclose(INPUTFILE);
    } while (++optind < argc);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}