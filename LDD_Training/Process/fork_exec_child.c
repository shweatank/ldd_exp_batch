#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

	printf("Parent Process started. PID = %d\n", getpid());

	pid_t pid;
	pid = fork();
	pid = fork();
	
	if(pid<0) {
		perror("fork");
		return 1;
	}
	else if(pid == 0) {
		printf("Child Process created. PID = %d, PPID = %d\n", getpid(), getppid());
		execlp("./hello", "./hello", NULL);
		printf("Child exit\n");

	}
	else {
		printf("Parent PID = %d\n", pid);
		wait(NULL);
		printf("Parent exiting.\n");
	}
	return 0;
}

