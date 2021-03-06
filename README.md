Design overview
---------------
In main, I first determine whether to use batch mode or interactive mode by detecting
the number of arguments and attempting to open the file to read from if one is provided.
The only difference between batch and interactive besides the source to read from (the 
file or stdin) is the use of a prompt in interactive mode.

I created two parsing functions, one is for parsing the line into separate commands
(parseline) and the other parses the sub commands into arguments ready to be executed
(parsecmd). These both utilize strtok.

The largest and main function I created is the execute function, which takes an input 
line and utilizes the parse functions to create arguments. I then use fork(), execvp(),
and wait() to create child processes for each command and execute them concurrently.

I handled the quit utility with a boolean flag (quit) that is set to true when "quit" was
input. Then, it would exit the program after all the child processes had completed if the
flag was set to true.



Complete specification
----------------------
Any empty commands are set to NULL and are skipped over in the loop with "continue", thus 
ignoring empty command lines, extra white space or semi-colons. My shell returns an error
message to stderr if the command does not exist or cannot be processed and continues to
run. I also print a message for any error with the batch use format or with opening the 
file to stderr.
