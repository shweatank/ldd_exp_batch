#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Usage: %s <pid>\n", argv[0]);
		return -1;
	}
	
	pid_t pid = atoi(argv[1]);

	printf("Sending signal to %d.......\n",pid);
	if(kill(pid, SIGTERM) == -1) {
		perror("kill");
		return -1;
	}
	printf("Signal Sent\n");
	return 0;
}
