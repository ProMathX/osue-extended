/**
 * @file mydiff.c
 * @author kernrkaftwerk (kernrkaftdev@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2026-09-03
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

/**
 * @brief compares 2 Lines
 *
 * @param s1_tobecompared string literal to read from
 * @param s2 string literal to be compared with
 * @param lineNumber line of current line
 * @param OUTPUTFILE  OUTPUTFILE (stdout)
 * @param caseINSensitiv bool acknowledging case sentivity
 */
void compareLines(char *s1_tobecompared, char *s2, int lineNumber, FILE *OUTPUTFILE, bool caseINSensitiv)
{
    if (OUTPUTFILE == NULL)
        error("No Output specified", "compareLines()");

    if (lineNumber <= 0)
        error("Line number should be greater than 0", "compareLines()");
    char __a, __b;

    int diffCount = 0;

    /**
     * @brief  Compare two lines character-by-character and count mismatches.
     *
     * @note   BUG I ACTUALLY HIT: the loop bound was taken from s1 only
     *         (strlen(s1) / until s1's '\n'), while s2 was indexed with the
     *         same index i.  If s2 is shorter than s1, this reads past the
     *         end of s2 -- undefined behavior.
     *
     *         Valgrind will NOT reliably catch this: it only flags an
     *         out-of-bounds read if the byte read happens to fall outside
     *         the allocated heap region.  A clean valgrind run is evidence
     *         of "no leaks", not evidence of "bounds-safe".
     *
     *         Lesson: when iterating two buffers of independent, unrelated
     *         length in lockstep, the loop bound must be derived from BOTH,
     *         e.g. i < min(len1, len2), not from one of them alone.
     */
    for (size_t i = 0; i < strlen(s1_tobecompared) && s1_tobecompared[i] != '\n' && i < strlen(s2) && s2[i] != '\n';
         ++i)
    {
        __a = s1_tobecompared[i];
        __b = s2[i];

        if (caseINSensitiv)
        {
            char a = (char)tolower(__a), b = (char)tolower(__b);

            if (a != b)
                diffCount++;
        }
        else if (__a != __b)
        {
            diffCount++;
        }
    }
    if (diffCount > 0)
        fprintf(OUTPUTFILE, "Line: %d characters: %d\n", lineNumber, diffCount);
}

/**
 * @brief
 *
 * @param MainInput INPUTFILE1
 * @param CompareInput  INPUTFILE2
 * @param OUTPUTFILE OUTPUTFILE (standard stdout)
 * @param ignoreCaseSensitivity specified with the -i flag, ignores case sensitivity
 * @return int 0 for success -1 for no success
 */
int diffInput(FILE *MainInput, FILE *CompareInput, FILE *OUTPUTFILE, bool ignoreCaseSensitivity)
{
    if (MainInput == NULL)
    {
        error("INPUTFILE emtpy", "diffInput()");
        return -1;
    }
    if (CompareInput == NULL)
    {
        error("INPUTFILE 2 empty", "diffinput");
        return -1;
    }

    char *line_MainInput = NULL;
    size_t line_buffer_len_MainInput = 0;
    ssize_t read_MainInput = 0;

    char *line_CompareInput = NULL;
    size_t line_buffer_len_CompareInput = 0;
    ssize_t read_CompareInput = 0;

    int lineNumber = 1;

    while (((read_MainInput = getline(&line_MainInput, &line_buffer_len_MainInput, MainInput)) != -1) &&
           (read_CompareInput = getline(&line_CompareInput, &line_buffer_len_CompareInput, CompareInput)) != -1)
    {
        int cmp = ignoreCaseSensitivity ? strncasecmp(line_MainInput, line_CompareInput, (size_t)read_MainInput)
                                        : strcmp(line_MainInput, line_CompareInput);

        // fprintf(OUTPUTFILE, "%d %d\n", (int)read_MainInput, cmp);

        if (cmp != 0)
        {
            compareLines(line_MainInput, line_CompareInput, lineNumber, OUTPUTFILE, ignoreCaseSensitivity);
            lineNumber++;
        }
        else
            lineNumber++;
    }

    line_buffer_len_CompareInput = 0, read_CompareInput = 0, line_buffer_len_MainInput = 0, read_MainInput = 0;
    free(line_MainInput);
    free(line_CompareInput);
    return 0;
}

int main(int argc, char **argv)
{
    FILE *INPUTFILE1, *INPUTFILE2;
    FILE *OUTPUTFILE = stdout;
    if (OUTPUTFILE == NULL)
        error("Failed to allocate OUPUTFILE", "OUTPUTFILE");
    bool caseInsensitiv = false, optionOutput = false;
    int option;
    while ((option = getopt(argc, argv, "io:")) != -1)
    {
        switch (option)
        {
        case 'i':
            caseInsensitiv = true;
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

    do
    {
        if ((argc - optind) == 0)
            error("not enough input ", "main()");
        else
        {
            INPUTFILE1 = fopen(argv[optind], "r");
            INPUTFILE2 = fopen(argv[optind + 1], "r");
        }
        optind += 2;

        assert(INPUTFILE1 != NULL);

        diffInput(INPUTFILE1, INPUTFILE2, OUTPUTFILE, caseInsensitiv);

        fflush(OUTPUTFILE);
        assert(feof(INPUTFILE1));
        // assert(feof(INPUTFILE2));
        fclose(INPUTFILE1);
        fclose(INPUTFILE2);
    } while (optind < argc);
    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}