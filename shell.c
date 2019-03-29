#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 


#define MAXLINE 512

void parsecmd(char *subcmds, char **args) //parsing the command
{

  static char* delimiter = " \n\t";
  char *token = strtok(subcmds, delimiter);
  while (token != NULL) {
    *args++ = token;
    token = strtok(NULL, delimiter);
  }
  *args = (char *)'\0';                 // end of argument list
}

void parseline(char *line, char **subcmds) //parsing the line into separate commands
{
    const char *delimiter = ";";
    int i = 0;
    char *token = strtok(line, delimiter);
    while (token != NULL) {
      subcmds[i++] = token;
      token = strtok(NULL, delimiter);
    }
    subcmds[i] = (char *) '\0';
}

void execute(char *line)  //main execution function
{
     char *subcmds[MAXLINE]={0};
     char *args[MAXLINE]={0};
     pid_t child;   //child pid
     int cstatus;   // exit status of child
     pid_t wpid;       // pid of child to be returned by wait
     bool quit = false; //quit flag

     parseline(line, subcmds); //parse line to sub commands

     int i;
     for (i = 0; subcmds[i]; ++i){

       parsecmd(subcmds[i], args); //parse commands into arguments

       if (args[0] == NULL)   // skip empty commands
         continue;

       if (strcmp(args[0], "quit") == 0) { // if quit is input
         quit = true; // set quit flag to true
         continue;
       }

       if ((child = fork()) == 0) { // fork child process
         execvp(args[0], args);   //execute child command
         fprintf(stderr, "Exec failed on command %s.\n", args[0]);
         exit(1);
       }
     }
     if (child == (pid_t)(-1)) {      // parent process
       fprintf(stderr, "Fork failed.\n");
     }
     else {
       while ((wpid = wait(&cstatus)) > 0); // wait for child processes to complete
     }
     if(quit)
       exit(0); // quit after all processes have completed
}

int main(int argc, char *argv[])
{
  char line[MAXLINE]={0};
  if(argc == 2){    //batch mode (no prompt)
    printf("FILE IS: %s, argv[1] is %s\n", argv[1], argv[1]);
    FILE *fr;
    if ((fr = fopen (argv[1],"r")) == NULL) {
      fprintf(stderr, "fopen(): No such file or directory\n");  //no file
      exit(1);
    }
    while (fgets(line, sizeof(line), fr)) { //reading from file
      execute(line);  //execute the line
    }
  }
  else if(argc == 1){ //interactive mode
    printf("prompt> "); //prompt for input
    while (fgets(line, sizeof(line), stdin)) {  //reading from stdin
      execute(line);  //execute the line
      printf("prompt> ");
    }
  }
  else {
    fprintf(stderr, "Usage: tinysh [batchFile]\n"); //incorrect usage
    exit(1);
  }
  return 0;
}
