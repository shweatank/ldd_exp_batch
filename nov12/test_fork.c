#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
   pid_t pid;
   printf("parent process started PID = %d\n",getpid());
   
 pid = fork();  // create new process
   if( pid< 0)
   { // fork failed
      perror("fork failed\n");
      return 1;
   }
    else if(pid == 0)
     {  // child process
        printf("child  process started PID = %d ,parent PID = %d \n",getpid(),getppid());

         //replace child with new program
        // execlp(program_name, arg 0,arg2 .... ,NULL); 
      // execlp("ls", "ls", "-l", NULL);
       execlp("./a.out", "welcome","-l", NULL);
       // if execl fails print error
       perror("execlp failed");
       }
     else{
        // parent process
          printf("parent waiting for child (PID == %d)..",pid);
          wait(NULL);  // wait for child to finish
          printf("child completed . parent exiting\n");
              }


//     pid_t pid;
/*     printf("parent process started PID = %d\n",getpid());
     pid = fork();  // create new process
     if( pid< 0)
     { // fork failed
        perror("fork failed\n");
        return 1;
     }
      else if(pid == 0)
       {  // child process
          printf("child  process started PID = %d ,parent PID = %d \n",getpid(),getppid());
  
         execlp("ls", "ls", "-l", NULL);
         // if execl fails print error
         perror("execlp failed");
         }
       else{
          // parent process
            printf("parent waiting for child (PID == %d)..",pid);
            wait(NULL);  // wait for child to finish
            printf("child completed . parent exiting\n");
               }
       */

return 0;

}
