#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]){
	
	if(argc < 2){
		printf("usage: <pid>");
		return 0;
	}
	pid_t pid = atoi(argv[1]);
	int ret = kill(pid,SIGTERM);
	if(ret == -1){
		perror("signal not sent\n");
		return 1;
	}

	printf("signal sent\n");
	return 0;


}
