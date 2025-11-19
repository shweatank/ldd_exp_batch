/*
   Process Family tree:
   Write a program to create a process of depth 3.
   Each process should print pid, ppid and its level.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <stdlib.h>

int main() {

	pid_t pid;
	pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (pid == 0) {
		pid_t pid1 = fork();
		if(pid1 < 0) {
			perror("fork");
			return EXIT_FAILURE;
		}
		else if (pid1 == 0) {
			printf("-------Level 3-------\n");
			printf("PPID = %d\n", getppid());
			printf("PID = %d\n", getpid());
			return EXIT_SUCCESS;
		}
		else {
			printf("-------Level 2-------\n");
			printf("PPID = %d\n", getppid());
			printf("PID = %d\n", getpid());
			wait(NULL);
			return EXIT_SUCCESS;
		}
	}
	else {
		printf("-------Level 1-------\n");
		printf("PPID = %d\n", getppid());
		printf("PID = %d\n", getpid());
		wait(NULL);
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}

