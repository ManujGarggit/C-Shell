#include "headers.h"

void PWD(char output_cwd[], char hostname[], char username[])
{
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    for (int i = 0; temp_cwd[i] != '\0'; i++)
    {
        printf("%c", temp_cwd[i]);
    }
    printf("\n");
}