#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    printf("Parent process started. PID = %d\n", getpid());
    
    pid = fork();
    
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Child process created. CHild PID= %d, Parent PID = %d\n", getpid(), getppid());
        execlp("./fork2.c", "gcc", "fork2.c", "-o", "fork2.o", "./fork2", NULL);
        // execlp("sh", "sh", "-c", "gcc fork2.c -o fork2.o && ./fork2.o", NULL);

        // execlp("ls", "ls","-l", NULL);
        perror("execlp failed");
    }
    else
    {
        printf("Parent waiting for child (PID = %d)...\n", pid);
        wait(NULL);
        printf("Child Completed. Parent Exiting.\n");
    }

    return 0;
}
