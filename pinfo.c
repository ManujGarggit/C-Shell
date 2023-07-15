#include "headers.h"

void pInfo(char input[], char output_cwd[])
{

    int i;
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
        printf("my bash: pinfo:too many arguments\n");
        printf("\033[0m");
        return;
    }

    int pid = -1;
    char stat = '-';
    int pg = -1;
    int tg = -1;

    char data[500] = "/proc/";
    char virtual_data[500] = "/proc/";
    char exec_data[500] = "/proc/";
    char *output = strtok(input, "\t ");
    output = strtok(NULL, "\t ");
    if (output == NULL)
    {
        // pinfo of the process id
        strcat(data, "self");
        strcat(virtual_data, "self");
        strcat(exec_data, "self");
        strcat(data, "/stat");
    }
    else
    {
        // we have to tell the pinfo for the given process id
        strcat(data, output);
        strcat(virtual_data, output);
        strcat(exec_data, output);
        strcat(data, "/stat");
    }

    FILE *file = fopen(data, "r");

    if(file==NULL){
        printf("\033[0;31m");
        printf("my bash: pinfo:not a valid pid\n");
        printf("\033[0m");
        return;
    }
    char ma[100];
    fscanf(file, "%d %s %c %s %d %s %s %d", &pid,ma, &stat, ma,&pg,ma,ma,&tg);
    printf("pid : %d\n", pid);

    if (pg==tg)
        printf("Process Status : %c+\n", stat);
    else
        printf("Process Status : %c\n", stat);

    char temp[500];
    strcat(virtual_data, "/statm");
    FILE* fp = fopen(virtual_data, "r");
    fscanf(fp, "%s", temp);
    printf("memory : %s {Virtual Memory}\n", temp);

    char executable_file[500];
    strcat(exec_data, "/exe");
    FILE* new = fopen(exec_data,"r");
    if(new==NULL){
        printf("executable file : permission denied\n");
    }
    else{
        int size_root = strlen(output_cwd);
        size_t l = readlink(exec_data, executable_file, 500);
        executable_file[l] = '\0';
        if (strcmp(executable_file, output_cwd) == 0)
        {
            printf("executable file : ~\n");
        }
        else
        {
            int flag_breaking = -1;
            for (int i = 0; i < size_root; i++)
            {
                if (executable_file[i] != output_cwd[i])
                {
                    flag_breaking = 1;
                    break;
                }
            }
            if (flag_breaking == 1)
            {
                printf("executable file : %s\n", executable_file);
            }
            else
            {
                printf("execuatble file : ~");
                for (int i = size_root; executable_file[i] != '\0'; i++)
                {
                    printf("%c", executable_file[i]);
                }
                printf("\n");
            }
        }
    }
}