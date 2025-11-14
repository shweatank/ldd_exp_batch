#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char*argv[])
{
    if(argc!=2)
       {
        fprintf(stderr,"usage : %s <pid>\n",argv[0]);
        return 1;
       }

pid_t pid = atoi(argv[1]);
printf("sending SIGTERM to process %d...\n",pid);
if(kill(pid,SIGTERM) == -1)
{
   perror("kill");
   return 1;
}

printf("signal sent\n");
return 0;
}
