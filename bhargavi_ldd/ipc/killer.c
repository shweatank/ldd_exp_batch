#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);  // fixed fprintf syntax
        return 1;
    }

    pid_t pid = atoi(argv[1]);   // convert argument to integer (PID)
    printf("Sending SIGTERM to process %d...\n", pid);

    // Corrected kill() usage and parentheses
    if (kill(pid, SIGTERM) == -1)
    {
        perror("kill");  // print why it failed
        return 1;
    }

    printf("Signal sent successfully!\n");
    return 0;
}

