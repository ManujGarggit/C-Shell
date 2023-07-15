#ifndef __DISCOVER_H

#define __DISCOVER_H

void print_relative(char file[], char target[], char target_dir_abs[], int flag_file_or_dir);

int print_discover(char comp[], char file_dir[], char target[], char target_dir_abs[], int flag_d, int flag_f, int flag_file_or_dir, int is_some_file);

void discoverLoop(char file_dir[], char target[], char target_dir_abs[], int flag_d, int flag_f, int flag_file_dir);

void DISCOVER(char dummy_input[], char output_cwd[]);

#endif