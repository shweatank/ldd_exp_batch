#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
struct stu{
	int a;
	int b;
	int result;
	char op[10];
}x;
int main()
{
	int fd=open("/dev/chrdev",O_RDWR,0777);
	char s[4];
	scanf("%d",&x.a);
	scanf("%d",&x.b);
	scanf(" %c",&x.op[0]);
	write(fd,&x,sizeof(x));
	read(fd,&x,sizeof(x));
	printf("result:--%d\n",x.result);
	close(fd);
	return 0;
}
