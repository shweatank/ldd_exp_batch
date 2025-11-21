#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid;

	printf("Parent Process started. PID = %d\n", getpid());

	pid = fork();
	
	if(pid<0) {
		perror("fork");
		return 1;
	}
	else if(pid == 0) {
		printf("Child Process created. PID = %d, PPID = %d\n", getpid(), getppid());

		execlp("./hello", "./hello", NULL);

		perror("execlp failed\n");
	}
	else {
		printf("Parent waiting for child PID = %d\n", pid);
		wait(NULL);
		printf("Child completed. Parent exiting.\n");
	}

	printf("Parent Process started. PID = %d\n", getpid());

	pid = fork();

	if(pid<0) {
		perror("fork");
		return 1;
	}
	else if(pid == 0) {
		printf("Child Process created. PID = %d, PPID = %d\n", getpid(), getppid());

		execlp("./hello", "./hello", NULL);

		perror("execlp failed\n");
	}
	else {
		printf("Parent waiting for child PID = %d\n", pid);
		wait(NULL);
		printf("Child completed. Parent exiting.\n");
	}
	return 0;
}

