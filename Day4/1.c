#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
int main()
{
int fd=open("/dev/sai",O_RDWR,0777);
char s[100];
read(0,s,sizeof(s));
write(1,s,sizeof(s));
close(fd);
return 0;
}
