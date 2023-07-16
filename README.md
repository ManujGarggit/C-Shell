# OSN

# ASSIGNMENT-2 and 3

> Name - Manuj Garg

> Roll No - 2021101047

> To compile the shell run `make` command in the terminal where the Makefile is present and then execute `./a.out`  

> FOR EXITING THE TERMINAL type "q" in the input or press ctrl+d 

> The error message are displayed in red

------ 

### SPEC - 1 (prompt in main.c)
- Displayed prompt similar to what is shown in the original terminal
- Directory from which the shell is invoked is (symbolized as ~) and the all the other directories are shown relative to it 

### SPEC - 2 (echo in echo.c , cd  in cd.c , pwd in pwd.c)

- Implemented echo with the handling of spaces and tabs while it can't handle when we give ';' or '&' in the words as that will be treated as separate processes
- Does not handle code for environment variables
- Implemented pwd to show the path of the current wokring directory
- In case of extra varible no error will be printed as like the original terminal
- Implemented cd to go in or out of directories 
- If invalid dir is given "No such file or dir"
- If invalid file "No such dir" is printed
- Cd takes only one argument apart from cd ("too many arguments error")
- In case of (cd-) if prev dir is not set error will be printed
- For this the code doesn't req the use of execvp

### SPEC - 3 (ls in ls.c)

- Implemented ls commands for multilple directories and files with the flags 
- The ouptut format will be the same as that of ls in org terminal when multiple files or dir is given . Files will be listed first in alphabetical order followed by directories in alphabetical order
- The contents in the directories will also be sorted out in the alphabetical order
- The directories will be printed in blue , exec files in green , other files in white
- For invalid files or dir "No such file or dir" is printed
- If a flag other than the flags mentioned in the pdf is given then "invalid arguments" will be shown

### SPEC - 4 (fg and bg processe in processes.c)

- If multiple foreground processes are given then the time for the next prompt to pop up will be the summation of time took for individual and this time will be displayed in the prompt
- Time will be printed if the time it took for the foregoround process will be more than 1 second
- The invalid commands for both the foreground and background are handled
- In the background case the pid for the invalid command will be assigned and invalid command line will be printed and on pressing enter new prompt will show with the line that displays exit status of the invalid command

### SPEC - 5 (pinfo in pinfo.c)

- In pinfo , process status is read from "/proc/"<self/pid>/stat" file and the foreground is checked by comparing the values of pgrp and tpgid 
- When they are equal the process will be foreground
- The virtaul mem is read from "/proc/"<self/pid>/statm" as the man page tells that it gives the virtual memory size of the program (unit is not printed)
- The executable file is read from "/proc/"<self/pid>/exec" and the absolute path is obtained from readlink and converted to relative path wrt to '~'
- For invalid pid(s) "invalid pid" error will be printed

### SPEC - 6 (finished background processes in main.c)

- At the completion of the background process the statement that will come up will be "process <process_name> with pid <pid> exited normally/abnromally"
- When we press enter or input a command and press enter then whosoever bg processes are completed that one with the mentioned output format for the completion of bg process will be printed

### SPEC - 7 (discover in discover.c)

- The input for the target directory , the flags , and the file name can be given in any order
- The file or directory which we wish to find should be given in quotes and the name for the file or directory should be mentioned not the relative path as mentioned in the pdf.
- This command doesn't handle -df/-fd flag as this type of flag is not given in pdf
- Multiple target directories , invalid flags , multiple file names are handled and app errors will be printed
- If we wish to find a file/dir and -f is given only files will be compared and in case of -d only dir are compared and if matching are found the name will be printed wrt to target dir and if we can't find anything error will be printed as no results found 
- If we execute disocver in the '/' directory segmentation fault will occur as in this some folders and files may not have the read permission so we can't go into them (by chdir)
- In case of my computer,some internal linking is there as when i execute discover /home/dell command with a particular file to find it is printing the files multiple times but otherwise it is totallty working fine . Had it been the error in the loop function nothing would have been shown as it would be stuck in the loop . 

### SPEC - 8 (history in main.c)

- For this command a file history.txt is created at the root of the user.
- Printing the last 10 commands when we type history and storing the most recent 20 commands and updating it when more than 20 distinct commands is given.

----

### SPEC - 1 (redirection of input in processes.c)

- An error message will be displayed if the input file does not exist.
- The output file will be created (with permissions 0644)if it does not already exist.
- In case the output file already exists, it will be overwritten in case of > and appended to in case of >>.
- Multiple inputs and outputs are not handled
- If there is a symbol of input or ouptut redirection assumed that there are only input or output files given after that (no text given after that to redirect eg(echo hi > a.txt hello))

### SPEC - 2 (piping in processes.c)

- Pipleines are handled thorugh forking the processes 
- If there is a invalid file in between then the error msg will be displayed thorugh expv

### SPEC - 3 (piping+redirection in processes.c)

- Implemented i/o redirection with pipes
- If in between we have the redirection to an input file and also the input is piped to a command the input will go the input file and not to the next piped command
- Invalid commands are handled through printing errors

### SPEC - 4 (jobs in main.c)

- Implemented jobs which give the description of the bg processes with the indexes(this is the order in the bg processes are assigned)
- In jobs the commands are sorted according to their full names so commands (<sleep 5> and < sleep 5> will be sorted differntly)
- Sent a kill signal in case of sig
- Fg and Bg have the usual functioning
- In the pdf it is given on doing bg on vim it becomes running but this is not implemented as this was not happening.

### SPEC - 5 (signal handling in main.c)

- On pressing Ctrl+d the terminal will be exited out
- On pressing Ctrl+c the current foregorund process will be stopped and nothing will happen if there is no foreground process running
- On pressing Ctrl+z the current fg process will be sent to the background and error printing will be ther if there is no foreground process running

### SPEC - 6 (autocompletion in main.c)
 
- Autocompletion have the usual functioning
- The directories are dispalyed in blue while the files are displayed in plain white for the printing part
- This will also handle if we give the files or directories to search in a folder using absolute path of that folder




