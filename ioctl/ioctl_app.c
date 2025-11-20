#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAJOR_NUM 100
#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 0, int)

int main()
{
    int fd, val = 10, out;

    fd = open("/dev/ioctl_demo", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ioctl(fd, IOCTL_SET_NUM, &val);
    ioctl(fd, IOCTL_GET_NUM, &out);

    printf("Value read from kernel: %d\n", out);

    close(fd);
    return 0;
}
