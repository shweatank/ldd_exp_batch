#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
	pid_t pid;
	printf("parent process started,PID=%d\n",getpid());
	pid=fork();
	if(pid<0)
	{
		//fork failed
		perror("fork failed\n");
		return 1;
	}
	else if(pid==0)
	{
		//child process
		printf("child prosess created,PID=%d, parent PID=%d\n",getpid(),getpid());
		//replace child with new program
		//execlp(program_name,arg0,arg1.....NULL);
		execlp("./hello", "./hello", NULL);
		wait(NULL);
		//if execlp fails,print error
		perror("execlp failed");
	}
	else
	{
		//parent process
		printf("parent waiting for child(PID=%d),,,,\n",pid);
	      	//wait for child to finish
		printf("child completed,parent exiting.\n");
	}
        printf("parent process started,PID=%d\n",getpid());
}

