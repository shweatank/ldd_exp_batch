#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
int main()
{
    int fd;
    char exp[100];
    char result[100];

    fd = open("/dev/calc_driver", O_RDWR);
    if (fd < 0)
    {
        printf("Error opening device\n");
        return -1;
    }

    printf("Enter expression (ex: 10 + 20): ");
    fgets(exp, sizeof(exp), stdin);

    write(fd, exp, strlen(exp));
    read(fd, result, sizeof(result));

    printf("From Kernel: %s\n", result);

    close(fd);
    return 0;
}

