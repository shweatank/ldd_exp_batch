/* fork_example.c */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        /* child */
        printf("Child: pid=%d, parent pid=%d\n", getpid(), getppid());
    } else {
        /* parent */
        printf("Parent: child pid=%d, pid=%d\n", pid, getpid());
	wait(NULL);
	printf("hello world\n");
    }

    return 0;
}

