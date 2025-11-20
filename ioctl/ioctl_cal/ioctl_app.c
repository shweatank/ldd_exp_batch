#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ioctl.h>

struct calc{
    int a;
    int b;
    char ops;
    int result;
};
#define IOCTL_CALC _IOWR(CALC_MAGIC, 0, struct calc)
#define CALC_MAGIC 'c'
int main()
{
    struct calc cal;
    int fd=open("/dev/ioctl_c",O_RDWR);
    if(fd< 0)
    {
        perror("open");
        return 1;
    }
    printf("enter two numbers and operations to be perform(ex:+,-,*,/):\n");
    scanf("%d %d %c",&cal.a,&cal.b,&cal.ops);

    if(ioctl(fd,IOCTL_CALC,&cal)<0)
    {
        perror("ioctl");
        close(fd);
        return 1;
    }
    printf("result: %d\n",cal.result);
    return 0;
}