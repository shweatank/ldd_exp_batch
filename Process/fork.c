/*
Write a program that creates exactly one child using fork.
Both parent and child should print their pid and ppid.
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
		printf("I am child. My PPID = %d and PID = %d\n", getppid(), getpid());
		printf("Child Exiting\n");
		return EXIT_SUCCESS;
	}
	else {
		printf("I am Parent. My PPID = %d and PID = %d\n", getppid(), getpid());
		printf("Parent Exiting\n");
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
