#include "headers.h"

int background_process(char output_cwd[],char hostname[],char username[],char background[],char* homedir){

    char i_o[500][500];
    int it_i_o=0;
    int j=0;
    int first=-1;
    int first_len;
    int second_len;
    int second=-1;
    int count_race=0;
    char input_file[500];
    char output_file[500];
    for(int i=0;background[i]!='\0';i++){
        if(background[i]=='<' || background[i]=='>'){
            if(count_race==0 && background[i+1]=='>'){
                first = 1;
                first_len=2;
                i++;
                i_o[it_i_o][j]='\0';
                it_i_o++;
                j=0;
                count_race++;
                continue;
            }
            else if(count_race==0 && background[i+1]!='>'){
                if(background[i]=='<') first=0;
                else if(background[i]=='>') first=1;
                first_len=1;
            }

            if(count_race==1 && background[i+1]=='>'){
                second = 1;
                second_len=2;
                i++;
                i_o[it_i_o][j]='\0';
                it_i_o++;
                j=0;
                count_race++;
                continue;
            }
            else if(count_race==1 && background[i+1]!='>'){
                if(background[i]=='<') second=0;
                else if(background[i]=='>') second=1;
                second_len=1;
            }
            i_o[it_i_o][j]='\0';
            it_i_o++;
            j=0;
            count_race++;
            continue;
        }
        i_o[it_i_o][j++] = background[i];
    }
    i_o[it_i_o][j]='\0';

    if(it_i_o>=3){
        printf("my bash: too many arguments\n");
        return 1;
    }
    if(first==1 && second!=-1){
        printf("my bash: invalid arguments\n");
        return 1;
    }
    if(first==0 && second==0){
        printf("my bash: invalid arguments\n");
        return 1;
    }

    int out,fd_out,fd_in,in;    
    if(it_i_o>=1){ 
        out = dup(1);
        in = dup(0);
        if(first==0){
            int count_file=0;
            char* output_i_o = strtok(i_o[1],"\t ");
            strcpy(input_file,output_i_o);
            output_i_o=strtok(NULL,"\t ");
            while(output_i_o!=NULL){
                count_file++;
                output_i_o=strtok(NULL,"\t ");
            }
            if(count_file>0){
                printf("my bash:too many input files\n");
                return 1;
            }
            fd_in = open(input_file,O_RDONLY);
            if(fd_in==-1){
                printf("my bash: invalid input file\n");
                return 1;
            }
            if(second==1){
                int count_file=0;
                char* output_i_o = strtok(i_o[2],"\t ");
                strcpy(output_file,output_i_o);
                output_i_o=strtok(NULL,"\t ");
                while(output_i_o!=NULL){
                    count_file++;
                    output_i_o=strtok(NULL,"\t ");
                }
                if(count_file>0){
                    printf("my bash:too many output files\n");
                    return 1;
                }
                if(second_len==1) fd_out = open(output_file,O_CREAT|O_WRONLY|O_TRUNC,0644);
                else fd_out = open(output_file,O_CREAT|O_APPEND|O_WRONLY,0644);
            }
        }
        if(first==1){
            int count_file=0;
            char* output_i_o = strtok(i_o[1],"\t ");
            strcpy(output_file,output_i_o);
            output_i_o=strtok(NULL,"\t ");
            while(output_i_o!=NULL){
                count_file++;
                output_i_o=strtok(NULL,"\t ");
            }
            if(count_file>0){
                printf("my bash:too many output files\n");
                return 1;
            }
            if(first_len==1) fd_out = open(output_file,O_CREAT|O_WRONLY|O_TRUNC,0644);
            else fd_out = open(output_file,O_CREAT|O_APPEND|O_WRONLY,0644);
        }
    }

    char dummy_input_expv[500];
    strcpy(dummy_input_expv,i_o[0]);
    char jobs_input[500];
    strcpy(jobs_input,dummy_input_expv);

    if(first==1){
        fflush(stdout);
        dup2(fd_out,1);
    }
    else if(first==0 && second==-1) dup2(fd_in,0);
    else if(first==0 && second==1){
        fflush(stdout);
        dup2(fd_in,0);
        dup2(fd_out,1);
    }

    char* a[100];
    int i=1;
    char* initial = strtok(dummy_input_expv,"\t ");
    a[0]=initial;
    initial=strtok(NULL,"\t ");
    while(initial!=NULL){
        a[i++]=initial;
        initial=strtok(NULL,"\t ");
    }
    a[i]=NULL;

    // i have to assign the numbers to the background processes 
    // fork will give me the pid of the background process
    // also store what is the background process

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
    
    pid_t pid = fork();
    
    if(pid==0){
        setpgrp();
        int retval = execvp(a[0],a);
        if(retval==-1){
            printf("my bash:command not found\n");
            printf("\033[0m");
            exit(0);
        }
    }
    else{
        if(first==0 && second==-1){
            dup2(in,0);
            close(fd_in);
        }
        if(first==0 && second==1){
            dup2(in,0);
            dup2(out,1);
            close(fd_in);
            close(fd_out);
        }
        if(first==1){
            dup2(out,1);
            close(fd_out);
        }
        start[num_to_assign].pid=pid;
        strcpy(start[num_to_assign].name,a[0]);
        strcpy(start[num_to_assign].full_name,jobs_input);
        printf("[%d] %d\n",num_to_assign+1,start[num_to_assign].pid);
    }                                       
}

int ind_command(char output_cwd[], char hostname[], char username[], char input[], char *homedir,int read,int w_,int org_out)
{
    int sprint_org=dup(1);
    char i_o[500][500];
    int it_i_o=0;
    int j=0;
    int first=-1;
    int first_len;
    int second_len;
    int second=-1;
    int count_race=0;
    char input_file[500];
    char output_file[500];
    for(int i=0;input[i]!='\0';i++){
        if(input[i]=='<' || input[i]=='>'){
            if(count_race==0 && input[i+1]=='>'){
                first = 1;
                first_len=2;
                i++;
                i_o[it_i_o][j]='\0';
                it_i_o++;
                j=0;
                count_race++;
                continue;
            }
            else if(count_race==0 && input[i+1]!='>'){
                if(input[i]=='<') first=0;
                else if(input[i]=='>') first=1;
                first_len=1;
            }

            if(count_race==1 && input[i+1]=='>'){
                second = 1;
                second_len=2;
                i++;
                i_o[it_i_o][j]='\0';
                it_i_o++;
                j=0;
                count_race++;
                continue;
            }
            else if(count_race==1 && input[i+1]!='>'){
                if(input[i]=='<') second=0;
                else if(input[i]=='>') second=1;
                second_len=1;
            }
            i_o[it_i_o][j]='\0';
            it_i_o++;
            j=0;
            count_race++;
            continue;
        }
        i_o[it_i_o][j++] = input[i];
    }
    i_o[it_i_o][j]='\0';

    char error[500];
    int out,fd_out,fd_in,in;
    if(it_i_o>=1){ 
        out = dup(1);
        in = dup(0);
        if(first==0){
            int count_file=0;
            char* output_i_o = strtok(i_o[1],"\t ");
            strcpy(input_file,output_i_o);
            output_i_o=strtok(NULL,"\t ");
            while(output_i_o!=NULL){
                count_file++;
                output_i_o=strtok(NULL,"\t ");
            }
            if(count_file>0){
                sprintf(error,"my bash: too many input files\n");
                write(org_out,error,strlen(error));
                return -1;
            }
            fd_in = open(input_file,O_RDONLY);
            if(fd_in==-1){
                sprintf(error,"my bash: invalid input file\n");
                write(org_out,error,strlen(error));
                return -1;
            }
            if(second==1){
                int count_file=0;
                char* output_i_o = strtok(i_o[2],"\t ");
                strcpy(output_file,output_i_o);
                output_i_o=strtok(NULL,"\t ");
                while(output_i_o!=NULL){
                    count_file++;
                    output_i_o=strtok(NULL,"\t ");
                }
                if(count_file>0){
                    sprintf(error,"my bash: too many output files\n");
                    write(org_out,error,strlen(error));
                    return -1;
                }
                if(second_len==1) fd_out = open(output_file,O_CREAT|O_WRONLY|O_TRUNC,0644);
                else fd_out = open(output_file,O_CREAT|O_APPEND|O_WRONLY,0644);
            }
        }
        if(first==1){
            int count_file=0;
            char* output_i_o = strtok(i_o[1],"\t ");
            strcpy(output_file,output_i_o);
            output_i_o=strtok(NULL,"\t ");
            while(output_i_o!=NULL){
                count_file++;
                output_i_o=strtok(NULL,"\t ");
            }
            if(count_file>0){
                sprintf(error,"my bash: too many output files\n");
                write(org_out,error,strlen(error));
                return -1;
            }
            if(second==0){
                sprintf(error,"my bash: wrong input output format\n");
                write(org_out,error,strlen(error));
            }
            if(first_len==1) fd_out = open(output_file,O_CREAT|O_WRONLY|O_TRUNC,0644);
            else fd_out = open(output_file,O_CREAT|O_APPEND|O_WRONLY,0644);
        }
    }

    char dummy_input[1000];
    char dummy_input_expv[500];
    strcpy(dummy_input,i_o[0]);
    strcpy(dummy_input_expv,i_o[0]);
    
    if(first==1){
        fflush(stdout);
        dup2(fd_out,1);
    }
    else if(first==0 && second==-1) dup2(fd_in,0);
    else if(first==0 && second==1){
        fflush(stdout);
        dup2(fd_in,0);
        dup2(fd_out,1);
    }

    char *initial_input = strtok(i_o[0], "\t ");
    if (initial_input == NULL)
    {
        if(first==0 && second==-1){
            dup2(in,0);
            close(fd_in);
        }
        if(first==0 && second==1){
            dup2(in,0);
            dup2(out,1);
            close(fd_in);
            close(fd_out);
        }
        if(first==1){
            dup2(out,1);
            close(fd_out);
        }
        return -1;
    }

    if (strcmp(initial_input, "q\0") == 0)
    {
        if(first==0 && second==-1){
            dup2(in,0);
            close(fd_in);
        }
        if(first==0 && second==1){
            dup2(in,0);
            dup2(out,1);
            close(fd_in);
            close(fd_out);
        }
        if(first==1){
            dup2(out,1);
            close(fd_out);
        }
        return -2;
    }
    else if (strcmp(initial_input, "echo\0") == 0)
    {
        echo(dummy_input);
    }
    else if (strcmp(initial_input, "cd\0") == 0)
    {
        CD(dummy_input, output_cwd);
    }
    else if (strcmp(initial_input, "pwd\0") == 0)
    {
        PWD(output_cwd, hostname, username);
    }
    else if (strcmp(initial_input, "clear\0") == 0)
    {
        printf("\e[1;1H\e[2J");
    }
    else if (strcmp(initial_input, "ls\0") == 0)
    {
        LS_start(dummy_input, output_cwd);
    }
    else if (strcmp(initial_input, "history\0") == 0)
    {
        HISTORY(homedir);
    }
    else if (strcmp(initial_input, "pinfo") == 0)
    {
        pInfo(dummy_input, output_cwd);
    }
    else if (strcmp(initial_input, "discover") == 0)
    {
        DISCOVER(dummy_input, output_cwd);
    }
    else if(strcmp(initial_input,"jobs\0")==0){
        JOBS(dummy_input);
    }
    else if(strcmp(initial_input,"sig")==0){
        SIG(dummy_input);
    }
    else if(strcmp(initial_input,"fg")==0){
        FG(dummy_input);
    }
    else if(strcmp(initial_input,"bg\0")==0){
        BG(dummy_input);
    }
    else
    {
        char* a[100];
        char to_assign_ctrl_z[500];
        strcpy(to_assign_ctrl_z,dummy_input_expv);
        int i=1;
        char* initial = strtok(dummy_input_expv,"\t ");
        a[0]=initial;
        initial=strtok(NULL,"\t ");
        while(initial!=NULL){
            a[i++]=initial;
            initial=strtok(NULL,"\t ");
        }
        a[i]=NULL;
        pid_t pid = fork();
        int begin = time(NULL);
        if(pid==0){
            int retval = execvp(a[0],a);
            if(retval==-1){
                sprintf(error,"my bash: command not found\n");
                write(org_out,error,strlen(error));
                exit(0);
                return -1;
            }
        }
        else{
            foreground_current.visited=1;
            strcpy(foreground_current.full_name,to_assign_ctrl_z);
            foreground_current.pid = pid;
            char* output=strtok(to_assign_ctrl_z,"\t ");
            strcpy(foreground_current.name,output);
            int status;
            waitpid(pid,&status,WUNTRACED|WCONTINUED);
            foreground_current.visited=-1;
            if(first==0 && second==-1){
                dup2(in,0);
                close(fd_in);
            }
            if(first==0 && second==1){
                dup2(in,0);
                dup2(out,1);
                close(fd_in);
                close(fd_out);
            }
            if(first==1){
                dup2(out,1);
                close(fd_out);
            }
            int end = time(NULL);
            global_foreground = (-1)*(end-begin);
            return 1;
        }
    }
    if(first==0 && second==-1){
        dup2(in,0);
        close(fd_in);
    }
    if(first==0 && second==1){
        dup2(in,0);
        dup2(out,1);
        close(fd_in);
        close(fd_out);
    }
    if(first==1){
        dup2(out,1);
        close(fd_out);
    }
    return 1;
}

int piping(char output_cwd[],char hostname[],char username[],char input[],char* homedir){
    char piped[500][500];
    int j=0;
    int it_process=0;
    for(int i=0;input[i]!='\0';i++){
        if(input[i]=='|'){
            piped[it_process][j]='\0';
            it_process++;
            j=0;
            continue;
        }
        piped[it_process][j++] = input[i];
    }
    piped[it_process][j]='\0';
    int no_piped=it_process+1;

    if(no_piped==1){
        int retval = ind_command(output_cwd,hostname,username,input,homedir,-1,-1,1);
        return retval;
    }
    else{
        int org_out=dup(1);
        int org_in=dup(0);
        int temp;
        for(int i=0;i<no_piped;i++){
            int des[2];
            if (pipe(des)<0){
                perror("my bash: could not create pipe.");
                exit(0);
            }
            pid_t pid = fork();
            if(pid==0){
                if(i==0){
                    fflush(stdout);
                    dup2(des[1],1);
                    int retval = ind_command(output_cwd,hostname,username,piped[0],homedir,-1,des[1],org_out);
                }
                else if(i>=1 && i<no_piped-1){
                    fflush(stdout);
                    dup2(temp,0);
                    dup2(des[1],1);
                    int retval = ind_command(output_cwd,hostname,username,piped[i],homedir,temp,des[1],org_out);
                }
                else if(i==(no_piped-1)){
                    dup2(temp,0);
                    int retval = ind_command(output_cwd,hostname,username,piped[no_piped-1],homedir,temp,-1,org_out);
                }
                dup2(org_in,0);
                dup2(org_out,1);
                exit(0);
            }
            else{
                wait(NULL);
                temp = des[0];
                close(des[1]);
            }
        }
    }
} 

int and(char output_cwd[],char hostname[],char username[],char input[],char* homedir){
    // now i have to iterate through the input and separate the background and foreground processes
    
    char background[500][500];
    char foreground[500];
    char process[500][500];
    int it_process=0;
    int flag_fore=0; // flag is set when we have a foreground process
    int it_back=0;
    int j=0;

    for(int i=0;input[i]!='\0';i++){
        if(input[i]=='&'){
            process[it_process][j]='\0';
            it_process++;
            j=0;
            continue;
        }
        process[it_process][j++] = input[i];
    }
    process[it_process][j]='\0';

    int flag_fore_break=-1;
    for(int j=0;process[it_process][j]!='\0';j++){
        if(process[it_process][j]!='\t' && process[it_process][j]!=' '){
            flag_fore_break=1;
            break;
        }
    }

    if(flag_fore_break==1){
        strcpy(foreground,process[it_process]);
        flag_fore=1;
        it_process--;
    }
    else{
        foreground[0]='\0';
        it_process--;
    }
    
    for(int i=0;i<=it_process;i++){
        int flag_i=-1;
        for(int j=0;process[i][j]!='\0';j++){
            if(process[i][j]!='\t' && process[i][j]!=' '){
                flag_i=1;
                break;
            }
        }
        if(flag_i==1) strcpy(background[it_back++],process[i]);
        else{
            printf("my bash: syntax error near unexpected token '&'\n");
            return 1;
        }
    }

    if(flag_fore==1){
        for(int i=0;i<it_back;i++) background_process(output_cwd,hostname,username,background[i],homedir);
        int var = time(NULL);
        int retval = piping(output_cwd,hostname,username,foreground,homedir);
        return retval;
    }
    else if(flag_fore==0) for(int i=0;i<it_back;i++) background_process(output_cwd,hostname,username,background[i],homedir);
}


void SIG(char input[]){
    char *output = strtok(input,"\t ");
    int a;
    int b;
    output = strtok(NULL,"\t ");
    a = atoi(output);
    output = strtok(NULL,"\t ");
    b = atoi(output);
    for(int i=0;i<100;i++){
        if((i+1)==a){
            if(start[i].visited==1) kill(start[i].pid,b);
            else{
                printf("my bash sig: no job with the given number exits\n");
                return;
            }
        }
    }
}

void FG(char input[]){
    char *output = strtok(input,"\t ");
    output = strtok(NULL,"\t ");
    int a;
    a = atoi(output);
    for(int i=0;i<100;i++){
        if((i+1)==a){
            if(start[i].visited==1){
                int status;
                setpgid(start[i].pid, getpgid(0));
                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(0, start[i].pid);
                if (kill(start[i].pid, SIGCONT) < 0) perror("Error");
                waitpid(start[i].pid, &status, WUNTRACED);
                start[i].visited=0;
                tcsetpgrp(0, getpgid(0));
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);
                if(WIFSTOPPED(status)==1) start[i].visited=1;
            }
            else{
                printf("my bash fg: no job with the given number exits\n");
                return;
            }
        }
    }
}

void BG(char input[]){
    char *output = strtok(input,"\t ");
    output = strtok(NULL,"\t ");
    int a;
    a = atoi(output);
    for(int i=0;i<100;i++){
        if((i+1)==a){
            if(start[i].visited==1){
                char file_name[500]="/proc/";
                char string_pid[500];
                sprintf(string_pid,"%d",start[i].pid);
                strcat(file_name,string_pid);
                strcat(file_name,"/stat");
                FILE* fp = fopen(file_name,"r");
                char me;
                fscanf(fp,"%*s %*s %c",&me);
                if(me=='T'){
                    // have to convert the state from stopped to running here
                    if (kill(start[i].pid, SIGCONT) < 0) perror("Error");
                }
                else{
                    printf("my bash bg: the bg process is already running\n");
                    return;
                }
            }
            else{
                printf("my bash fg: no job with the given number exits\n");
                return;
            }
        }
    }
}

