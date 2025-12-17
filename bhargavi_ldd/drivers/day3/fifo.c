#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#define fifo_name "myfifo"
int main()
{
	int fd;
	char buf[100];
	printf("enter the message to write in to the buffer \n");
	//scanf("%s",buf);
	fgets(buf, sizeof(buf), stdin);
	mkfifo(fifo_name , 0666);
	pid_t pid;
	pid=fork();
	if(pid)
	{
		printf("inside the parent \n");
		fd=open(fifo_name, O_WRONLY);
		if(fd==-1)
		{
			printf("faield to open the fifo in the write only mode \n");
		}
		write(fd,buf,sizeof(buf));
		close(fd);
	}
	else
	{
		printf("inside the child process \n");
		fd=open(fifo_name, O_RDONLY);
		if(fd==-1)
		{
			printf("failed to open the fifo from the read end \n");
			exit(0);
		}
		read(fd,buf,sizeof(buf));
		printf("the mesage inside the buffer is %s\n",buf);
		close(fd);
	}
	return 0;
}
