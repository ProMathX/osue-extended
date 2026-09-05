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

typedef struct IO
{

    FILE *INPUT1;
    FILE *INPUt2;
    FILE *OUPUT;

} IO;

typedef struct LINE
{

    char *s1;
    char *s2;
    bool characterChase;

} LINE;

int expand(IO *FILES)
{
    LINE line_settings = {.s1 = NULL, .s2 = NULL, .characterChase = false};

    return 0;
}

int main(int argc, char **argv)
{

    exit(EXIT_SUCCESS);
}