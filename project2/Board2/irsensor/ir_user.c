#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IR_MAGIC        'i'
#define IR_GET_STATUS   _IOR(IR_MAGIC, 1, int)

int main()
{
    int fd, status;

    fd = open("/dev/ir_sensor", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    while (1) {
        ioctl(fd, IR_GET_STATUS, &status);

        if (status == 1)
            printf("Object detected!\n");

        sleep(1);
    }

    close(fd);
    return 0;
}

