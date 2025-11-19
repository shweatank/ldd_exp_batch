/*
   execl	execl(path, arg0, arg1, ..., NULL)	-------->  List of arguments, NULL-terminated
   execv	execv(path, argv)					-------->  Array of arguments (argv[])
   execle	execle(path, arg0, ..., NULL, envp)	-------->  Arguments + environment array
   execve	execve(path, argv, envp)	        -------->  Most general, used by kernel; argv and envp arrays
   execlp	execlp(file, arg0, ..., NULL)	    -------->  Searches $PATH to find file
   execvp	execvp(file, argv)	                -------->  Array of arguments, searches $PATH
   l → list of arguments
   v → vector (array) of arguments
   p → search $PATH for executable
   e → specify environment
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t pid = fork();

	if (pid < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (pid == 0) {
		printf("child Started.\n");

		//execl("/bin/ls", "ls", "-l", NULL);

		/*
		char *args[] = {"ls", "-l", NULL};
		execv("/bin/ls", args);
		 */

		/*
		char *envp[] = {"MYVAR=HelloWorld", "PATH=/usr/bin", NULL};
		char *args[] = {"env", NULL};
		execve("/usr/bin/env", args, envp);
		*/

		/*
		char *envp[] = {"MYVAR=HelloWorld", "PATH=/usr/bin", NULL};
		execle("/usr/bin/env", "env", NULL, envp);
		*/

		/*
		char *args[] = {"ls", "-l", NULL};
        execvp("ls", args);
		*/

		execlp("ls", "ls", "-l", NULL);

		perror("exec");
		return EXIT_SUCCESS;

	}
	else {
		printf("Parent Started. Waiting for child to exit\n");
		wait(NULL);
		printf("Child exited\n");
		printf("Parent exited\n");
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
