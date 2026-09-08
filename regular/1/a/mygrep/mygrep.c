#include <assert.h>
#include <bits/getopt_core.h>
#include <bits/posix2_lim.h>
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
        fprintf(stderr, "Usage: %s[-i] [-o outfile] keyword [file]\n", program_name);                                  \
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
    char *user_input_string;
    char *strings_to_be_checked;
    bool characterChase;

} options_t;

bool strcontain(char *line, char *keyword, bool Ignore_characterChase)
{
    if (Ignore_characterChase)
    {
        for (size_t i = 0; line[i]; i++)
        {
            line[i] = (char)tolower(line[i]);
        }

        for (size_t i = 0; keyword[i]; i++)
        {
            keyword[i] = (char)tolower(keyword[i]);
        }
    }

    return (strstr(line, keyword) != NULL);
}

int grep(IO *files, options_t *opts, char *keyword)
{
    char *line = NULL;
    size_t line_buffer_len = 0;
    ssize_t read_size = 0;

    while ((read_size = getline(&line, &line_buffer_len, files->INPUT1)))
    {
        line[read_size - 1] = '\0';

        if (strcontain(line, keyword, opts->characterChase))
        {
            fprintf(files->OUPUT, "%s", line);
        }
    }

    free(line);
    line_buffer_len = 0;
    return -1;
}

int main(int argc, char **argv)
{
    FILE *INPUT = stdin, *OUTPUTFILE = stdout;

    if (OUTPUTFILE == NULL)
        error("Failed to allocate OUPUTFILE", "OUTPUTFILE");

    int option = 0;
    bool characterChase = false;
    while ((option = getopt(argc, argv, "io")) != -1)
    {
        switch (option)
        {
        case 'i':
            characterChase = true;
            break;
        case 'o':
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
            error("failed to allocate the INPUTFILE", "./mygep");

        IO files = {.INPUT1 = INPUT, .OUPUT = OUTPUTFILE};
        options_t opts = {.user_input_string = NULL, .strings_to_be_checked = NULL, .characterChase = false};

        grep(&files, &opts, argv[optind]);
        fflush(files.OUPUT);

    } while (++optind < argc);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}