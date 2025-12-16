#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main()
{
    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        close(fd[1]);
        char buff[20];
        read(fd[0], buff, sizeof(buff));
        printf("%s\n", buff);
    }
    else
    {
        close(fd[0]);
        char buff[] = "hello world";
		
        write(fd[1], buff, strlen(buff) + 1);
    }
}

