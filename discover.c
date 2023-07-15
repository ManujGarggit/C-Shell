#include "headers.h"

void print_relative(char file[], char target[], char target_dir_abs[], int flag_file_or_dir)
{
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    int size_root = strlen(target_dir_abs);
    if (strcmp(temp_cwd, target_dir_abs) == 0)
    {
        if (flag_file_or_dir == 1)
        {
            printf("%s\n", target);
        }
        else
        {
            printf("%s", target);
            printf("/%s\n", file);
        }
    }
    else
    {
        printf("%s", target);
        for (int i = size_root; temp_cwd[i] != '\0'; i++)
        {
            printf("%c", temp_cwd[i]);
        }
        if (flag_file_or_dir == 1)
        {
            printf("\n");
        }
        else
        {
            printf("/%s\n", file);
        }
    }
    free(temp_cwd);
}

int print_discover(char comp[], char file_dir[], char target[], char target_dir_abs[], int flag_d, int flag_f, int flag_file_or_dir, int is_some_file)
{   
    // comp gives us the content
    // file_dir gives us the to comp with
    // target gives us the input in target dir
    // abs path
    // curr dir abs path
    // flag_file_or_dir == 1 for dir and 0 for file
    count_before++;
    if (flag_d == 1 && flag_f == 0)
    {
        if (is_some_file == 1)
        {
            if (flag_file_or_dir == 1)
            {
                if (strcmp(comp, file_dir) == 0)
                {
                    print_relative(comp, target, target_dir_abs, flag_file_or_dir);
                    count_discover++;
                }
            }
        }
        else
        {
            if (flag_file_or_dir == 1)
            {
                print_relative(comp, target, target_dir_abs, flag_file_or_dir);
                count_discover++;
            }
        }
    }
    else if (flag_d == 0 && flag_f == 1)
    {
        if (is_some_file == 1)
        {
            if (flag_file_or_dir == 0)
            {
                if (strcmp(comp, file_dir) == 0)
                {
                    print_relative(comp, target, target_dir_abs, flag_file_or_dir);
                    count_discover++;
                }
            }
        }
        else
        {
            if (flag_file_or_dir == 0)
            {
                print_relative(comp, target, target_dir_abs, flag_file_or_dir);
                count_discover++;
            }
        }
    }
    else
    {
        if (is_some_file == 1)
        {
            if (strcmp(comp, file_dir) == 0)
            {
                print_relative(comp, target, target_dir_abs, flag_file_or_dir);
                count_discover++;
            }
        }
        else
        {
            print_relative(comp, target, target_dir_abs, flag_file_or_dir);
            count_discover++;
        }
    }
}

void discoverLoop(char file_dir[], char target[], char target_dir_abs[], int flag_d, int flag_f, int flag_file_dir)
{
    struct dirent *d;
    DIR *dh = opendir(".");
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    while ((d = readdir(dh)) != NULL)
    {
        if (d->d_name[0] == '.')
            continue;
        int retval = chdir(d->d_name);
        if (retval == -1)
        {
            // that means that is a file
            print_discover(d->d_name, file_dir, target, target_dir_abs, flag_d, flag_f, 0, flag_file_dir);
        }
        else
        {
            // we have already gone into the directory
            print_discover(d->d_name, file_dir, target, target_dir_abs, flag_d, flag_f, 1, flag_file_dir);
            discoverLoop(file_dir, target, target_dir_abs, flag_d, flag_f, flag_file_dir);
            chdir(temp_cwd);
        }
    }
    closedir(dh);
    free(temp_cwd);
}

void DISCOVER(char dummy_input[], char output_cwd[])
{

    count_discover = 0;
    count_before = 0;
    int flag_d = 0;
    int flag_f = 0;
    char *output = strtok(dummy_input, "\t ");
    output = strtok(NULL, "\t ");
    int flag_file_dir = 0; // it stores whether the find dir or file is given or not
    int flag_tar_dir = 0;  // it stores whether the target dir is given or not
    char file_dir[500];    // it stores the name of the file or directory which we have to find
    char dummy[500];
    char target[500]; // it stores the target dir given
    int count_quotes = 0;
    int count_tar = 0;

    if (output == NULL)
    {   
        dummy[0] = '.';
        dummy[1] = '\0';
        flag_tar_dir = 1;
        strcpy(target, dummy);
    }

    while (output != NULL)
    {
        if (output[0] == '-')
        {
            if (output[1] == '\0')
            {
                printf("\033[0;31m");
                printf("my bash: discover:invalid arguments\n");
                printf("\033[0m");
                return;
            }
            else if (output[1] == 'd')
                flag_d = 1;
            else if (output[1] == 'f')
                flag_f = 1;
            else if (output[2] != '\0')
            {
                printf("\033[0;31m");
                printf("my bash: discover:invalid arguments\n");
                printf("\033[0m");
                return;
            }
            else
            {
                printf("\033[0;31m");
                printf("my bash: discover:invalid arguments\n");
                printf("\033[0m");
                return;
            }
        }
        else if (output[0] == '"')
        {
            int i;
            for (i = 1; output[i] != '"'; i++)
                file_dir[i - 1] = output[i];
            file_dir[i - 1] = '\0';
            flag_file_dir = 1;
            count_quotes++;
        }
        else
        {
            if (output[0] == '~')
            {
                int len = strlen(output);
                char temp[500];
                char temp_dum[500];
                int i, j;
                if (output[len - 1] == '/')
                    output[len - 1] = '\0';
                for (i = 1; output[i] != '\0'; i++)
                    temp[i - 1] = output[i];
                temp[i - 1] = '\0';
                strcpy(temp_dum, output_cwd);
                strcat(temp_dum, temp);
                strcpy(dummy, temp_dum);
                strcpy(target, output);
            }
            else
            {
                int len = strlen(output);
                strcpy(dummy, output);
                if (output[len - 1] == '/')
                    output[len - 1] = '\0';
                strcpy(target, output);
            }
            flag_tar_dir = 2;
            count_tar++;
        }
        output = strtok(NULL, "\t ");
    }

    if (count_quotes > 1)
    {
        printf("\033[0;31m");
        printf("my bash: discover:too many files to find\n");
        printf("\033[0m");
        return;
    }

    if (count_tar > 1)
    {
        printf("\033[0;31m");
        printf("my bash: discover:too many target directories\n");
        printf("\033[0m");
        return;
    }

    if (flag_d == 0 && flag_f == 0)
    {
        flag_d = 1;
        flag_f = 1;
    }

    if(flag_tar_dir==0){
        dummy[0] = '.';
        dummy[1] = '\0';
        flag_tar_dir = 1;
        strcpy(target, dummy);
    }

    char comp[500];
    char *temp_dir = (char *)malloc(sizeof(char) * 500);
    char *target_dir_abs = (char *)malloc(sizeof(char) * 500); // stores the absolute path of the given target directory
    temp_dir = getcwd(temp_dir, 500);
    int retval = chdir(dummy);
    if (retval == -1)
    {
        if (flag_tar_dir == 2)
        {
            printf("\033[0;31m");
            printf("my bash: discover:invalid target directory\n");
            printf("\033[0m");
            return;
        }
    }
    target_dir_abs = getcwd(target_dir_abs, 500);
    chdir(temp_dir);
    chdir(target_dir_abs);
    int i, j, len = strlen(target_dir_abs);
    for (i = len - 1; i >= 0; i--)
        if (target_dir_abs[i] == '/')
            break;
    for (j = i + 1; j < len; j++)
        comp[j - i - 1] = target_dir_abs[j];
    comp[j - i - 1] = '\0';
    print_discover(comp, file_dir, target, target_dir_abs, flag_d, flag_f, 1, flag_file_dir);
    discoverLoop(file_dir, target, target_dir_abs, flag_d, flag_f, flag_file_dir);
    chdir(temp_dir);
    if (count_discover == 0){
        printf("\033[0;31m");
        printf("my bash:discover:No matches found\n");
        printf("\033[0m");
    }
}