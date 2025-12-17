#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    char msg[] = "hello bhargavi";
    char buf[20];

    if (pipe(fd) == -1)   // create pipe
    {
        printf("Unable to create pipe\n");
        exit(1);
    }

    pid_t pid = fork();  // create child process

    if (pid > 0)   // Parent process
    {
        printf("Inside the parent\n");

        close(fd[0]); // close read end

        write(fd[1], msg, strlen(msg) + 1);  // send message with null terminator

        close(fd[1]); // close write end
    }
    else if (pid == 0)   // Child process
    {
        printf("Inside the child\n");

        close(fd[1]); // close write end

        read(fd[0], buf, sizeof(buf));  // read message
        printf("The buffer copied from parent to child is: %s\n", buf);

        close(fd[0]); // close read end
    }
    else
    {
        printf("Fork failed\n");
    }

    return 0;
}

