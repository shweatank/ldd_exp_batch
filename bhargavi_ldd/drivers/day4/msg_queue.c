#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/msg.h>
#include<sys/ipc.h>
struct msg
{
	long mtype;
	char buf[20];
}m;
int main(int argc, char *argv[])
{
	int id;
	id=msgget(1,IPC_CREAT | 0600);
	printf("the process id is %d\n",getpid());
	memset(m.buf,0,sizeof(m.buf));
	while(1)
	{
		printf("the enter the message to send\n");
		fgets(m.buf,sizeof(m.buf),stdin);
		m.mtype=atoi(argv[1]);
		msgsnd(id,&m,strlen(m.buf),0);
		printf("the message sent is %s\n",m.buf);
		return 0;
	}
}
