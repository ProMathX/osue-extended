#include <assert.h>
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
        fprintf(stderr, "Usage: %s [-i] [-o outfile] keyword [file]\n", program_name);                                 \
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
    char *line_buff = strdup(line);
    char *keyword_buff = strdup(keyword);

    if (Ignore_characterChase)
    {
        for (size_t i = 0; line[i]; i++)
        {
            line_buff[i] = (char)tolower(line[i]);
        }

        for (size_t i = 0; keyword[i]; i++)
        {
            keyword_buff[i] = (char)tolower(keyword[i]);
        }
    }

    bool __bool = (strstr(line_buff, keyword_buff) != NULL);
    free(line_buff);
    free(keyword_buff);
    return __bool;
}

int grep(IO *files, options_t *opts, char *keyword)
{
    char *line = NULL;
    size_t line_buffer_len = 0;
    ssize_t read_size = 0;

    while ((read_size = getline(&line, &line_buffer_len, files->INPUT1)))
    {
        if (read_size > 0 && line[read_size - 1] == '\n')
            line[read_size - 1] = '\0';

        if (strcontain(line, keyword, opts->characterChase))
        {
            fprintf(files->OUPUT, "%s\n", line);
            // fflush(files->OUPUT);
        }
    }

    free(line);
    line_buffer_len = 0;
    return -1;
}

int main(int argc, char **argv)
{
    FILE *INPUT, *OUTPUTFILE = stdout;

    if (OUTPUTFILE == NULL)
    {
        error("Failed to allocate OUPUTFILE", "OUTPUTFILE");
        exit(EXIT_FAILURE);
    }

    int option = 0;
    bool characterChase = false;
    while ((option = getopt(argc, argv, "io::")) != -1)
    {
        switch (option)
        {
        case 'i':
            characterChase = true;
            break;
        case 'o':
            OUTPUTFILE = fopen(optarg, "r+");
            break;
        case ':':
            error("Error no Arguments given", argv[0]);
            break;

        case '?':
            error("unknown arg", argv[0]);
            break;
        }
    }

    if (optind >= argc)
    {
        error("missing keywork", "mygrep");
        exit(EXIT_FAILURE);
    }

    char *keyword = argv[optind++];
    options_t opts = {.characterChase = characterChase};
    if (optind == argc)
    {
        IO files = {.INPUT1 = stdin, .OUPUT = OUTPUTFILE};
        grep(&files, &opts, keyword);
    }
    else
        do
        {
            INPUT = fopen(argv[optind], "r");

            if (INPUT == NULL)
                error("failed to allocate the INPUTFILE", argv[0]);

            IO files = {.INPUT1 = INPUT, .OUPUT = OUTPUTFILE};

            grep(&files, &opts, keyword);
            fclose(INPUT);

        } while (++optind < argc);

    fclose(OUTPUTFILE);
    exit(EXIT_SUCCESS);
}