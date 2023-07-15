#include "headers.h"

void HISTORY(char *homedir)
{
    // it will output 10 commands
    // store 20 commands
    FILE *fptr;
    struct passwd *pw = getpwuid(getuid());
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    chdir(homedir);
    fptr = fopen("history.txt", "r");
    char *line;
    size_t len = 0;
    char contents[100][500];
    int it_con = 0;
    while ((getline(&line, &len, fptr) != -1))
    {
        strcpy(contents[it_con++], line);
    }
    fclose(fptr);
    chdir(temp_cwd);
    if (it_con <= 10)
    {
        for (int i = 0; i < it_con; i++)
        {
            printf("%s", contents[i]);
        }
    }
    else if (it_con > 10)
    {
        for (int i = it_con - 10; i < it_con; i++)
        {
            printf("%s", contents[i]);
        }
    }
}