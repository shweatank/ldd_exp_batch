#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Single handler for both signals
void handle_signal(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nSIGINT arrived (Ctrl + C pressed)\n");
    }
    else if (sig == SIGTSTP)
    {
        printf("\nSIGTSTP arrived (Ctrl + Z pressed)\n");
	raise(SIGSTOP);
    }
    else
    {
        printf("\nSome other signal arrived: %d\n", sig);
    }
}

int main()
{
    // Register the same handler for both signals
    signal(SIGINT, handle_signal);   // Ctrl + C
    signal(SIGTSTP, handle_signal);  // Ctrl + Z

    while (1)
    {
        printf("Working...\n");
        sleep(1);
    }

    return 0;
}

