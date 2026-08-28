/**
 * @file main.c
 * @author kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief  Write a C-program that visualizes data in its binary represnation.
 * @version 0.1
 * @date 2026-08-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse(char *s)
{
    char *rv = (char *)malloc(sizeof(char)*strlen(s));

    for(size_t i = 0; i < strlen(s); i++)
    {
        rv[i] = s[strlen(s)-1-i];
    }

    return rv;
}

char *AsciiToBianry(char c)
{
   char *rv = (char *)malloc (sizeof (char) * 9);

   for (int i = 0; i < 8; ++i)
   {
    rv[i] = (c & (1 << i) ? '1' : '0');
   }

   return reverse(rv);
}

int main(int argc, char **argv)
{
    printf("%s\n",AsciiToBianry('a'));
    return 0;
}

