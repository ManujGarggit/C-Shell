#include "headers.h"

int length(long long int a)
{
    int count = 0;
    while (a > 0)
    {
        a /= 10;
        count++;
    }
    return count;
}


long long max(long long a, long long b)
{
    return a >= b ? a : b;
}


void PRINT(char input[])
{
    struct stat sb;
    DIR *dir = opendir(input);
    if (dir)
    {
        printf("\033[0;34m");
        printf("%s  ", input);
        printf("\033[0m");
    }
    else
    {
        if (stat(input, &sb) == 0 && sb.st_mode & S_IXUSR)
        {
            printf("\033[0;32m");
            printf("%s  ", input);
            printf("\033[0m");
        }
        else
        {
            printf("%s  ", input);
        }
    }
    free(dir);
}

void permissions(char path[500])
{
    struct stat fileStat;
    stat(path, &fileStat);
    if ((fileStat.st_mode & S_IRUSR))
        printf("r");
    else
        printf("-");
    if ((fileStat.st_mode & S_IWUSR))
        printf("w");
    else
        printf("-");
    if ((fileStat.st_mode & S_IXUSR))
        printf("x");
    else
        printf("-");
    if ((fileStat.st_mode & S_IRGRP))
        printf("r");
    else
        printf("-");
    if ((fileStat.st_mode & S_IWGRP))
        printf("w");
    else
        printf("-");
    if ((fileStat.st_mode & S_IXGRP))
        printf("x");
    else
        printf("-");
    if ((fileStat.st_mode & S_IROTH))
        printf("r");
    else
        printf("-");
    if ((fileStat.st_mode & S_IWOTH))
        printf("w");
    else
        printf("-");
    if ((fileStat.st_mode & S_IXOTH))
        printf("x");
    else
        printf("-");
}

void sort_the_dir(char sort[][500], int n)
{

    char tempname[5000];
    int swap_count = 0;

    if (n == 1)
    {
        return;
    }

    for (int i = 0; i < n - 1; i++)
    {
        if (strcmp(sort[i], sort[i + 1]) < 0)
        {
            continue;
        }
        else
        {
            strcpy(tempname, sort[i]);
            strcpy(sort[i], sort[i + 1]);
            strcpy(sort[i + 1], tempname);
            ++swap_count;
        }
    }
    if (swap_count > 0)
        sort_the_dir(sort, n - 1);
}

void print_file(char input[], int a, int b, int c, int d)
{
    struct stat stats;
    struct passwd *u;
    struct group *gr;
    printf("-");
    permissions(input);
    printf("  ");
    stat(input, &stats);
    printf("%*ld  ", a, stats.st_nlink);
    u = getpwuid(stats.st_uid);
    printf("%*s  ", b, u->pw_name);
    gr = getgrgid(stats.st_gid);
    printf("%*s  ", c, gr->gr_name);
    printf("%*ld  ", d, stats.st_size);
    PRINT(input);
    printf("\n");
}


void printDir(char input[], int m_b_u, int len_user, int len_gr, int file_size)
{
    struct stat stats;
    struct passwd *u;
    struct group *gr;
    DIR *dir = opendir(input);
    if (dir)
        printf("d");
    else
        printf("-");
    permissions(input);
    printf("  ");
    stat(input, &stats);
    printf("%*ld  ", m_b_u, stats.st_nlink);
    u = getpwuid(stats.st_uid);
    printf("%*s  ", len_user, u->pw_name);
    gr = getgrgid(stats.st_gid);
    printf("%*s  ", len_gr, gr->gr_name);
    printf("%*ld  ", file_size, stats.st_size);
    char *a = ctime(&stats.st_mtime);
    int count_colon = 0;
    for (int i = 4; a[i] != '\0'; i++)
    {
        if (a[i] == ':')
            count_colon++;
        if (count_colon == 2)
            break;
        printf("%c", a[i]);
    }
    printf("  ");
    PRINT(input);
    printf("\n");
    free(dir);
}


void LS(int flag, int flag_dir_file, char input[], int a, int b, int c, int d)
{

    char sort[500][500];
    if (flag == 0)
    {
        if (flag_dir_file == 0)
        {
            PRINT(input);
            return;
        }
        char *dir = ".";
        int it_sort = 0;
        struct dirent *d;
        DIR *dh = opendir(dir);
        while ((d = readdir(dh)) != NULL)
        {
            if (d->d_name[0] == '.')
                continue;
            strcpy(sort[it_sort++], d->d_name);
        }
        free(dh);
        free(d);
        if (it_sort == 0)
            return;
        sort_the_dir(sort, it_sort);
        for (int i = 0; i < it_sort; i++)
        {
            PRINT(sort[i]);
        }
        printf("\n");
    }
    else if (flag == 1)
    {
        char sort[500][500];
        if (flag_dir_file == 0)
        {
            PRINT(input);
            return;
        }
        char *dir = ".";
        int it_sort = 0;
        struct dirent *d;
        DIR *dh = opendir(dir);
        while ((d = readdir(dh)) != NULL)
        {
            strcpy(sort[it_sort++], d->d_name);
        }
        free(dh);
        free(d);
        if (it_sort == 0)
            return;
        sort_the_dir(sort, it_sort);
        for (int i = 0; i < it_sort; i++)
        {
            PRINT(sort[i]);
        }
        printf("\n");
    }
    else if (flag == 2)
    {
        struct stat stats;
        struct passwd *u;
        struct group *gr;
        long long int sum = 0;
        if (flag_dir_file == 0)
        {
            print_file(input, a, b, c, d);
            return;
        }
        int it_file = 0;
        int it_dir = 0;
        char *dir = ".";
        int it_sort = 0;
        struct dirent *d;
        int m_b_u = INT_MIN;
        int len_user = INT_MIN;
        int len_gr = INT_MIN;
        int file_size = INT_MIN;
        DIR *dh = opendir(dir);
        while ((d = readdir(dh)) != NULL)
        {
            if (d->d_name[0] == '.')
                continue;
            strcpy(sort[it_sort++], d->d_name);
        }
        free(dh);
        free(d);
        if (it_sort == 0)
        {
            printf("total %lld\n", sum / 2);
            closedir(dh);
            return;
        }
        sort_the_dir(sort, it_sort);
        for (int i = 0; i < it_sort; i++)
        {
            stat(sort[i], &stats);
            sum += (stats.st_blocks);
            m_b_u = max(m_b_u, length(stats.st_nlink));
            u = getpwuid(stats.st_uid);
            len_user = max(len_user, strlen(u->pw_name));
            gr = getgrgid(stats.st_gid);
            len_gr = max(len_gr, strlen(gr->gr_name));
            file_size = max(file_size, length(stats.st_size));
        }
        printf("total %lld\n", sum / 2);
        for (int i = 0; i < it_sort; i++)
        {
            printDir(sort[i], m_b_u, len_user, len_gr, file_size);
        }
    }
    else if (flag == 3)
    {
        struct stat stats;
        struct passwd *u;
        long long int sum = 0;
        struct group *gr;
        if (flag_dir_file == 0)
        {
            print_file(input, a, b, c, d);
            return;
        }
        int it_file = 0;
        int it_dir = 0;
        char *dir = ".";
        int it_sort = 0;
        struct dirent *d;
        int m_b_u = INT_MIN;
        int len_user = INT_MIN;
        int len_gr = INT_MIN;
        int file_size = INT_MIN;
        DIR *dh = opendir(dir);
        while ((d = readdir(dh)) != NULL)
        {
            strcpy(sort[it_sort++], d->d_name);
        }
        free(dh);
        if (it_sort == 0)
        {
            printf("total %lld\n", sum / 2);
            closedir(dh);
            return;
        }
        sort_the_dir(sort, it_sort);
        for (int i = 0; i < it_sort; i++)
        {
            stat(sort[i], &stats);
            sum += (stats.st_blocks);
            m_b_u = max(m_b_u, length(stats.st_nlink));
            u = getpwuid(stats.st_uid);
            len_user = max(len_user, strlen(u->pw_name));
            gr = getgrgid(stats.st_gid);
            len_gr = max(len_gr, strlen(gr->gr_name));
            file_size = max(file_size, length(stats.st_size));
        }
        printf("total %lld\n", sum / 2);
        for (int i = 0; i < it_sort; i++)
        {
            printDir(sort[i], m_b_u, len_user, len_gr, file_size);
        }
    }
}

void LS_start(char input[], char output_cwd[])
{

    int flag_a = 0;
    int flag_l = 0;
    int it = 0;

    char input_dir_file[500][500];
    char dummy_input[500];

    strcpy(dummy_input, input); // first only for seeing that data contains . ,.. or ~
    char *output_dummy = strtok(dummy_input, "\t ");
    output_dummy = strtok(NULL, "\t ");

    if (output_dummy == NULL)
    {
        LS(0, -1, output_dummy, -1, -1, -1, -1); // we don't have any arguments (not any directory name or file name in the input)
        return;
    }

    int flag;

    // 0 -> no flag
    // 1 -> only a
    // 2 -> only l
    // 3 ->both l and a

    char *output = strtok(input, "\t ");
    output = strtok(NULL, "\t ");
    while (output != NULL)
    {
        if (output[0] == '-')
        {
            int len = strlen(output);
            if (len > 3)
            {
                printf("\033[0;31m");
                printf("my bash: ls:invalid arguments\n");
                printf("\033[0m");
                return;
            }

            if (output[1] == 'a' || output[2] == 'a')
                flag_a = 1;
            if (output[1] == 'l' || output[2] == 'l')
                flag_l = 1;

            if (output[1] != 'l' && output[1] != 'a')
            {
                printf("\033[0;31m");
                printf("my bash: ls:invalid arguments\n");
                printf("\033[0m");
                return;
            }
            else if (output[2] != '\0')
            {
                if (output[2] != 'a' && output[2] != 'l')
                {
                    printf("\033[0;31m");
                    printf("my bash: ls:invalid arguments\n");
                    printf("\033[0m");
                    return;
                }
            }
        }
        else if (output[0] == '~')
        {
            if (output[1] == '\0')
                strcpy(input_dir_file[it++], output_cwd);
            else if (output[1] == '/')
            {
                if (output[2] == '\0')
                    strcpy(input_dir_file[it++], output_cwd);
                else
                {
                    char dummy[500];
                    char cwd[500];
                    int i;
                    for (i = 1; output[i] != '\0'; i++)
                    {
                        dummy[i - 1] = output[i];
                    }
                    dummy[i - 1] = '\0';
                    strcpy(cwd, output_cwd);
                    strcat(cwd, dummy);
                    strcpy(input_dir_file[it++], cwd);
                }
            }
        }
        else
        {
            strcpy(input_dir_file[it++], output);
        }
        output = strtok(NULL, "\t ");
    }

    // flag_dir_file 0 for a file and 1 for dir

    if (flag_a == 1 && flag_l == 1)
        flag = 3;
    else if (flag_a == 1 && flag_l == 0)
        flag = 1;
    else if (flag_a == 0 && flag_l == 1)
        flag = 2;
    else
        flag = 0;

    char files[500][500];
    char directory[500][500];
    int it_dir = 0;
    int it_file = 0;

    for (int i = 0; i < it; i++)
    {
        int flag_file_is_there = 0;
        if (access(input_dir_file[i], F_OK) == 0)
            flag_file_is_there = 1;
        DIR *dir = opendir(input_dir_file[i]);
        if (dir)
        {
            strcpy(directory[it_dir++], input_dir_file[i]);
        }
        else
        {
            if (flag_file_is_there == 1)
            {
                strcpy(files[it_file++], input_dir_file[i]);
            }
            else
            {   
                printf("\033[0;31m");
                printf("ls:cannot access '%s':No such file or directory\n", input_dir_file[i]);
                printf("\033[0m");
            }
        }
        free(dir);
    }

    sort_the_dir(files, it_file);
    sort_the_dir(directory, it_dir);

    if (it_file == 0 && it_dir == 0)
    {
        char temp[500];
        if (flag == 1 || flag == 2 || flag == 3)
        {
            LS(flag, -1, temp, -1, -1, -1, -1);
        }
    }

    struct stat stats;
    struct passwd *u;
    struct group *gr;
    int m_b_u = INT_MIN;
    int len_user = INT_MIN;
    int len_gr = INT_MIN;
    int file_size = INT_MIN;

    for (int i = 0; i < it_file; i++)
    {
        stat(files[i], &stats);
        m_b_u = max(m_b_u, length(stats.st_nlink));
        u = getpwuid(stats.st_uid);
        len_user = max(len_user, strlen(u->pw_name));
        gr = getgrgid(stats.st_gid);
        len_gr = max(len_gr, strlen(gr->gr_name));
        file_size = max(file_size, length(stats.st_size));
    }

    for (int i = 0; i < it_file; i++)
    {
        LS(flag, 0, files[i], m_b_u, len_user, len_gr, file_size);
    }

    if (it_file > 0)
    {
        if (it_dir > 0)
        {
            if (flag == 0 || flag == 1)
                printf("\n\n");
            else
                printf("\n");
        }
        else
        {
            if (flag == 0 || flag == 1)
                printf("\n");
        }
    }

    int count_dir = 0;

    char *temp_cwd = (char *)malloc(sizeof(char) * 500);

    for (int i = 0; i < it_dir; i++)
    {
        temp_cwd = getcwd(temp_cwd, 500);
        if (it_dir > 1 || it_file >= 1)
            printf("%s:\n", directory[i]);
        chdir(directory[i]);
        LS(flag, 1, input_dir_file[i], -1, -1, -1, -1);
        chdir(temp_cwd);
        count_dir++;
        if ((it_dir - count_dir) > 0)
            printf("\n");
    }
    free(temp_cwd);
}