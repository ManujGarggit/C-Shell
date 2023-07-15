#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <termios.h>
#include <ctype.h>


#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "history.h"
#include "discover.h"
#include "process.h"
#include "pinfo.h"

struct bg_check{
    int visited;
    // it checks whether a process is being running or not 
    // if 0 not any process at this place
    // if 1 a process is at that place
    char name[500];
    pid_t pid;
    char full_name[500];
};

typedef struct bg_check bgCheck;

char output_cwd_prev[500];
int flag;
int count_discover;
int count_before;
int global_foreground;
int index_exit;

bgCheck start[100];

bgCheck foreground_current;

bgCheck exit_queue[100];


#endif
