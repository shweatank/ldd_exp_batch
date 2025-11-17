#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s pid\n", argv[0]);
        return 1;
    }


    pid_t pid = atoi(argv[1]);
    printf("pid: %d",pid);

    if (kill(pid, SIGTERM) == -1)
    {
        perror("Kill");
        return 1;
    }

    printf("Signal Sent");
    
    return 0;
}
