#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid;

	int var = 10;
	printf("Parent Process started. PID = %d\n", getpid());
	printf("addr var = %x\n", &var);
	pid = fork();
	
	if(pid<0) {
		perror("fork");
		return 1;
	}
	else if(pid == 0) {
		printf("Child Process created. PID = %d, PPID = %d\n", getpid(), getppid());
		var = 30;
		printf("child var = %d : %x\n", var, &var);
		return 0;

	}
	else {
		printf("Parent waiting for child PID = %d\n", pid);
		var = 20;
		printf("parent var = %d : %x\n", var, &var);
		wait(NULL);
		printf("parent var = %d : %x\n", var, &var);
		printf("Child completed. Parent exiting.\n");
	}
	return 0;
}
