#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
int pid;
void funr(){
	printf("Signal received, Woke up %d\n",pid);

	kill(pid,SIGTERM);
}

int main(){
	pid = getpid();
	printf("process pid: %d \n",pid);
	//printf("This process is going to get killed, byebye\n");
	signal(SIGTERM,funr);
	while(1){}
	return 0;

}
