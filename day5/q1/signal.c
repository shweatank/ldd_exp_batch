#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("usage: ./exe <pid>");
		return 1;
	}
	pid_t pid = atoi(argv[1]);

	printf("Singnaling the parent process\n");
	kill(pid,SIGINT);

	printf("Signal sent\n");
	return 0;
}
