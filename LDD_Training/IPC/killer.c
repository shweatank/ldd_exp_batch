#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Usage: %s <pid>\n", argv[0]);
		return 1;
	}

	pid_t pid = atoi(argv[1]);

	printf("Sending SIGTERM to process %d.....\n", pid);

	if(kill(pid, SIGINT) == -1) {
		perror("kill");
		return 1;
	}

	printf("Signal sent!\n");

	return 0;

}
	
