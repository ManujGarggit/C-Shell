#include "headers.h"

void echo(char input[])
{
    char *output = strtok(input, "\t ");
    output = strtok(NULL,"\t ");
    while (output != NULL)
    {
        printf("%s ", output);
        output = strtok(NULL, "\t ");
    }
    printf("\n");
}