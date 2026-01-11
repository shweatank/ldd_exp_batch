#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

	int pipe_fd[2];
	
	pipe(pipe_fd);

	pid_t pid = fork();

	if(pid > 0) {

		close(pipe_fd[0]);
		write(pipe_fd[1], "Hello...", 9);
		
		printf("Parent wrote = %s\n", "Hello...");

		return 0;
	}
	else if(pid == 0) {
		close(pipe_fd[1]);
		char str[10];
		read(pipe_fd[0], str, 9);

		printf("Child Read = %s\n", str);

		return 0;
	}
	else {
		printf("Error in Forking\n");
	}

	return 0;

}
		

