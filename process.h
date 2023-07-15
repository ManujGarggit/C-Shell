#ifndef __PROCESS_H

#define __PROCESS_H


int background_process(char output_cwd[],char hostname[],char username[],char background[],char* homedir);

int ind_command(char output_cwd[], char hostname[], char username[], char input[], char *homedir,int read,int w_,int org_out);

int and(char output_cwd[],char hostname[],char username[],char input[],char* homedir);

int piping(char output_cwd[],char hostname[],char username[],char input[],char* homedir);

void JOBS(char input[]);

void SIG(char input[]);

void FG(char input[]);

void BG(char input[]);

#endif