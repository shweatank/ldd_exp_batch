#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	int fd[3];
	char msg[]="hello world \n";
	char read_buf[20];
	if(pipe(fd)==-1)
		printf("failed to create the pipe\n");
	pid_t pid;
	pid=fork();
	if(pid)
	{
		printf("inside the parent \n");
		close(fd[0]);
		write(fd[1],msg,strlen(msg));
		close(fd[1]);
	}
	else
	{
		printf("inside the child\n");
		close(fd[1]);
		read(fd[0],read_buf,sizeof(read_buf));
		printf("the msg inside it is %s\n",read_buf);
		close(fd[0]);
	}
}
