/*--------------------------------------------------------------------

File: dp.c
Student name(s): William Tran-Viet  
Student id(s): 300241608
Group no:  N/A

Description:  Double pipe program.  To pipe the output from the standard
          output to the standard input of two other processes.
          Usage:  dp  <cmd1> : <cmd2> : <cmd3>
          Output from process created with cmd1 is piped to
          processes created with cmd2 and cmd3

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
/* prototypes */
int doublePipe(char **,char **,char **);

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{

   int i,j;         /*indexes into arrays */
   char *cmd1[10];  /*array for arguments of first command */
   char *cmd2[10];  /*array for arguments of second command */
   char *cmd3[10];  /*array for arguments of third command */
   if(argc == 1)
   {
     printf("Usage: dp <cmd1 arg...> : <cmd2 arg...> : <cmd3 arg....>\n");
     exit(1);
   }


   /* get the first command */
   for(i=1,j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found first command */
      cmd1[j]=argv[i];
   }
   cmd1[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only one command found\n");
      exit(1);
   }
   else i++;

   /* get the second command */
   for(j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found second command */
      cmd2[j]=argv[i];
   }
   cmd2[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only two commands found\n");
      exit(1);
   }
   else i++;

   /* get the third command */
   for(j=0 ; i<argc ; i++,j++) cmd3[j]=argv[i];
   cmd3[j]=NULL;
   if(j==0) /* no command after last : */
   {
      printf("Bad command syntax - missing third command\n");
      exit(1);
   }
   exit(doublePipe(cmd1,cmd2,cmd3));
}

/*--------------------------------------------------------------------------
  ----------------- You have to implement this function --------------------
  --------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

int doublePipe(char **cmd1, char **cmd2, char **cmd3)
{ 
   int fd[2]; // pipe from cmd1 -> cmd2
   pipe(fd);
   int fdn[2]; // pipe from cmd1 -> cmd3
   pipe(fdn);

   pid_t pid1 = fork();
   if(pid1 < 0){
      return 2;
   }
   if (pid1 == 0){ // child 1 (cmd1)
      dup2(fd[1], 1); // write end cmd1 -> cmd2
      close(fd[0]); // read end cmd1 -> cmd2
      close(fdn[0]); // read end cmd1 -> cmd3
      close(fdn[1]);
      execvp(cmd1[0], cmd1); 
   }

   pid_t pid1a = fork();

   if(pid1a < 0){
      return 2;
   }
   if(pid1a == 0){
       dup2(fdn[1], 1); // write end cmd1 -> cmd3
       close(fd[0]);
       close(fd[1]);
       close(fdn[0]);
       execvp(cmd1[0], cmd1);
   }

   int pid2 = fork();
   if(pid2 < 0){
      return 2; 
   }
   if(pid2 == 0){ // child 2 (cmd2)
      dup2(fd[0], 0); // read from cmd1
      close(fd[0]);
      close(fd[1]);
      close(fdn[0]);
      close(fdn[1]);
      execvp(cmd2[0], cmd2);
   } 

   int pid3 = fork();
   if(pid3 < 0){
      return 2;
   }
   if(pid3 == 0){
      close(fd[0]);
      close(fd[1]);
      dup2(fdn[0], 0); // read from cmd1 
      close(fdn[0]);
      close(fdn[1]);
      execvp(cmd3[0], cmd3);
   }

   waitpid(pid1, NULL, 0);
   waitpid(pid1a, NULL, 0);
   waitpid(pid2, NULL, 0);
   waitpid(pid3, NULL, 0);

   close(fd[0]);
   close(fd[1]);
   close(fdn[0]);
   close(fdn[1]);
   return 0;
}
