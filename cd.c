#include "headers.h"

void CD(char input[], char output_cwd[])
{

    int i;

    char *temp_cwd = (char *)malloc(sizeof(char) * 500);

    char dummy_dum_input[500];
    for (i = 0; input[i] != '\0'; i++)
    {
        dummy_dum_input[i] = input[i];
    }
    dummy_dum_input[i] = '\0';
    int count_finite = 0;
    char *output_finite = strtok(dummy_dum_input, "\t ");
    output_finite = strtok(NULL, "\t ");
    while (output_finite != NULL)
    {
        count_finite++;
        output_finite = strtok(NULL, "\t ");
    }
    if (count_finite > 1)
    {
        printf("\033[0;31m");
        printf("my bash: cd:too many arguments\n");
        printf("\033[0m");
        return;
    }

    char *output = strtok(input, "\t ");
    output = strtok(NULL, "\t ");
    if (output == NULL)
    {
        if (flag == -1)
            flag = 1;
        temp_cwd = getcwd(temp_cwd, 500);
        for (i = 0; temp_cwd[i] != '\0'; i++)
        {
            output_cwd_prev[i] = temp_cwd[i];
        }
        output_cwd_prev[i] = '\0';
        chdir(output_cwd);
        free(output);
        free(temp_cwd);
        return;
    }

    while (output != NULL)
    {
        if (output[0] == '~')
        {
            int flag_dir = 1;
            int flag_file = 0;
            if (flag == -1)
                flag = 1;
            temp_cwd = getcwd(temp_cwd, 500);
            for (i = 0; temp_cwd[i] != '\0'; i++)
            {
                output_cwd_prev[i] = temp_cwd[i];
            }
            output_cwd_prev[i] = '\0';
            chdir(output_cwd);
            if (output[1] == '/')
            {
                if (output[2] == '\0')
                    return;
                int i;
                char new_string[500];
                for (i = 2; output[i] != '\0'; i++)
                {
                    new_string[i - 2] = output[i];
                }
                new_string[i - 2] = '\0';
                DIR *dir = opendir(new_string);
                if (dir == NULL)
                    flag_dir = 0;
                if (access(new_string, F_OK) == 0)
                    flag_file = 1;
                int retval = chdir(new_string);
                if (retval == -1)
                {
                    // we have to see now the commands of no such file or dir
                    // no such dir
                    if (flag_file == 1)
                    {
                        printf("\033[0;31m");
                        printf("my bash: cd:No such directory\n");
                        printf("\033[0m");
                    }
                    else
                    {
                        printf("\033[0;31m");
                        printf("my bash: cd:No such file or directory\n");
                        printf("\033[0m");
                    }
                }
                closedir(dir);
            }
            else if (output[1] != '\0')
            {
                printf("\033[0;31m");
                printf("my bash: cd:invalid argumnets\n");
                printf("\033[0m");
            }
        }
        else if (strcmp(output, "-") == 0)
        {
            if (flag == -1)
            {
                printf("\033[0;31m");
                printf("my bash: cd: OLDPWD not set\n");
                printf("\033[0m");
                return;
            }
            else
            {
                char dummy_prev[500];
                int i;
                temp_cwd = getcwd(temp_cwd, 500);
                for (i = 0; temp_cwd[i] != '\0'; i++)
                {
                    dummy_prev[i] = temp_cwd[i];
                }
                dummy_prev[i] = '\0';
                for (i = 0; output_cwd_prev[i] != '\0'; i++)
                {
                    printf("%c", output_cwd_prev[i]);
                }
                printf("\n");
                chdir(output_cwd_prev);
                strcpy(output_cwd_prev, dummy_prev);
            }
        }
        else
        {
            int flag_dir = 1;  // if dir exists then 1
            int flag_file = 0; // if file exists then 1
            temp_cwd = getcwd(temp_cwd, 500);
            if (strcmp(output, "..") == 0)
            {
                if (strcmp(temp_cwd, output_cwd) == 0)
                {
                    printf("%s\n", output_cwd);
                }
            }
            for (i = 0; temp_cwd[i] != '\0'; i++)
            {
                output_cwd_prev[i] = temp_cwd[i];
            }
            output_cwd_prev[i] = '\0';
            DIR *dir = opendir(output);
            if (dir == NULL)
                flag_dir = 0;
            if (access(output, F_OK) == 0)
                flag_file = 1;
            int retval = chdir(output);
            if (retval == -1)
            {
                // we have to see now the commands of no such file or dir
                // no such dir
                if (flag_file == 1)
                {
                    printf("\033[0;31m");
                    printf("my bash: cd:No such directory\n");
                    printf("\033[0m");
                }
                else
                {
                    printf("\033[0;31m");
                    printf("my bash: cd:No such file or directory\n");
                    printf("\033[0m");
                }
            }
            else
                flag = 1;
            closedir(dir);
        }
        output = strtok(NULL, "\t ");
    }
    free(output);
    free(temp_cwd);
}