#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef enum
{
    ADD = 0,
    SUB = 1
} operation_t;

struct calc_data
{
    int num1;
    int num2;
    operation_t op;
};

int main()
{
    int fd = open("/dev/calc_dev", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    struct calc_data data = {10, 5, SUB};

    write(fd, &data, sizeof(data));

    char buf[128];
    int n = read(fd, buf, sizeof(buf));
    buf[n] = '\0';

    printf("Result: %s\n", buf);
    close(fd);
    return 0;
}
