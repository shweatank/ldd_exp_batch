#include<stdio.h>
#include<unistd.h>

int main(void)
{
    printf("pid: %d\n",getpid());
    printf("Waiting\n");

    while (1)
    {
        sleep(1);
    }

    return 0;
}
