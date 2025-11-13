#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int a = 10;
int main()
{
   pid_t pid;
// pid_t pid2;
   printf("parent process started PID = %d\n",getpid());
   
   pid = fork();
   //pid2 = fork();

   if( pid < 0)
   { // fork failed
      perror("fork failed\n");
      return 1;
   }
    else if(pid == 0)
       {  // child process
        printf("child  process started PID = %d ,parent PID = %d \n",getpid(),getppid());
        a = 20;
        printf(" child valu = %x  %d\n",&a,a);
       }
     else{
        // parent process
           printf(" parent valu = %x  %d\n",&a,a);

          printf("parent waiting for child (PID == %d)..",pid);
          wait(NULL);  // wait for child to finish
          printf("child completed . parent exiting\n");
          }

return 0;

}
