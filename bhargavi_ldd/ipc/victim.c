/* victim_minimal.c */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("victim running... PID=%d\n", getpid());
    printf("waiting for a signal...\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

