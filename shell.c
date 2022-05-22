#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <time.h>


int nonBuiltIn(char * args[], int i);

int main(int argc, char **argv) {
// Set the prompt
char prompt[256] = "";
char input[256];
char * cmd[256];
int i = 0;
int k = 0;



//if multiple arguments
if (argc > 1 ) {
  //if prompt argument given then set prompt
  if (!strcmp(argv[1],"-p")) {
    strcpy(prompt,argv[2]) ;
  }
  else{
    printf("%s\n","Error invalid syntax");
  }

}
//otherwise set prompt to default
else{
    strcpy(prompt,"shell>") ;
}
// User input infinite loop

while (1) {
  i = 0;
  k = 0;
  //print out inital prompt
  printf("%s",prompt);

  // Get user input
 fgets(input,256,stdin);

 //remove the new line from fgets by replacing with \0
input[strlen(input) - 1] = '\0';

//now parse the input word for word and store in cmd[] array
cmd[0] = strtok(input," ");
while (cmd[i] != NULL){
    i++;
    cmd[i] = strtok(NULL," ");
}
//now move data from cmd[] to a fixed array of the size of input words called: cmdfixed[]
  char * cmdfixed[i+1];
   while (k < i)
   {
     cmdfixed[k] = cmd[k];
     k++;
   }
   cmdfixed[k] = '\0';

  // ---------- Builtin Commands ---------------------------------------
  //if user wants to exit, then exit
  if (!strcmp(input, "exit") == 1) {
    return 0;
  }
  //gets the pid
  else if (!strcmp(input, "pid") == 1) {
    printf("Shell Process ID: %d\n", getpid());

  }
  //gets parent pid
  else if (!strcmp(input, "ppid") == 1) {
    printf("Parent Process ID: %d\n", getppid());

  }
//cd command
  else if (!strcmp(cmd[0], "cd") == 1) {
    //if just cd with no parameters, default to home directory
    if (cmd[1] == NULL){
      chdir(getenv("HOME"));
    }
    //else grab second parameter to cd to
    else{
        chdir(cmd[1]);
    } 
  }
  else if (!strcmp(input, "pwd") == 1) {
    char currDir[256];
    printf("%s\n", getcwd(currDir,sizeof(currDir)));

  }
  else {
  // ---------- Non-Builtin Commands--------------------------
  //Here we call external commands
   nonBuiltIn(cmdfixed,k);
  }
}
return 0;
}
// ----------------------------------- Helper Functions ----------------------------------
int nonBuiltIn(char * args[], int i) {
  //here we hold our return status
  int status;
  int statush;


  //fork for non built in command
  int pid = fork();

  if (pid == 0) {
    printf("[%d] %s\n",getpid(),args[0]);
    //if the final parameter is a '&' remove it before executing the command
    if (*args[i-1] == '&'){ args[i -1 ] = '\0';}

    //execute the command
    execvp(args[0],args);
    //print Error if failed and kill
    printf("Command %s failed to execute.\n",args[0]);
    kill(pid,SIGTERM);
  }
  //wait for child to finish then continue to new prompt
   else if( pid != 0){
     //if '&' parameter was used, dont wait for child process to finish
     if (*args[i-1] == '&')
     {
        int test = waitpid(pid,&status,WNOHANG);         
     }

     //otherwise wait for child to finish
     else{
       printf("%d %d %d ",pid, getpid(),getppid());

       waitpid(pid,&status,0);
     }
     
     //print exit status of child
     if (WIFEXITED(status)) {
        printf("[%d] %s exited with status %d\n",pid,args[0] ,WEXITSTATUS(status));
      }

   }
    return 0;
   

}
