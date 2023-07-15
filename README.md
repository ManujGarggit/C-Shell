# OSN

# ASSIGNMENT-3

> Name - Manuj Garg

> Roll No - 2021101047

> FOR EXITING THE TERMINAL type "q" in the input or press ctrl+d 

> The error message are displayed in plain white 

### SPEC - 1 

- An error message will be displayed if the input file does not exist.
- The output file will be created (with permissions 0644)if it does not already exist.
- In case the output file already exists, it will be overwritten in case of > and appended to in case of >>.
- Multiple inputs and outputs are not handled
- If there is a symbol of input or ouptut redirection assumed that there are only input or output files given after that (no text given after that to redirect eg(echo hi > a.txt hello))

### SPEC - 2 

- Pipleines are handled thorugh forking the processes 
- If there is a invalid file in between then the error msg will be displayed thorugh expv

### SPEC - 3

- Implemented i/o redirection with pipes
- If in between we have the redirection to an input file and also the input is piped to a command the input will go the input file and not to the next piped command
- Invalid commands are handled through printing errors

### SPEC - 4

- Implemented jobs which give the description of the bg processes with the indexes(this is the order in the bg processes are assigned)
- In jobs the commands are sorted according to their full names so commands (<sleep 5> and < sleep 5> will be sorted differntly)
- Sent a kill signal in case of sig
- Fg and Bg have the usual functioning
- In the pdf it is given on doing bg on vim it becomes running but this is not implemented as this was not happening.

### SPEC - 5 

- On pressing Ctrl+d the terminal will be exited out
- On pressing Ctrl+c the current foregorund process will be stopped and nothing will happen if there is no foreground process running
- On pressing Ctrl+z the current fg process will be sent to the background and error printing will be ther if there is no foreground process running

### SPEC - 6
 
- Autocompletion have the usual functioning
- The directories are dispalyed in blue while the files are displayed in plain white for the printing part
- This will also handle if we give the files or directories to search in a folder using absolute path of that folder




