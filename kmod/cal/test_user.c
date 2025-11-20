#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct calculator{
    int a;
    int b;
    char buf[20];
};

int main(void)
{
    int fd, result;
    struct calculator cal;

    printf("enter two numbers & operations to be performed:\n");
    scanf("%d %d %s", &cal.a, &cal.b, cal.buf);

    fd = open("/dev/testmod_2", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    write(fd, &cal, sizeof(cal));

    /* reset read pointer */
    lseek(fd, 0, SEEK_SET);

    /* read only result (int) */
    int bytes = read(fd, &result, sizeof(result));

    if (bytes == sizeof(result)) {
        printf("Result from kernel = %d\n", result);
    } else {
        printf("Error reading result\n");
    }

    close(fd);
    return 0;
}
