#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ioctl.h>
#define DEVICE "/dev/Interrupt"

#define MAJOR 100
#define IOCTL_SET_NUMBER _IOW(MAJOR,0,int)
#define IOCTL_GET_NUMBER _IOR(MAJOR,1,int)
#define IOCTL_OPE_CHARAC _IOW(MAJOR,2,char)
struct stu{
	int a;
	int b;
	int result;
}x;

int main()
{
	int val=1234;
	int read_val=0;
	int fd=open(DEVICE,O_RDWR);
	char s;
	if(fd<0)
		printf("open fails\n");
//	while(1)
//	{
	scanf("%d%d",&x.a,&x.b);
	write(fd,&x,sizeof(x));
	
	printf("sending value:-- %d to kernel....\n",val);
	ioctl(fd,IOCTL_SET_NUMBER,&val);
	scanf(" %c",&s);
	int i=ioctl(fd,IOCTL_OPE_CHARAC,&s);
	printf("return value:--%d\n",i);
	ioctl(fd,IOCTL_GET_NUMBER,&read_val);
	printf("receving value:--%d from the kernel\n",read_val);
	sleep(10);	
	read(fd,&x,sizeof(x));
	printf("result:--%d\n",x.result);
//	}
	close(fd);

	return 0;
}
