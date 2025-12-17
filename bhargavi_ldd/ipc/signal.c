#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Function to handle SIGINT (Ctrl + C)
void sigint_handler(int sig)
{
    printf("\nCaught signal %d (SIGINT)\n", sig);
    printf("You pressed Ctrl + C, but I'm not exiting yet!\n");
}

int main()
{
    // Register signal handler
    signal(SIGINT, sigint_handler);

    printf("Program running... Press Ctrl + C to trigger SIGINT.\n");

    while (1)
    {
        printf("Working...\n");
        sleep(1);
    }

    return 0;
}


