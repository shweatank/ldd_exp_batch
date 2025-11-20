#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/ioctl_demo"

#define MAJOR_NUM 100
#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0 ,int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 1, int)

int main()
{
    int fd;
    int write_val[2];
    char choice;
    int read_val = 0;

    fd = open("/dev/ioctl_calc_dev", O_RDWR);
       if(fd < 0) {
          perror("failed to open device\n");
           return 1;
           }

printf("enter two numbers for cal and command\n");
scanf("%d %d %c",&write_val[0],&write_val[1],&choice);

int ret = write(fd,write_val,sizeof(write_val));
 
 
ioctl(fd, IOCTL_SET_NUM, &choice);

int res;
read(fd, &res, sizeof(res));

printf("received from kernel %d\n",res);

close(fd);
return 0;
             
}
