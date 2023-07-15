#ifndef __LS_H

#define __LS_H

int length(long long int a);

long long max(long long a, long long b);

void permissions(char path[500]);

void sort_the_dir(char sort[][500], int n);

void print_file(char input[], int a, int b, int c, int d);

void printDir(char input[], int m_b_u, int len_user, int len_gr, int file_size);

void LS(int flag, int flag_dir_file, char input[], int a, int b, int c, int d);

void LS_start(char input[], char output_cwd[]);

void PRINT(char input[]);

#endif