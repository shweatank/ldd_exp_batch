/*
Write a program that demonstrates orphan process
*/
 
#include <stdio.h>
#include <sys/types.h>	//defines data types used by system calls
#include <unistd.h>		//for POSIX system calls 
#include <stdlib.h>		//for mem management(malloc), process control(exit), type converstion(atoi), search sort, macros

int main() {
	pid_t pid;
	pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (pid == 0) {
		printf("Child before sleep PPID = %d and PID = %d\n", getppid(), getpid());
		sleep(10);
		printf("Child after sleep PPID = %d and PID = %d\n", getppid(), getpid());
		printf("Child Exited\n");
		return EXIT_SUCCESS;
	}
	else {
		printf("Parent started. PPID = %d and PID = %d\n", getppid(), getpid());
		printf("Parent Exited\n");
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
