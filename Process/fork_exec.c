/*
   Write a program that takes a command as a CLA.
   The program shoudl fork a child process that uses exec function to run command.
   The parent should wait for child and print its exit status.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <command>\n", argv[0]);
		return EXIT_FAILURE;
	}
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (pid == 0) {
		printf("Child started\n");
		execvp(argv[1], argv+1);
		printf("Child exited\n");
		return EXIT_SUCCESS;
	}
	else {
		printf("Parent Started. Waiting for child...\n");
		int waitStatus;
		wait(&waitStatus);
		printf("Child exited with status %d\n", waitStatus);
		printf("Parent exited\n");
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
