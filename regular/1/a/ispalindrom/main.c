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
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        fprintf(stderr, "Usage: %s [-d DELAY] [-o OUTPUTFILE] [FILE]\n", program_name);                                \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

/**
 * @brief easily reverse the string
 *
 * @param s input string
 * @return char* returns reversed string
 */
char *strrev(const char *s)
{
    char *rv = (char *)malloc(sizeof(char) * (strlen(s) + 1));

    if (rv == NULL)
    {
        error("Couldn't allocate memory", "strrev()");
    }

    for (size_t i = 0; i < strlen(s); i++)
    {
        rv[i] = s[strlen(s) - 1 - i];
    }

    rv[strlen(s)] = '\0';

    return rv;
}

/**
 * @brief checks simply the program if the given string is a palindrome
 *
 * @param s input string
 * @return true if the string is a palindrome
 * @return false if the string is not a palindrome
 */
bool isPalindromeNormal(const char *s)
{
    if (s == NULL)
    {
        error("Invalid String", "isPalindrome()");
    }
    char *string_check = strrev(s);
    bool rv = strcmp(s, string_check);
    free(string_check);
    return rv;
}

bool isPalindromeIgnore(const char *s)
{
    if (s == NULL)
    {
        error("Invalid String", "isPalindrome()");
    }

    char *__lower =(char *)malloc(sizeof(s)+1);

    for (size_t i = 0; s[i]; i++)
    {
        __lower[i] = (char)tolower((unsigned char)s[i]);
    }

    char *string_check = strrev(__lower);
    bool rv = strcmp(s, string_check);
    free(string_check);
    return rv;
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
    // Test 1
    printf("%s\n", isPalindromeNormal("Hello") ? "true" : "false");

    FILE *INPUTFILE = stdin;
    FILE *OUTPUTFILE = stdout;

    int option;
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
     * @todo extended the user flags
     *
     */

    while ((c = getopt(argc, argv, "iso:")) != -1)
    {

        switch (option)
        {
        case 'i':
            ignoreWhiteSpace = true;

            break;

        case 's':
            ignoreCaseSensitivity = true;
            break;

        case 'o':
            optionOutput = true;
            break;
        }
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t read;

    if (argc != 2)
    {
        error("to many arguments", argv[0]);
    }

    exit(EXIT_SUCCESS);
}