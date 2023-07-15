#include "headers.h"

char tab[500]; // stores the thing we are gonna print in the prompt

char fgets_input[500];

int pt;

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int semicolon(char input[], char output_cwd[], char hostname[], char username[], char *homedir)
{
    // no of non null strings after splitting in space and tab should be atleast the no of semicolons
    // strip the input into the semicolons
    char ac_output[500][500];
    char *output = strtok(input, ";"); // the input will be refined as well
    int it = 0;
    while (output != NULL)
    {
        strcpy(ac_output[it], output);
        output = strtok(NULL, ";");
        it++;
    }
    for (int i = 0; i < it; i++)
    {
        int retval = and(output_cwd, hostname, username, ac_output[i], homedir);
        if (retval == -2)
            return -1;
    }
}

void function(){
    int wait_status;
    int status;
    for(int i=0;i<100;i++){
        if(start[i].visited==1){
            wait_status = waitpid(start[i].pid,&status,WNOHANG);
            if(wait_status==0){
                continue;
            }
            else{
                start[i].visited=0;
                strcpy(exit_queue[index_exit].name,start[i].name);
                exit_queue[index_exit].pid=start[i].pid;
                int retval = WIFEXITED(status);
                if(retval==1) exit_queue[index_exit].visited=0;
                else exit_queue[index_exit].visited=1;
                index_exit++;
            }
        }
    }
}

void handler(){
    // this function will store all the exited commands 
    // and make the status of the corressponding processes with 1
    int status;
    pid_t pid = waitpid(-1,&status,WNOHANG);
    if(pid>0){
        for(int i=0;i<100;i++){
            if(pid==start[i].pid){
                start[i].visited=0;
                strcpy(exit_queue[index_exit].name,start[i].name);
                exit_queue[index_exit].pid=pid;
                int retval = WIFEXITED(status);
                if(retval==1) exit_queue[index_exit].visited=0;
                else exit_queue[index_exit].visited=1;
                index_exit++;
            }
        }
    }
}

void prompt(char output_cwd[], char username[], char hostname[], int size_root)
{
    printf("\033[0;32m");
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    int flag_breaking = -1;
    if (strcmp(temp_cwd, output_cwd) == 0)
    {
        printf("%s", username);
        printf("@");
        printf("%s", hostname);
        printf("\033[0m");
        printf("\033[0;37m");
        printf(":");
        printf("\033[0m");
        printf("\033[0;34m");
        printf("~");
        printf("\033[0m");
        if(global_foreground<=-1){
            printf("\033[0;35m");
            printf("took %d s",-1*(global_foreground));
            printf("\033[0m");
            printf("\033[0;37m");
            printf("> ");
            printf("\033[0m");
        }
        else{
            printf("\033[0;37m");
            printf("> ");
            printf("\033[0m");
        }
    }
    else
    {
        for (int i = 0; i < size_root; i++)
        {
            if (temp_cwd[i] != output_cwd[i])
            {
                printf("%s", username);
                printf("@");
                printf("%s", hostname);
                printf("\033[0m");
                printf("\033[0;37m");
                printf(":");
                printf("\033[0m");
                printf("\033[0;34m");
                printf("%s", temp_cwd);
                printf("\033[0m");
                if(global_foreground<=-1){
                    printf("\033[0;35m");
                    printf("took %d s",-1*(global_foreground));
                    printf("\033[0m");
                    printf("\033[0;37m");
                    printf("> ");
                    printf("\033[0m");
                }
                else{
                    printf("\033[0;37m");
                    printf("> ");
                    printf("\033[0m");
                }
                flag_breaking = 1;
                break;
            }
        }
        // we have to print relative to tilde
        if (flag_breaking == -1)
        {
            printf("%s", username);
            printf("@");
            printf("%s", hostname);
            printf("\033[0m");
            printf("\033[0;37m");
            printf(":");
            printf("\033[0m");
            printf("\033[0;34m");
            printf("~");
            printf("\033[0m");
            printf("\033[0;34m");
            for (int i = size_root; temp_cwd[i] != '\0'; i++)
            {
                printf("%c", temp_cwd[i]);
            }
            printf("\033[0m");
            if(global_foreground<=-1){
                printf("\033[0;35m");
                printf("took %d s",-1*(global_foreground));
                printf("\033[0m");
                printf("\033[0;37m");
                printf("> ");
                printf("\033[0m");
            }
            else{
                printf("\033[0;37m");
                printf("> ");
                printf("\033[0m");
            }
        }
    }
    free(temp_cwd);
}

void sort_the_struct(bgCheck input[100],int n){
    bgCheck tempname;
    int swap_count = 0;

    if (n == 1)
    {
        return;
    }

    for (int i = 0; i < n - 1; i++)
    {
        if (strcmp(input[i].full_name, input[i + 1].full_name) < 0)
        {
            continue;
        }
        else
        {
            tempname.pid = input[i].pid;
            tempname.visited=input[i].visited;
            strcpy(tempname.full_name,input[i].full_name);
            strcpy(tempname.name,input[i].name);
            input[i].pid = input[i+1].pid;
            input[i].visited=input[i+1].visited;
            strcpy(input[i].full_name,input[i+1].full_name);
            strcpy(input[i].name,input[i+1].name);
            input[i+1].pid = tempname.pid;
            input[i+1].visited=tempname.visited;
            strcpy(input[i+1].full_name,tempname.full_name);
            strcpy(input[i+1].name,tempname.name);
            ++swap_count;
        }
    }
    if (swap_count > 0)
        sort_the_struct(input, n - 1);
}

void JOBS(char input[]){
    char *output = strtok(input,"\t ");
    output = strtok(NULL,"\t ");
    if(output==NULL){
        bgCheck jobs_sorted[100];
        int it_sort=0;
        for(int i=0;i<100;i++){
            if(start[i].visited==1){
                jobs_sorted[it_sort].pid = start[i].pid;
                jobs_sorted[it_sort].visited = i+1;
                strcpy(jobs_sorted[it_sort].full_name,start[i].full_name);
                strcpy(jobs_sorted[it_sort].name,start[i].name);
                it_sort++;
            }
        }
        sort_the_struct(jobs_sorted,it_sort);
        for(int i=0;i<it_sort;i++){
            char file_name[500]="/proc/";
            char string_pid[500];
            sprintf(string_pid,"%d",jobs_sorted[i].pid);
            strcat(file_name,string_pid);
            strcat(file_name,"/stat");
            FILE* fp = fopen(file_name,"r");
            char status;
            fscanf(fp,"%*s %*s %c",&status);
            if(status=='T'){
                printf("[%d] ",jobs_sorted[i].visited);
                printf("Stopped ");
                printf("%s ",jobs_sorted[i].full_name);
                printf("[%d]\n",jobs_sorted[i].pid);
            }
            else{
                printf("[%d] ",jobs_sorted[i].visited);
                printf("Running ");
                printf("%s ",jobs_sorted[i].full_name);
                printf("[%d]\n",jobs_sorted[i].pid);
            }
        }
    }
    else if(strcmp(output,"-r")==0){
        bgCheck jobs_sorted[100];
        int it_sort=0;
        for(int i=0;i<100;i++){
            if(start[i].visited==1){
                jobs_sorted[it_sort].pid = start[i].pid;
                jobs_sorted[it_sort].visited = i+1;
                strcpy(jobs_sorted[it_sort].full_name,start[i].full_name);
                strcpy(jobs_sorted[it_sort].name,start[i].name);
                it_sort++;
            }
        }
        sort_the_struct(jobs_sorted,it_sort);
        for(int i=0;i<it_sort;i++){
            char file_name[500]="/proc/";
            char string_pid[500];
            sprintf(string_pid,"%d",jobs_sorted[i].pid);
            strcat(file_name,string_pid);
            strcat(file_name,"/stat");
            FILE* fp = fopen(file_name,"r");
            char status;
            fscanf(fp,"%*s %*s %c",&status);
            if(status=='T'){
                continue;
            }
            else{
                printf("[%d] ",jobs_sorted[i].visited);
                printf("Running ");
                printf("%s ",jobs_sorted[i].full_name);
                printf("[%d]\n",jobs_sorted[i].pid);
            }
        }
    }
    else if(strcmp(output,"-s")==0){
        bgCheck jobs_sorted[100];
        int it_sort=0;
        for(int i=0;i<100;i++){
            if(start[i].visited==1){
                jobs_sorted[it_sort].pid = start[i].pid;
                jobs_sorted[it_sort].visited = i+1;
                strcpy(jobs_sorted[it_sort].full_name,start[i].full_name);
                strcpy(jobs_sorted[it_sort].name,start[i].name);
                it_sort++;
            }
        }
        sort_the_struct(jobs_sorted,it_sort);
        for(int i=0;i<it_sort;i++){
            char file_name[500]="/proc/";
            char string_pid[500];
            sprintf(string_pid,"%d",jobs_sorted[i].pid);
            strcat(file_name,string_pid);
            strcat(file_name,"/stat");
            FILE* fp = fopen(file_name,"r");
            char status;
            fscanf(fp,"%*s %*s %c",&status);
            if(status=='T'){
                printf("[%d] ",jobs_sorted[i].visited);
                printf("Stopped ");
                printf("%s ",jobs_sorted[i].full_name);
                printf("[%d]\n",jobs_sorted[i].pid);
            }
            else{
                continue;
            }
        }
    }
    else{
        printf("my bash jobs: invalid flags\n");
        return;
    }
}

void ctrc_handler(){
    return;
}

void ctrz_handler(){
    if(foreground_current.visited!=-1){
        kill(foreground_current.pid,SIGTSTP);
        int num_to_assign=-1;
        for(int i=99;i>=0;i--){
            if(start[i].visited==1){
                num_to_assign=i;
                break;
            }
        }
        if(num_to_assign==-1) num_to_assign=0;
        else num_to_assign++;
        start[num_to_assign].visited=1;
        strcpy(start[num_to_assign].full_name,foreground_current.full_name);
        start[num_to_assign].pid=foreground_current.pid;
        strcpy(start[num_to_assign].name,foreground_current.name);
        return;
    }
    else{
        return;
    }
}

void function_tab(char output_cwd[], char username[], char hostname[], int size_root,char input[]){
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);
    int start=0;
    int start_comp=0;
    int len = strlen(input);
    char directory[500];
    for(int i=0;i<500;i++) directory[i]='\0';
    char comp[500];
    for(int i=0;i<500;i++) comp[i]='\0';
    int j;
    for(int i=len-1;i>=0;i--){
        if(input[i]==' '){
            start=i+1;
            break;
        }
    }
    for(int i=len-1;i>=0;i--){
        if(input[i]=='/'){
            start_comp=i+1;
            break;
        }
    }
    for(int j=start;j<start_comp;j++) directory[j-start]=input[j];
    int start_for_this = max(start_comp,start);
    for(int j=start_for_this;j<len;j++) comp[j-start_for_this]=input[j];
    int comp_len = strlen(comp);
    char sort[500][500];
    char to_print[500][500];
    int it_print=0;
    chdir(directory);
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
    for(int i=0;i<it_sort;i++){
        int flag_each=1;
        for(int j=0;j<comp_len;j++){
            if(sort[i][j]!=comp[j]){
                flag_each=0;
                break;
            }
        }
        if(flag_each==1) strcpy(to_print[it_print++],sort[i]);
    }
    if(it_print==0){
        int i;
        char input_to_copy[500];
        for(i=0;i<500;i++) input_to_copy[i]='\0';
        int start_for_this = max(start_comp,start);
        for(i=0;i<start_for_this;i++) input_to_copy[i]=input[i];
        for(i=start_for_this;i<len;i++) input_to_copy[i]=comp[i-start_for_this];
        input_to_copy[i]='\0';
        printf("\n");
        chdir(temp_cwd);
        prompt(output_cwd,username,hostname,size_root);
        printf("%s",input_to_copy);
        strcpy(fgets_input,input_to_copy);
    }
    else if(it_print==1){   
        int i;
        printf("\n");
        char input_to_copy[500];
        for(i=0;i<500;i++) input_to_copy[i]='\0';
        int start_for_this = max(start_comp,start);
        for(i=0;i<start_for_this;i++) input_to_copy[i]=input[i];
        DIR *dir = opendir(to_print[0]);
        if (dir)
        {
            chdir(temp_cwd);
            prompt(output_cwd,username,hostname,size_root);
            printf("%s",input_to_copy);
            int len1 = strlen(comp);
            int len2 = strlen(to_print[0]);
            pt+=(len2-len1+1);
            printf("%s/",to_print[0]);
            strcat(input_to_copy,to_print[0]);
            strcat(input_to_copy,"/\0");
            strcpy(fgets_input,input_to_copy);
        }
        else{
            chdir(temp_cwd);
            prompt(output_cwd,username,hostname,size_root);
            printf("%s",input_to_copy);
            int len1 = strlen(comp);
            int len2 = strlen(to_print[0]);
            pt+=(len2-len1+1);
            printf("%s ",to_print[0]);
            strcat(input_to_copy,to_print[0]);
            strcat(input_to_copy," \0");
            strcpy(fgets_input,input_to_copy);
        }
        free(dir);
    }
    else{   
        // here i have to handle the multiple things
        printf("\n");
        for(int i=0;i<it_print;i++){
            struct stat sb;
            DIR *dir = opendir(to_print[i]);
            if (dir)
            {
                printf("\033[0;34m");
                printf("%s/\n",to_print[i]);
                printf("\033[0m");
            }
            else printf("%s\n", to_print[i]);
            free(dir);
        }
        // now i have to see the extent common in the common things in the files or dir
        char comp_new[500];
        strcpy(comp_new,to_print[0]);
        char new_to_print[500];
        for(int i=0;i<500;i++) new_to_print[i]='\0';
        int len_comp_new = strlen(comp_new);
        for(int i=0;i<len_comp_new;i++){
            int flag_each=1;
            for(int j=1;j<it_print;j++){
                if(comp_new[i]!=to_print[j][i]){
                    flag_each=0;
                    break;
                }
            }   
            if(flag_each==0) break;
            new_to_print[i]=comp_new[i];
        }
        chdir(temp_cwd);
        prompt(output_cwd,username,hostname,size_root);
        int i;
        char input_to_copy[500];
        for(i=0;i<500;i++) input_to_copy[i]='\0';
        int start_for_this = max(start_comp,start);
        for(i=0;i<start_for_this;i++) input_to_copy[i]=input[i];
        int len2 = strlen(new_to_print);
        int len1 = strlen(comp);
        pt+=(len2-len1);
        printf("%s",input_to_copy);
        printf("%s",new_to_print);
        strcat(input_to_copy,new_to_print);
        strcat(input_to_copy,"\0");
        strcpy(fgets_input,input_to_copy);
    }
}


int main()
{
    char *username;
    char hostname[500];
    char *output_cwd; // root dir
    int size_root = 0;
    flag=-1;
    count_discover=0;
    count_before = 0;
    index_exit=0;
    for(int i=0;i<100;i++){
        start[i].visited=0;
        start[i].pid=-1;
    }
    printf("\e[1;1H\e[2J");
    char input[500];
    int flag = -1;
    // char *username;
    username = getlogin();
    // char hostname[500];
    gethostname(hostname, 500);
    output_cwd = (char *)malloc(sizeof(char) * 500); // root dir
    output_cwd = getcwd(output_cwd, 500);
    // int size_root = 0;
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    for (int i = 0; output_cwd[i] != '\0'; i++)
        size_root++; // size of the string of the dir
    int flag_is_there_in_prev = 0;
    char *temp_cwd = (char *)malloc(sizeof(char) * 500);
    temp_cwd = getcwd(temp_cwd, 500);

    char prev_comm_history[500];

    int no_commands = 0;
    int count = 0;
    chdir(homedir);
    char tmp[500];

    FILE *fptr = fopen("history.txt", "r");

    if (fptr != NULL)
    {
        while (!feof(fptr))
        {
            fgets(tmp, 1024, fptr);
            count++;
        }
    }

    if (fptr != NULL)
    {
        int len = strlen(tmp);
        tmp[len - 1] = '\0';
        strcpy(prev_comm_history, tmp);
        flag_is_there_in_prev = 1;
    }

    no_commands = count - 1;

    chdir(temp_cwd);

    foreground_current.visited=-1;
    

    while (1)
    {
        signal(SIGINT,ctrc_handler);
        signal(SIGTSTP,ctrz_handler);
        pt = 0;
        setbuf(stdout, NULL);
        enableRawMode();
        memset(fgets_input, '\0', 100);
        char c;
        signal(SIGCHLD,handler);
        function();
        if(index_exit>0){
            printf("\033[0;34m");
            for(int i=0;i<index_exit;i++) printf("Process %s with pid %d exited %s\n",exit_queue[i].name,exit_queue[i].pid,exit_queue[i].visited==1?"abnormally":"normally");
            printf("\033[0m");
            index_exit=0;
        }
        // decides we are executing which command
        prompt(output_cwd, username, hostname, size_root);
        temp_cwd = getcwd(temp_cwd, 500);
        global_foreground=0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10){
                    printf("\033[0;34m");
                    for(int i=0;i<index_exit;i++) printf("\nProcess %s with pid %d exited %s",exit_queue[i].name,exit_queue[i].pid,exit_queue[i].visited==1?"abnormally":"normally");
                    printf("\033[0m");
                    index_exit=0;
                    break;
                }
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (fgets_input[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        fgets_input[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    function_tab(output_cwd,username,hostname,size_root,fgets_input);
                } else if (c == 4) {
                    printf("\n");
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                fgets_input[pt++] = c;
                printf("%c", c);
            }
        }
        printf("\n");
        disableRawMode();
        strcpy(input,fgets_input);
        int index_valid = -1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (input[i] != '\t' && input[i] != ' ' && input[i] != '\n')
            {
                index_valid = 1;
                break;
            }
        }
        if (index_valid == 1)
        {
            // that command is valid
            // store the command in the prev array
            if (flag_is_there_in_prev == 0)
            {
                // we don't have to check
                flag_is_there_in_prev = 1;
                chdir(homedir);
                FILE *fptr = fopen("history.txt", "w");
                fprintf(fptr, "%s\n", input);
                fclose(fptr);
                chdir(temp_cwd);
            }
            else
            {
                // we have to check the prev command
                if (strcmp(input, prev_comm_history) != 0)
                {
                    // we have to append in this case
                    no_commands++;
                    if (no_commands > 20)
                    {
                        char contents[50][200];
                        int it_con = 0;
                        int count_line = 0;
                        no_commands = 20;
                        chdir(homedir);
                        FILE *fptr = fopen("history.txt", "r");
                        char *line;
                        size_t len = 0;
                        while ((getline(&line, &len, fptr) != -1))
                        {
                            if (count_line >= 1)
                            {
                                strcpy(contents[it_con++], line);
                            }
                            count_line++;
                        }
                        fclose(fptr);
                        chdir(temp_cwd);
                        chdir(homedir);
                        fptr = fopen("history.txt", "w");
                        for (int i = 0; i < it_con; i++)
                        {
                            fprintf(fptr, "%s", contents[i]);
                        }
                        fprintf(fptr, "%s\n", input);
                        fclose(fptr);
                        chdir(temp_cwd);
                    }
                    else
                    {
                        chdir(homedir);
                        FILE *fptr = fopen("history.txt", "a");
                        fprintf(fptr, "%s\n", input);
                        fclose(fptr);
                        chdir(temp_cwd);
                    }
                }
            }
            strcpy(prev_comm_history, input);
        }
        // now we have to send this input to a semicolon function
        int retval = semicolon(input, output_cwd, hostname, username, homedir);
        if (retval == -1)
            break;
        
    }
    free(output_cwd);
    free(temp_cwd);
    return 0;
}