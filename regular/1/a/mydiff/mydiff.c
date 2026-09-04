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
#include <bits/posix2_lim.h>
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
 * @param s1_tobecompared string literal to line-> from
 * @param line string literal to be compared with
 * @param lineNumber line of current line
 * @param OUTPUTFILE  OUTPUTFILE (stdout)
 * @param caseINSensitiv bool acknowledging case sentivity
 */

typedef struct OPTIONS_LINE
{
    char *s1;
    char *s2;
    bool characterChase;
} OPTIONS_LINE;

typedef struct IO
{
    FILE *__MAIN_INPUTFILE;
    FILE *__MAIN_COMPAREFILE;
    FILE *OUTPUTFILE;

} IO;

void compareLines(OPTIONS_LINE *line, IO *files, int lineNumber)
{
    if (files->OUTPUTFILE == NULL)
        error("No Output specified", "compareLines()");

    if (lineNumber <= 0)
        error("Line number should be greater than 0", "compareLines()");
    char __a, __b;

    int diffCount = 0;

    /**
     * @brief  Compare two lines character-by-character and count mismatches.
     *
     * @note   BUG  HIT: the loop bound was taken from s1 only
     *         (strlen(s1) / until s1's '\n'), while line->.s2 was indexed with the
     *         same index i.  If line->.s2 is shorter than s1, this reads past the
     *         end of line->.s2 -- UDB.
     *
     *         Valgrind will NOT reliably catch this: it only flags an
     *         out-of-bounds line-> if the byte line-> happens to fall outside
     *         the allocated heap region.  A clean valgrind run is evidence
     *         of "no leaks", not evidence of "bounds-safe".
     */
    for (size_t i = 0; i < strlen(line->s1) && line->s1[i] != '\n' && i < strlen(line->s2) && line->s2[i] != '\n'; ++i)
    {
        __a = line->s1[i];
        __b = line->s2[i];

        if (line->characterChase)
        {
            char a = (char)tolower(__a), b = (char)tolower(__b);

            if (a != b)
                diffCount++;
        }
        else if (__a != __b)
            diffCount++;
    }
    if (diffCount > 0)
        fprintf(files->OUTPUTFILE, "Line: %d characters: %d\n", lineNumber, diffCount);
}

/**
 * @brief diffs the input
 * @param files
 * @param line
 * @return int
 */
int diffInput(IO *files, OPTIONS_LINE *line)
{
    if (files->__MAIN_INPUTFILE == NULL)
    {
        error("INPUTFILE1 emtpy", "diffInput()");
        return -1;
    }
    if (files->__MAIN_COMPAREFILE == NULL)
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

    while (((read_MainInput = getline(&line_MainInput, &line_buffer_len_MainInput, files->__MAIN_INPUTFILE)) != -1) &&
           (read_CompareInput =
                getline(&line_CompareInput, &line_buffer_len_CompareInput, files->__MAIN_COMPAREFILE)) != -1)
    {
        line->s1 = line_MainInput;
        line->s2 = line_CompareInput;
        int cmp = line->characterChase ? strncasecmp(line_MainInput, line_CompareInput, (size_t)read_MainInput)
                                       : strcmp(line_MainInput, line_CompareInput);

        if (cmp != 0)
        {
            compareLines(line, files, lineNumber);
            lineNumber++;
        }
        else
            lineNumber++;
        // fprintf(stderr, "line1: %s | line2: %s\n", line_MainInput, line_CompareInput);
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
    bool CharacterParseSetting = false, optionOutput = false;
    int option;
    while ((option = getopt(argc, argv, "io:")) != -1)
    {
        switch (option)
        {
        case 'i':
            CharacterParseSetting = true;
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

        IO __TEST_FILES = {.__MAIN_INPUTFILE = INPUTFILE1, .__MAIN_COMPAREFILE = INPUTFILE2, .OUTPUTFILE = OUTPUTFILE};

        OPTIONS_LINE line_settings = {.s1 = NULL, .s2 = NULL, .characterChase = CharacterParseSetting};

        diffInput(&__TEST_FILES, &line_settings);

        fflush(OUTPUTFILE);
        //  assert(feof(INPUTFILE1));
        //  assert(feof(INPUTFILE2));
        fclose(INPUTFILE1);
        fclose(INPUTFILE2);
    } while (optind < argc);
    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}