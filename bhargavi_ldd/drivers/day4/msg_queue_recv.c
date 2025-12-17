#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h> 
struct msg
{
	long mtype;
	char buf[20];
}m;
int main(int argc, char *argv[])
{
	int id;
	id=msgget(1, 0666);
	if(id==-1)
	{
		printf("failed to connect \n");
		exit(1);
	}
	memset(m.buf,0,sizeof(m.buf));
	m.mtype=atoi(argv[1]);
	while(1)
	{
		printf("the recived message is \n");
		msgrcv(id,&m,sizeof(m.buf),m.mtype,0);
		printf("the recived message is %s\n",m.buf);
	}
	return 0;
}
