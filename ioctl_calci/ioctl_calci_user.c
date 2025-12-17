#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MAJOR_NUM 100
#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 1, int)

int main() {
    int fd, num = 55;

    fd = open("/dev/ioctl_calci", O_RDWR);

    ioctl(fd, IOCTL_SET_NUM, &num);

    ioctl(fd, IOCTL_GET_NUM, &num);
    printf("Kernel returned: %d\n", num);

    close(fd);
    return 0;
}

