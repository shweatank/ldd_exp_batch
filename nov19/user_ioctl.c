#include <stdio.h>
#include <fcmtl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/ioctl_demo"

#define MAJOR_NUM 100
#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0 ,int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 1, int)

int main()
{
    int fd;
    int val = 1234;
    int read_val = 0;
    fd = open(DEVICE, O_RDWR);
       if(fd < 0) {
          perror("failed to open device\n");
           return 1;
           }
 
printf("sending values %d to kernel ...\n ",val);
ioctl(fd, IOCTL_SET_NUM, &val);

printf("requesting value fro kernel ..\n");
ioctl(fd, IOCTL_GET_NUM, &val);

printf("received from kernel\n");
close(fd);
return 0;
             
}
