#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/ioctl_demo"

#define MAJOR_NUM 100
#define IOCTL_ADD_NUM     _IOW(MAJOR_NUM, 0, int)
#define IOCTL_SUB_NUM     _IOW(MAJOR_NUM, 1, int)
#define IOCTL_MUL_NUM     _IOW(MAJOR_NUM, 2, int)
#define IOCTL_DIV_NUM     _IOW(MAJOR_NUM, 3, int)
#define IOCTL_GET_RESULT  _IOR(MAJOR_NUM, 4, int)

int main()
{
    int fd, val, result;

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    val = 10;
    ioctl(fd, IOCTL_ADD_NUM, &val);
    printf("Added %d\n", val);

    val = 3;
    ioctl(fd, IOCTL_SUB_NUM, &val);
    printf("Subtracted %d\n", val);

    val = 4;
    ioctl(fd, IOCTL_MUL_NUM, &val);
    printf("Multiplied by %d\n", val);

    val = 2;
    ioctl(fd, IOCTL_DIV_NUM, &val);
    printf("Divided by %d\n", val);

    ioctl(fd, IOCTL_GET_RESULT, &result);
    printf("Final result = %d\n", result);

    close(fd);
    return 0;
}

